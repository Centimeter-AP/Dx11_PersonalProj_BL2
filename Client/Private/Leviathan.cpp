#include "Leviathan.h"
#include "LeviathanState.h"
#include "GameInstance.h"

CLeviathan::CLeviathan(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster { pDevice, pContext }
{

}

CLeviathan::CLeviathan(const CLeviathan& Prototype)
	: CMonster( Prototype )
{

}

HRESULT CLeviathan::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLeviathan::Initialize(void* pArg)
{
 	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_LeviathanStates()))
		return E_FAIL;
	
	//m_pModelCom->Set_Animation(RAKK_ANIM::Flight_BankL, true);
	m_eCurState = STATE_Flying_Idle;
	Set_State(m_eCurState);
	return S_OK;
}

void CLeviathan::Priority_Update(_float fTimeDelta)
{
#pragma region AnimationTests
	static _uint test = {};
	if (KEY_DOWN(DIK_LBRACKET))
	{
		test > 30 ? test = 0 : test++;
		m_pModelCom->Set_Animation(test, true);
	}
	if (KEY_DOWN(DIK_RBRACKET))
	{
		test < 1 ? test = 30 : test--;
		m_pModelCom->Set_Animation(test, true);
	}
#pragma endregion
	Update_State(fTimeDelta);

}

EVENT CLeviathan::Update(_float fTimeDelta)
{

	return EVN_NONE;
}

void CLeviathan::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CLeviathan::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;


	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			continue;
		//m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0);
		m_pModelCom->Bind_Bone_Matrices(m_pShaderCom, "g_BoneMatrices", i);
		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}
	


	return S_OK;
}

HRESULT CLeviathan::Ready_Components(void* pArg)
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(m_iLevelID, TEXT("Prototype_Component_Model_Warrior"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	return S_OK;
}

HRESULT CLeviathan::Ready_LeviathanStates()
{
	//m_pStates[Leviathan_STATE::STATE_Idle] = new CLeviathanState_Idle(this);

	m_pCurState = m_pStates[LEVI_STATE::STATE_Flying_Idle];
	return S_OK;
}

void CLeviathan::Set_State(LEVI_STATE eState)
{
	m_ePrevState = m_eCurState;
	m_eCurState = eState;

	m_pCurState->Exit();
	m_pCurState = m_pStates[eState];
	m_pCurState->Enter();
}

void CLeviathan::Update_State(_float fTimeDelta)
{
	//if (m_ePrevState != m_eCurState)
	//{
	//	m_pCurState->Enter();
	//	m_ePrevState = m_eCurState;
	//}
	m_pCurState->Execute(fTimeDelta);
}


CLeviathan* CLeviathan::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLeviathan* pInstance = new CLeviathan(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLeviathan::Clone(void* pArg)
{
	CLeviathan* pInstance = new CLeviathan(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLeviathan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLeviathan::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
