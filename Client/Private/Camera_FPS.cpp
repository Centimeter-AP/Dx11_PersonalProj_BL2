#include "Camera_FPS.h"
#include "GameInstance.h"
#include "Player.h"
#include "Model.h"

CCamera_FPS::CCamera_FPS(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera { pDevice, pContext }
{

}

CCamera_FPS::CCamera_FPS(const CCamera_FPS& Prototype)
	: CCamera (Prototype)
{

}

HRESULT CCamera_FPS::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_FPS::Initialize(void* pArg)
{
	CCamera::CAMERA_DESC			Desc{};

	if (FAILED(Initialize_Player()))
		return E_FAIL;

	Desc.vEye = _float3(0.f, 20.f, -15.f);
	Desc.vAt = _float3(0.f, 0.f, 0.f);
	Desc.fFov = XMConvertToRadians(45.0f); // 
	Desc.fNear = 0.1f;
	Desc.fFar = 1000.f;
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 10.0f;
	Desc.szName = TEXT("Camera_FPS");

	m_fSensor = 0.1f;
	m_isUsing = true;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Set_PlayerBone()))
		return E_FAIL;

	return S_OK;
}

void CCamera_FPS::Priority_Update(_float fTimeDelta)
{
	if (!m_isUsing)
		return;

	Mouse_Fix();


#ifdef _CONSOLE
	static _float consoleTicker = {};
	consoleTicker += fTimeDelta;
	if (consoleTicker >= 1.f)
	{
		// 쓰고 싶은 것..
		consoleTicker = 0.f;
	}
#endif // _CONSOLE

}

EVENT CCamera_FPS::Update(_float fTimeDelta)
{
	if (!m_isUsing)
		return EVN_NONE;

	Update_Camera(fTimeDelta);
	__super::Bind_Matrices();
	//m_pTransformCom->Go_Straight(fTimeDelta);
	//m_pTransformCom->Go_Left(fTimeDelta);
	return EVN_NONE;
}

void CCamera_FPS::Late_Update(_float fTimeDelta)
{

}

HRESULT CCamera_FPS::Render()
{
	
	return S_OK;
}

HRESULT CCamera_FPS::Initialize_Player()
{
	m_pPlayer = GET_PLAYER;
	m_pPlayerModel = dynamic_cast<CModel*>(m_pPlayer->Get_Component(L"Com_Model"));
	m_pPlayerTransform = m_pPlayer->Get_Transform();
	m_szPlayerCameraBoneName = "Camera";

	if (FAILED(Set_PlayerBone()))
		return E_FAIL;
	return S_OK;
}

void CCamera_FPS::Start_Recoil()
{
	// Y:+위, Z:-뒤로 (모델 축 기준; 필요에 따라 조정)

	m_vRecoilOffset = { 0.f, 0.0035f, -0.0075f };
	m_vRecoilVelocity = { 0.f, 0.f, 0.f };
	m_bRecoiling = true;
}

HRESULT CCamera_FPS::Set_PlayerBone()
{
	// 본 인덱스 얻기
	m_iBoneIndex = m_pPlayerModel->Find_BoneIndex(m_szPlayerCameraBoneName.c_str());
	if (m_iBoneIndex <= 0) return E_FAIL;
	return S_OK;
}

void CCamera_FPS::Mouse_Fix()
{
	POINT		ptMouse{ g_iWinSizeX >> 1, g_iWinSizeY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CCamera_FPS::Update_Camera(_float fTimeDelta)
{
	// 월드 행렬 적용 (원래 셰이더에서 월드 곱해주기때문에 지금은 로컬상태임)
	_matrix matWorld = m_pPlayerTransform->Get_WorldMatrix();
	_matrix matBone = XMLoadFloat4x4(m_pPlayerModel->Get_CombinedTransformationMatrix(m_iBoneIndex)) * matWorld;

	// Eye / At 추출
	_vector vEyeBase = XMVectorSetW(matBone.r[3], 1.f);
	_vector vLookBase = XMVectorSetW(XMVector4Normalize(matBone.r[0]), 0.f);

	Update_Recoil(fTimeDelta);

	//_vector vRecoilOffsetVec = XMLoadFloat3(&m_vRecoilOffset);
	//_vector vEyeFinal = vEyeBase + vRecoilOffsetVec;
	//_vector vLookFinal = vLookBase;

	//if (m_bRecoiling)
	//{
	//	// 예: m_vRecoilOffset.x 만큼 X축 회전
	//	_matrix pitchRot = XMMatrixRotationAxis(
	//		XMVectorSet(1.f, 0.f, 0.f, 0.f),
	//		XMVectorGetY(vRecoilOffsetVec) * -5.f // 감도 조절용 계수
	//	);
	//	vLookFinal = XMVector3TransformNormal(vLookBase, pitchRot);
	//}





		// 3) 뼈(matBone) 기준 로컬 축 계산
	const _vector vWorldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	// Right = Up × Look
	_vector vRightLocal = XMVector3Normalize(
		XMVector3Cross(vWorldUp, vLookBase)
	);
	// Up    = Look × Right
	_vector vUpLocal = XMVector3Cross(vLookBase, vRightLocal);

	// 4) 로컬 오프셋 → 월드 오프셋
	_vector vOffsetLocal = XMLoadFloat3(&m_vRecoilOffset);
	_vector vOffWorld =
		vRightLocal * m_vRecoilOffset.x
		+ vUpLocal * m_vRecoilOffset.y
		+ vLookBase * m_vRecoilOffset.z;

	// 5) 최종 Eye 위치 적용
	_vector vEyeFinal = vEyeBase + vOffWorld;
	m_pTransformCom->Set_State(STATE::POSITION, vEyeFinal);

	// 6) (선택) 회전 반동—피치만 예시
	_vector vLookFinal = vLookBase;
	if (m_bRecoiling)
	{
		// Y 오프셋 만큼 피치 회전
		_matrix rotPitch = XMMatrixRotationAxis(
			vRightLocal,
			m_vRecoilOffset.y * -5.f
		);
		vLookFinal = XMVector3TransformNormal(vLookBase, rotPitch);
	}










	// 5) Transform에 적용
	m_pTransformCom->Set_State(STATE::POSITION, vEyeBase);
	m_pTransformCom->LookWithLook(vLookFinal);






	//m_pTransformCom->Set_State(STATE::POSITION, vEye);
	//m_pTransformCom->LookWithLook(vLook);
}

void CCamera_FPS::Update_Recoil(_float fTimeDelta)
{
	if (!m_bRecoiling)
		return;

	// 스프링 강성·댐핑 계수
	const _float kSpring = 40.f;
	const _float kDamping = 8.f;

	// 현재 오프셋·속도 로드
	_vector offset = XMLoadFloat3(&m_vRecoilOffset);
	_vector velocity = XMLoadFloat3(&m_vRecoilVelocity);

	// 가속도 = -k·offset - d·velocity
	_vector accel = offset * (-kSpring) - velocity * kDamping;

	// 속도·위치 적분
	velocity = velocity + accel * fTimeDelta;
	offset = offset + velocity * fTimeDelta;

	// 다시 저장
	XMStoreFloat3(&m_vRecoilOffset, offset);
	XMStoreFloat3(&m_vRecoilVelocity, velocity);

	// 거의 복원됐으면 종료
	if (XMVectorGetX(XMVector3Length(offset)) < 0.0001f &&
		XMVectorGetX(XMVector3Length(velocity)) < 0.0001f)
	{
		m_bRecoiling = false;
		m_vRecoilOffset = { 0.f,0.f,0.f };
		m_vRecoilVelocity = { 0.f,0.f,0.f };
	}
}

CCamera_FPS* CCamera_FPS::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_FPS* pInstance = new CCamera_FPS(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_FPS::Clone(void* pArg)
{
	CCamera_FPS* pInstance = new CCamera_FPS(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_FPS");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_FPS::Free()
{
	__super::Free();


}
