#include "Leviathan.h"
#include "LeviathanState.h"
#include "GameInstance.h"
#include "Levi_HitMesh.h"
#include "Levi_Bullet.h"

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
	
	if (FAILED(Ready_PartObjects(pArg)))
		return E_FAIL;

	if (FAILED(Ready_LeviathanStates()))
		return E_FAIL;
	
	//m_pModelCom->Set_Animation(RAKK_ANIM::Flight_BankL, true);
	m_eCurState = STATE_Engage;
	Set_State(m_eCurState);

	m_iTongueBoneIdx = m_pModelCom->Find_BoneIndex("tongue_jaw");


	return S_OK;
}

void CLeviathan::Priority_Update(_float fTimeDelta)
{
	m_iHP = 0;

	if (KEY_DOWN(DIK_P))
	{
		m_PartObjects.begin()->second->Set_Dead();
	}
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
		{
			pPartObject.second->Priority_Update(fTimeDelta);
			m_iHP += static_cast<CLevi_HitMesh*>(pPartObject.second)->Get_HP();
		}
	}
	if (m_iHP <= 0 && m_bDying == false)
	{
		Set_State(LEVI_STATE::STATE_Dead);
		m_bDying = true;
	}

	m_pColliderCom->Set_ColliderColor(RGBA_GREEN);
	Update_State(fTimeDelta);
	static _float fTime = 0.f;
	fTime += fTimeDelta;
	if (fTime >= 1.f)
	{
		cout << "Leviathan HP : " << m_iHP << endl;
		fTime = 0.f;
	}
}

EVENT CLeviathan::Update(_float fTimeDelta)
{
	Update_PartObjects(fTimeDelta);

	m_pColliderGroundAttackCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	if (KEY_DOWN(DIK_Z))
		Spawn_Bullet();
	return EVN_NONE;
}

HRESULT CLeviathan::Update_PartObjects(_float fTimeDelta)
{
	for (auto iter = m_PartObjects.begin(); iter != m_PartObjects.end(); )
	{
		EVENT Events = { EVENT::EVN_NONE };
		if (nullptr != (iter->second))
			Events = (iter->second)->Update(fTimeDelta);
		if (Events == EVENT::EVN_DEAD)
		{
			if (iter->first == TEXT("PartObject_HitMesh_LeftEye"))
				Set_State(LEVI_STATE::STATE_Stun_Left);
			else if (iter->first == TEXT("PartObject_HitMesh_RightEye"))
				Set_State(LEVI_STATE::STATE_Stun_Right);
			m_pGameInstance->Delete_Collider(iter->second);
			Safe_Release(iter->second);
			iter = m_PartObjects.erase(iter);
		}
		else
			++iter;
	}
	return S_OK;
}

void CLeviathan::Late_Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Late_Update(fTimeDelta);
	}
	m_pColliderCom->Set_ColliderColor(RGBA_GREEN);
	m_pColliderGroundAttackCom->Set_ColliderColor(RGBA_MAGENTA);
	m_pGameInstance->Add_DebugComponent(m_pColliderGroundAttackCom);
	__super::Late_Update(fTimeDelta);
}

HRESULT CLeviathan::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			int a = 0;

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS, 0)))
			int a = 0;

		m_pModelCom->Bind_Bone_Matrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_EmissiveTexture", i, aiTextureType_EMISSIVE, 0)))
		{
			if (FAILED(m_pShaderCom->Begin(ANIMMESH_DEFAULT)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pShaderCom->Begin(ANIMMESH_EMISSIVE)))
				return E_FAIL;
		}


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
	AABBDesc.iColliderGroup = ENUM_CLASS(COL_GROUP::BOSS);
	AABBDesc.iColliderID = ENUM_CLASS(COL_ID::MONSTER_BOSS_LEVIATHAN);
	AABBDesc.vExtents = _float3(50.f, 50.f, 50.f);
	AABBDesc.vCenter = _float3(0.f, AABBDesc.vExtents.y, 0.f);
	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBDesc)))
		return E_FAIL;

	//CBounding_AABB::AABB_DESC AABBDesc = {};
	//AABBDesc.pOwner = this;
	//AABBDesc.eType = COLLIDER::AABB;
	AABBDesc.iColliderGroup = ENUM_CLASS(COL_GROUP::MON_ATTACK);
	AABBDesc.iColliderID = ENUM_CLASS(COL_ID::MONSTER_BOSS_LEVIATHAN_ATK);
	AABBDesc.vExtents = _float3(40.f, 5.f, 40.f);
	AABBDesc.vCenter = _float3(0.f, 45.f, -70.f);
	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_ColliderAttack"), reinterpret_cast<CComponent**>(&m_pColliderGroundAttackCom), &AABBDesc)))
		return E_FAIL;
	m_pColliderGroundAttackCom->Set_Active(false);

	//AABBDesc.iColliderGroup = ENUM_CLASS(COL_GROUP::MON_ATTACK);
	//AABBDesc.iColliderID = ENUM_CLASS(COL_ID::MONSTER_BOSS_LEVIATHAN_ATK);
	//AABBDesc.vExtents = _float3(40.f, 5.f, 40.f);
	//AABBDesc.vCenter = _float3(0.f, 45.f, -70.f);
	///* For.Com_Collider */
	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
	//	TEXT("Com_ColliderAttack"), reinterpret_cast<CComponent**>(&m_pColliderGroundAttackCom), &AABBDesc)))
	//	return E_FAIL;
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

	Desc.iLevelID = ENUM_CLASS(LEVEL::BOSS);
	Desc.fRotationPerSec = XMConvertToRadians(0.f);
	Desc.fSpeedPerSec = 0.f;
	Desc.pParentMatrix = m_pTransformCom->Get_WorldMatrix4x4Ptr();
	Desc.pParentObject = this;

	// ¾ÕµÚ/À§¾Æ·¡/ÁÂ¿ì

	/* For.PartObject_HitMesh_Tongue */
	Desc.strSocketMatrixName = ("tongue_head"); // ¾È¸Â¾Æ¼­ ¸øÇÏ°Ù¾î //¸Â¾Ò¾î
	Desc.bHasTransformPreset = true;													
	XMStoreFloat4x4(&Desc.PresetMatrix, XMMatrixScaling(0.8f, 0.8f, 0.8f)
		* XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), 0.f)
		* XMMatrixTranslation(0.f, 2.f, 6.f));// -ÁÂ +¿ì / +¾Õ -µÚ / -À§ +¾Æ·¡
	Desc.strVIBufferTag = TEXT("Prototype_Component_Model_Levi_HitMesh");
	Desc.szName = L"Hitmesh_Tongue";
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::BOSS), TEXT("PartObject_HitMesh_Tongue"), TEXT("Prototype_GameObject_Levi_HitMesh"), &Desc)))
		return E_FAIL;

	/* For.PartObject_HitMesh_LeftEye */
	Desc.strSocketMatrixName = ("Head"); // ¿Þ´« ¿À¸¥´«
	Desc.bHasTransformPreset = true;
	XMStoreFloat4x4(&Desc.PresetMatrix, XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationRollPitchYaw(XMConvertToRadians(-25.f), XMConvertToRadians(90.f), 0.f) * XMMatrixTranslation(10.f, 0.f, -6.f));
	Desc.strVIBufferTag = TEXT("Prototype_Component_Model_Levi_HitMesh");
	Desc.szName = L"Hitmesh_LeftEye";
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::BOSS), TEXT("PartObject_HitMesh_LeftEye"), TEXT("Prototype_GameObject_Levi_HitMesh"), &Desc)))
		return E_FAIL;
	/* For.PartObject_HitMesh_RightEye */
	Desc.strSocketMatrixName = ("Head"); // ¿Þ´« ¿À¸¥´«
	Desc.bHasTransformPreset = true;
	XMStoreFloat4x4(&Desc.PresetMatrix, XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationRollPitchYaw(XMConvertToRadians(25.f), XMConvertToRadians(90.f), 0.f) * XMMatrixTranslation(-10.f, 0.f, -6.f));
	Desc.strVIBufferTag = TEXT("Prototype_Component_Model_Levi_HitMesh");
	Desc.szName = L"Hitmesh_RightEye";
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::BOSS), TEXT("PartObject_HitMesh_RightEye"), TEXT("Prototype_GameObject_Levi_HitMesh"), &Desc)))
		return E_FAIL;

	/* For.PartObject_HitMesh_Heart */
	Desc.strSocketMatrixName = ("upper_fat"); // ½ÉÀå
	Desc.bHasTransformPreset = true; 
	XMStoreFloat4x4(&Desc.PresetMatrix, XMMatrixScaling(0.8f, 0.8f, 0.8f)* XMMatrixRotationRollPitchYaw(XMConvertToRadians(-15.f), XMConvertToRadians(90.f), 0.f)* XMMatrixTranslation(-2.f, 23.f, 6.5f));
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

HRESULT CLeviathan::Spawn_Bullet()
{
	auto TailBoneMat = XMLoadFloat4x4(m_pModelCom->Get_CombinedTransformationMatrix(m_iTongueBoneIdx)) * m_pTransformCom->Get_WorldMatrix();

	CLevi_Bullet::DESC SpitDesc = {};
	SpitDesc.bHasTransformPreset = true;
	SpitDesc.iLevelID = ENUM_CLASS(LEVEL::BOSS);
	SpitDesc.fSpeedPerSec = 10.f;
	SpitDesc.iDamage = m_iDamage;
	_vector targetPos = m_pTarget->Get_Transform()->Get_State(STATE::POSITION);
	XMStoreFloat3(&SpitDesc.vTargetPos, XMVectorSetY(targetPos, targetPos.m128_f32[1]));

	XMStoreFloat4x4(&SpitDesc.PresetMatrix, XMMatrixTranslation(TailBoneMat.r[3].m128_f32[0], TailBoneMat.r[3].m128_f32[1], TailBoneMat.r[3].m128_f32[2]));

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_Levi_Bullet"),
		ENUM_CLASS(LEVEL::BOSS), TEXT("Layer_MonBullet"), &SpitDesc)))
		return E_FAIL;
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
	Safe_Release(m_pColliderGroundAttackCom);
}
