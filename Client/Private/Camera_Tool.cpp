#include "Camera_Tool.h"
#include "GameInstance.h"

CCamera_Tool::CCamera_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera { pDevice, pContext }
{

}

CCamera_Tool::CCamera_Tool(const CCamera_Tool& Prototype)
	: CCamera (Prototype)
{

}

HRESULT CCamera_Tool::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Tool::Initialize(void* pArg)
{
	CCamera::CAMERA_DESC			Desc{};

	Desc.vEye = _float3(0.f, 20.f, -15.f);
	Desc.vAt = _float3(0.f, 0.f, 0.f);
	Desc.fFov = XMConvertToRadians(60.0f);
	Desc.fNear = 0.1f;
	Desc.fFar = 500.f;
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 10.0f;
	lstrcpy(Desc.szName, TEXT("Camera"));

	m_fSensor = 0.01f;
	m_isUsing = true;
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Tool::Priority_Update(_float fTimeDelta)
{
	// if (GetKeyState('A') & 0x8000)

	if (!m_isUsing)
		return;

	if (m_pGameInstance->Get_DIMouseState(DIM::RBUTTON) & 0x80)
	{
		if (m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
		{
			m_pTransformCom->Go_Left(fTimeDelta);
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
		{
			m_pTransformCom->Go_Right(fTimeDelta);
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
		{
			m_pTransformCom->Go_Straight_Hover(fTimeDelta);
		}
		if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
		{
			m_pTransformCom->Go_Backward_Hover(fTimeDelta);
		}

		_long		MouseMove = {};
		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM::X))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMove * fTimeDelta * m_fSensor);
		}

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM::Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), MouseMove * fTimeDelta * m_fSensor);
		}

		_long		MouseWheel = {};
		if (MouseWheel = m_pGameInstance->Get_DIMouseMove(DIMM::WHEEL))
		{
			if (MouseWheel > 0)
			{
				m_pTransformCom->Go_Straight(MouseWheel * fTimeDelta * m_fSensor);
			}	
			else
			{
				m_pTransformCom->Go_Backward(-MouseWheel * fTimeDelta * m_fSensor);
			}
		}
	}

	__super::Bind_Matrices();
}

EVENT CCamera_Tool::Update(_float fTimeDelta)
{

	if (!m_isUsing)
		return EVN_NONE;
	return EVN_NONE;
}

void CCamera_Tool::Late_Update(_float fTimeDelta)
{

	if (!m_isUsing)
		return;
}

HRESULT CCamera_Tool::Render()
{
	
	return S_OK;
}

CCamera_Tool* CCamera_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Tool* pInstance = new CCamera_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Tool::Clone(void* pArg)
{
	CCamera_Tool* pInstance = new CCamera_Tool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Tool::Free()
{
	__super::Free();


}
