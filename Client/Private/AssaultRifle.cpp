#include "AssaultRifle.h"
#include "ARState.h"
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
	DESC* pDesc = static_cast<DESC*>(pArg);
	if (pDesc == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_ARStates()))
		return E_FAIL;

	m_pModelCom->Set_Animation(3, false);
	m_pModelCom->Set_Animation_TickPerSecond_All(30.f);
	m_pModelCom->Play_Animation(0.01f);
	return S_OK;
}

void CAssaultRifle::Priority_Update(_float fTimeDelta)
{
	Update_State(fTimeDelta);
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
}

void CAssaultRifle::Set_State(AR_STATE eState)
{
	m_ePrevState = m_eCurState;
	m_eCurState = eState;

	m_pCurState->Exit();
	m_pCurState = m_pStates[eState];
}

void CAssaultRifle::Update_State(_float fTimeDelta)
{
	if (m_ePrevState != m_eCurState)
	{
		m_pCurState->Enter();
		m_ePrevState = m_eCurState;
	}
	m_pCurState->Execute(fTimeDelta);
}


HRESULT CAssaultRifle::Ready_Components(void* pArg)
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_AR"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CAssaultRifle::Ready_ARStates()
{
	m_pStates[AR_STATE::STATE_Idle] = new CARState_Idle(this);
	m_pStates[AR_STATE::STATE_Draw] = new CARState_Draw(this);
	m_pStates[AR_STATE::STATE_Fire] = new CARState_Fire(this);
	m_pStates[AR_STATE::STATE_Reload] = new CARState_Reload(this);
	m_pStates[AR_STATE::STATE_Reload_Fast] = new CARState_Reload_Fast(this);
	m_pCurState = m_pStates[AR_STATE::STATE_Idle];

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
	for (auto State : m_pStates)
		Safe_Delete(State);
}
