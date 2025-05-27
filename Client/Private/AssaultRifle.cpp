#include "AssaultRifle.h"

#include "GameInstance.h"

CAssaultRifle::CAssaultRifle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWeapon{ pDevice, pContext }
{

}

CAssaultRifle::CAssaultRifle(const CAssaultRifle& Prototype)
	: CWeapon( Prototype )
{

}

HRESULT CAssaultRifle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAssaultRifle::Initialize(void* pArg)
{
	GAMEOBJECT_DESC			Desc{};

	Desc.fRotationPerSec = 0.f;
	Desc.fSpeedPerSec = 0.f;
	lstrcpy(Desc.szName, TEXT("AssaultRifle"));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_pModelCom->Set_Animation(3, false);
	m_pModelCom->Set_Animation_TickPerSecond_All(30.f);
	return S_OK;
}

void CAssaultRifle::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
#pragma region AnimationTests
	static _uint test = {};
	if (KEY_DOWN(DIK_RBRACKET))
	{
		++test == AR_ANIMATION_END ? test = 0 : test;
		m_pModelCom->Set_Animation(test, true);
	}
	if (KEY_DOWN(DIK_LBRACKET))
	{
		test < 1 ? test = 74 : test--;
		m_pModelCom->Set_Animation(test, true);
	}
#pragma endregion
}

EVENT CAssaultRifle::Update(_float fTimeDelta)
{
	
	return __super::Update(fTimeDelta);
}

void CAssaultRifle::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CAssaultRifle::Render()
{
	return __super::Render();
	return S_OK;
}

HRESULT CAssaultRifle::Ready_Components(void* pArg)
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_AR"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	return S_OK;
}

CAssaultRifle* CAssaultRifle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAssaultRifle* pInstance = new CAssaultRifle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAssaultRifle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAssaultRifle::Clone(void* pArg)
{
	CAssaultRifle* pInstance = new CAssaultRifle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAssaultRifle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAssaultRifle::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
