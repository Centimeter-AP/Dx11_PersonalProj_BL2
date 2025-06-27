#include "Leviathan.h"
#include "LeviathanState.h"
#include "GameInstance.h"
#include "Levi_HitMesh.h"

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
	
	//if (FAILED(Ready_PartObjects(pArg)))
	//	return E_FAIL;

	if (FAILED(Ready_LeviathanStates()))
		return E_FAIL;
	
	//m_pModelCom->Set_Animation(RAKK_ANIM::Flight_BankL, true);
	m_eCurState = STATE_Engage;
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

	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Priority_Update(fTimeDelta);
	}

	m_pColliderCom->Set_ColliderColor(RGBA_GREEN);
	Update_State(fTimeDelta);

}

EVENT CLeviathan::Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Update(fTimeDelta);
	}

	return EVN_NONE;
}

void CLeviathan::Late_Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Late_Update(fTimeDelta);
	}
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
	if (FAILED(__super::Add_Component(m_iLevelID, TEXT("Prototype_Component_Model_Leviathan"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::AABB_DESC AABBDesc = {};
	AABBDesc.pOwner = this;
	AABBDesc.eType = COLLIDER::AABB;
	AABBDesc.iColliderGroup = ENUM_CLASS(COL_GROUP::MONSTER);
	AABBDesc.iColliderID = ENUM_CLASS(COL_ID::MONSTER_BOSS_LEVIATHAN);
	AABBDesc.vExtents = _float3(100.f, 100.f, 100.f);
	AABBDesc.vCenter = _float3(0.f, -50.f, 0.f);
	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLeviathan::Ready_PartObjects(void* pArg)
{
	if (FAILED(Ready_HitMeshes(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLeviathan::Ready_HitMeshes(void* pArg)
{
	CLevi_HitMesh::DESC Desc = {};

	Desc.fRotationPerSec = XMConvertToRadians(0.f);
	Desc.fSpeedPerSec = 0.f;
	Desc.pParentMatrix = m_pTransformCom->Get_WorldMatrix4x4Ptr();
	Desc.pParentObject = this;

	/* For.PartObject_HitMesh_Tongue */
	Desc.strSocketMatrixName = ("tongue_jaw");
	Desc.bHasTransformPreset = true;
	XMStoreFloat4x4(&Desc.PresetMatrix, XMMatrixTranslation(0.f, 0.f, 0.f));
	Desc.strVIBufferTag = TEXT("Prototype_Component_Model_Levi_HitMesh");
	Desc.szName = L"Hitmesh_Tongue";
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::BOSS), TEXT("PartObject_HitMesh_Tongue"), TEXT("Prototype_GameObject_Levi_HitMesh"), &Desc)))
		return E_FAIL;

	/* For.PartObject_HitMesh_LeftEye */
	Desc.strSocketMatrixName = ("Head"); // ¿Þ´« ¿À¸¥´«
	Desc.bHasTransformPreset = true;
	XMStoreFloat4x4(&Desc.PresetMatrix, XMMatrixTranslation(0.f, 0.f, 0.f));
	Desc.strVIBufferTag = TEXT("Prototype_Component_Model_Levi_HitMesh");
	Desc.szName = L"Hitmesh_LeftEye";
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::BOSS), TEXT("PartObject_HitMesh_LeftEye"), TEXT("Prototype_GameObject_Levi_HitMesh"), &Desc)))
		return E_FAIL;
	/* For.PartObject_HitMesh_RightEye */
	Desc.strSocketMatrixName = ("Head"); // ¿Þ´« ¿À¸¥´«
	Desc.bHasTransformPreset = true;
	XMStoreFloat4x4(&Desc.PresetMatrix, XMMatrixTranslation(0.f, 0.f, 0.f));
	Desc.strVIBufferTag = TEXT("Prototype_Component_Model_Levi_HitMesh");
	Desc.szName = L"Hitmesh_RightEye";
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::BOSS), TEXT("PartObject_HitMesh_RightEye"), TEXT("Prototype_GameObject_Levi_HitMesh"), &Desc)))
		return E_FAIL;

	/* For.PartObject_HitMesh_Heart */
	Desc.strSocketMatrixName = ("upper_fat"); // ½ÉÀå
	Desc.bHasTransformPreset = true;
	XMStoreFloat4x4(&Desc.PresetMatrix, XMMatrixTranslation(0.f, 0.f, 0.f));
	Desc.strVIBufferTag = TEXT("Prototype_Component_Model_Levi_HitMesh");
	Desc.szName = L"Hitmesh_Heart";
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::BOSS), TEXT("PartObject_HitMesh_Heart"), TEXT("Prototype_GameObject_Levi_HitMesh"), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLeviathan::Ready_LeviathanStates()
{
	m_pStates[LEVI_STATE::STATE_Engage] = new CLeviathanState_Engage(this);
	m_pStates[LEVI_STATE::STATE_Idle] = new CLeviathanState_Idle(this);
	m_pStates[LEVI_STATE::STATE_Attack_Spray] = new CLeviathanState_Attack_Spray(this);
	m_pStates[LEVI_STATE::STATE_Attack_ThrowRock] = new CLeviathanState_Attack_ThrowRock(this);
	m_pStates[LEVI_STATE::STATE_Attack_Slam] = new CLeviathanState_Attack_Slam(this);
	m_pStates[LEVI_STATE::STATE_Attack_SpitWorm] = new CLeviathanState_Attack_SpitWorm(this);
	m_pStates[LEVI_STATE::STATE_Damaged] = new CLeviathanState_Damaged(this);
	m_pStates[LEVI_STATE::STATE_Stun_Left] = new CLeviathanState_Stun_Left(this);
	m_pStates[LEVI_STATE::STATE_Stun_Right] = new CLeviathanState_Stun_Right(this);
	m_pStates[LEVI_STATE::STATE_LookAround] = new CLeviathanState_LookAround(this);
	m_pStates[LEVI_STATE::STATE_Dead] = new CLeviathanState_Dead(this);

	m_pCurState = m_pStates[LEVI_STATE::STATE_Engage];
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

void CLeviathan::On_Collision(_uint iMyColID, _uint iHitColID, CCollider* pHitCol)
{
	if (static_cast<COL_ID>(iMyColID) == COL_ID::MONSTER_BOSS_LEVIATHAN)
		return ;

	__super::On_Collision(iMyColID, iHitColID, pHitCol);

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

	for (auto& State : m_pStates)
		Safe_Release(State);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
