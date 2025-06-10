#include "Stalker.h"
#include "StalkerState.h"
#include "GameInstance.h"

CStalker::CStalker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster { pDevice, pContext }
{

}

CStalker::CStalker(const CStalker& Prototype)
	: CMonster( Prototype )
{

}

HRESULT CStalker::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStalker::Initialize(void* pArg)
{
 	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_StalkerStates()))
		return E_FAIL;
	
	//m_pModelCom->Set_Animation(RAKK_ANIM::Flight_BankL, true);
	m_eCurState = STATE_Flying_Idle;
	Set_State(m_eCurState);

	m_fAttackableDistance = 5.f;
	m_fDetectiveDistance = 20.f;


	return S_OK;
}

void CStalker::Priority_Update(_float fTimeDelta)
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

EVENT CStalker::Update(_float fTimeDelta)
{
	
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	return __super::Update(fTimeDelta);
}

void CStalker::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CStalker::Render()
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
	
	m_pColliderCom->Set_ColliderColor(RGBA_GREEN);
	m_pColliderCom->Render();

	return S_OK;
}

HRESULT CStalker::Ready_Components(void* pArg)
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(m_iLevelID, TEXT("Prototype_Component_Model_Rakk"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::AABB_DESC AABBDesc = {};
	AABBDesc.pOwner = this;
	AABBDesc.eType = COLLIDER::AABB;
	AABBDesc.iColliderGroup = ENUM_CLASS(COL_GROUP::MONSTER);

	AABBDesc.iColliderID = ENUM_CLASS(COL_ID::MONSTER_RAKK);
	AABBDesc.vExtents = _float3(0.4f, 0.3f, 0.3f);
	AABBDesc.vCenter = _float3(0.0f, -AABBDesc.vExtents.y, 0.f);
	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CStalker::Ready_StalkerStates()
{
	m_pStates[STALKER_STATE::STATE_Idle] = new CStalkerState_Idle(this);
	m_pStates[STALKER_STATE::STATE_Dead] = new CStalkerState_Dead(this);

	m_pCurState = m_pStates[STALKER_STATE::STATE_Idle];
	return S_OK;
}

void CStalker::Set_State(STALKER_STATE eState)
{
	m_ePrevState = m_eCurState;
	m_eCurState = eState;

	m_pCurState->Exit();
	m_pCurState = m_pStates[eState];
	m_pCurState->Enter();
}

void CStalker::Update_State(_float fTimeDelta)
{
	//if (m_ePrevState != m_eCurState)
	//{
	//	m_pCurState->Enter();
	//	m_ePrevState = m_eCurState;
	//}
	m_pCurState->Execute(fTimeDelta);
}

void CStalker::Set_State_Dead()
{
	Set_State(STALKER_STATE::STATE_Dead);
}


CStalker* CStalker::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStalker* pInstance = new CStalker(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStalker::Clone(void* pArg)
{
	CStalker* pInstance = new CStalker(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CStalker");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStalker::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
