#include "Pistol.h"
#include "PSTState.h"
#include "GameInstance.h"

CPistol::CPistol(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWeapon{ pDevice, pContext }
{

}

CPistol::CPistol(const CPistol& Prototype)
	: CWeapon(Prototype)
{

}

HRESULT CPistol::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPistol::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);
	if (pDesc == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PSTStates()))
		return E_FAIL;

	m_pModelCom->Set_Animation(3, false);
	m_pModelCom->Set_Animation_TickPerSecond_All(30.f);
	return S_OK;
}

void CPistol::Priority_Update(_float fTimeDelta)
{
	Update_State(fTimeDelta);
	__super::Priority_Update(fTimeDelta);
#pragma region AnimationTests
	static _uint test = {};
	if (KEY_DOWN(DIK_RBRACKET))
	{
		++test == PST_ANIMATION_END ? test = 0 : test;
		m_pModelCom->Set_Animation(test, true);
	}
	if (KEY_DOWN(DIK_LBRACKET))
	{
		test < 1 ? test = 74 : test--;
		m_pModelCom->Set_Animation(test, true);
	}
#pragma endregion
}

EVENT CPistol::Update(_float fTimeDelta)
{

	return __super::Update(fTimeDelta);
}

void CPistol::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPistol::Render()
{
	return __super::Render();
}

void CPistol::Set_State(PST_STATE eState)
{
	m_ePrevState = m_eCurState;
	m_eCurState = eState;

	m_pCurState->Exit();
	m_pCurState = m_pStates[eState];
}

void CPistol::Update_State(_float fTimeDelta)
{
	if (m_ePrevState != m_eCurState)
	{
		m_pCurState->Enter();
		m_ePrevState = m_eCurState;
	}
	m_pCurState->Execute(fTimeDelta);
}


HRESULT CPistol::Ready_Components(void* pArg)
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Pistol"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPistol::Ready_PSTStates()
{
	m_pStates[PST_STATE::STATE_Idle] = new CPSTState_Idle(this);
	m_pStates[PST_STATE::STATE_Draw] = new CPSTState_Draw(this);
	m_pStates[PST_STATE::STATE_Fire] = new CPSTState_Fire(this);
	m_pStates[PST_STATE::STATE_Reload] = new CPSTState_Reload(this);
	m_pStates[PST_STATE::STATE_Reload_Fast] = new CPSTState_Reload_Fast(this);
	m_pCurState = m_pStates[PST_STATE::STATE_Idle];

	return S_OK;
}

CPistol* CPistol::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPistol* pInstance = new CPistol(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPistol");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPistol::Clone(void* pArg)
{
	CPistol* pInstance = new CPistol(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPistol");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPistol::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	for (auto State : m_pStates)
		Safe_Delete(State);
}
