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
	Desc.fNear = 0.25f;
	Desc.fFar = 500.f;
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

	Update_Camera();
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

void CCamera_FPS::Update_Camera()
{
	// 월드 행렬 적용 (원래 셰이더에서 월드 곱해주기때문에 지금은 로컬상태임)
	_matrix matWorld = m_pPlayerTransform->Get_WorldMatrix();
	_matrix matFinal = XMLoadFloat4x4(m_pPlayerModel->Get_CombinedTransformationMatrix(m_iBoneIndex)) * matWorld;

	// Eye / At 추출
	_vector vEye = XMVectorSetW(matFinal.r[3], 1.f);
	_vector vLook = XMVectorSetW(XMVector4Normalize(matFinal.r[0]), 0.f);
	//_vector vLook = XMVectorSetW(XMVector4Normalize(m_pPlayerTransform->Get_State(STATE::LOOK)), 0.f);

	//_vector vAtVec = vEye + vLook * 10.f;

	m_pTransformCom->Set_State(STATE::POSITION, vEye);
	m_pTransformCom->LookWithLook(vLook);
	//m_pTransformCom->LookAt(XMVectorSetW(vAtVec, 1.f));
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
