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
	Desc.fFar = 1000.f;
	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = 10.0f;
	Desc.szName = TEXT("Camera");

	m_fSensor = 0.1f;
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
	if (KEY_PRESSING(DIK_LSHIFT))
		m_fSpeedMultiplier = 4.f;
	else
		m_fSpeedMultiplier = 1.f;

	if (MOUSE_PRESSING(DIM::RBUTTON))
	{
		if (KEY_PRESSING(DIK_A))
		{
			m_pTransformCom->Go_Left(fTimeDelta * m_fSpeedMultiplier);
		}
		if (KEY_PRESSING(DIK_D))
		{
			m_pTransformCom->Go_Right(fTimeDelta * m_fSpeedMultiplier);
		}
		if (KEY_PRESSING(DIK_W))
		{
			m_pTransformCom->Go_Straight_Hover(fTimeDelta * m_fSpeedMultiplier);
		}
		if (KEY_PRESSING(DIK_S))
		{
			m_pTransformCom->Go_Backward_Hover(fTimeDelta * m_fSpeedMultiplier);
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
				m_pTransformCom->Go_Straight(MouseWheel * fTimeDelta * m_fSensor * m_fSpeedMultiplier);
			}	
			else
			{
				m_pTransformCom->Go_Backward(-MouseWheel * fTimeDelta * m_fSensor * m_fSpeedMultiplier);
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
