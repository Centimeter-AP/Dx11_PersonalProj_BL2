#include "Skag.h"
#include "SkagState.h"
#include "GameInstance.h"

CSkag::CSkag(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster { pDevice, pContext }
{

}

CSkag::CSkag(const CSkag& Prototype)
	: CMonster( Prototype )
{

}

HRESULT CSkag::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkag::Initialize(void* pArg)
{
 	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Initialize_BasicStatus(rand()%3 + 1);

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_SkagStates()))
		return E_FAIL;
	
	m_eCurState = STATE_Idle;
	Set_State(m_eCurState);



	m_iSpineBoneIdx = m_pModelCom->Find_BoneIndex("Spine1");
	m_iHeadBoneIdx = m_pModelCom->Find_BoneIndex("Head");

	m_pTransformCom->Set_State(STATE::POSITION, m_pNavigationCom->Get_CurCenterPoint());
	return S_OK;
}

void CSkag::Priority_Update(_float fTimeDelta)
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
	if (m_pGravityCom->Is_Grounded())
		m_pTransformCom->Set_State(Engine::STATE::POSITION, m_pNavigationCom->SetUp_Height(m_pTransformCom->Get_State(Engine::STATE::POSITION), 0.1f));
	if (m_eCurState != STATE_PhaseLocked)
		m_pGravityCom->Update(fTimeDelta);
}

EVENT CSkag::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_1))
	{
		m_bChargeCheck = true;
	}
	if (KEY_DOWN(DIK_2))
	{
		m_bLeapCheck = true;
	}

	auto SpineBoneMat = XMLoadFloat4x4(m_pModelCom->Get_CombinedTransformationMatrix(m_iSpineBoneIdx));
	auto HeadBoneMat = XMLoadFloat4x4(m_pModelCom->Get_CombinedTransformationMatrix(m_iHeadBoneIdx));
	m_pColliderCom->Update(SpineBoneMat * m_pTransformCom->Get_WorldMatrix());
	m_pColliderHeadCom->Update(HeadBoneMat * m_pTransformCom->Get_WorldMatrix());
	m_pColliderGroundAttackCom->Update(HeadBoneMat * m_pTransformCom->Get_WorldMatrix());
	return __super::Update(fTimeDelta);
}

void CSkag::Late_Update(_float fTimeDelta)
{
	m_pColliderCom->Set_ColliderColor(RGBA_GREEN);
	m_pColliderHeadCom->Set_ColliderColor(RGBA_GREEN);
	m_pColliderGroundAttackCom->Set_ColliderColor(RGBA_GREEN);
	m_pGameInstance->Add_DebugComponent(m_pColliderGroundAttackCom);

	__super::Late_Update(fTimeDelta);
}

HRESULT CSkag::Render()
{
	return __super::Render();
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;


	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			continue;

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS, 0)))
			continue;
		m_pModelCom->Bind_Bone_Matrices(m_pShaderCom, "g_BoneMatrices", i);
		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CSkag::Ready_Components(void* pArg)
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(m_iLevelID, TEXT("Prototype_Component_Model_Skag"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::AABB_DESC AABBDesc = {};
	AABBDesc.pOwner = this;
	AABBDesc.eType = COLLIDER::AABB;
	AABBDesc.iColliderGroup = ENUM_CLASS(COL_GROUP::MONSTER);
	AABBDesc.iColliderID = ENUM_CLASS(COL_ID::MONSTER_SKAG);
	AABBDesc.vExtents = _float3(35.f, 43.f, 35.f);
	AABBDesc.vCenter = _float3(0.f, -17.f, 0.f);
	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBDesc)))
		return E_FAIL;

	CBounding_Sphere::SPHERE_DESC SphereDesc = {};
	SphereDesc.pOwner = this;
	SphereDesc.eType = COLLIDER::SPHERE;
	SphereDesc.iColliderGroup = ENUM_CLASS(COL_GROUP::MONSTER);
	SphereDesc.iColliderID = ENUM_CLASS(COL_ID::MONSTER_SKAG_HEAD);
	SphereDesc.fRadius = 20.f;
	SphereDesc.vCenter = _float3(0.f, 0.f, 0.f);
	/* For.Com_ColliderHead */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_ColliderHead"), reinterpret_cast<CComponent**>(&m_pColliderHeadCom), &SphereDesc)))
		return E_FAIL;

	SphereDesc.iColliderGroup = ENUM_CLASS(COL_GROUP::MON_ATTACK);
	SphereDesc.iColliderID = ENUM_CLASS(COL_ID::MONSTER_SKAG_ATK);
	SphereDesc.fRadius = 100.f;
	SphereDesc.vCenter = _float3(70.f, 50.f, 0.f);
	/* For.Com_ColliderAttack */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_ColliderAttack"), reinterpret_cast<CComponent**>(&m_pColliderGroundAttackCom), &SphereDesc)))
		return E_FAIL;
	m_pColliderGroundAttackCom->Set_Active(false);

	/* For.Com_Navigation */
	CNavigation::NAVIGATION_DESC		NaviDesc{};
	NaviDesc.iIndex = m_iNavIndex;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;

	/* For.Com_Gravity */
	CGravity::DESC	GravityDesc{};
	GravityDesc.fGravity = -40.f;
	GravityDesc.fJumpPower = 25.f;
	GravityDesc.pOwnerNavigationCom = m_pNavigationCom;
	GravityDesc.pOwnerTransformCom = m_pTransformCom;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Gravity"),
		TEXT("Com_Gravity"), reinterpret_cast<CComponent**>(&m_pGravityCom), &GravityDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkag::Ready_SkagStates()
{
	m_pStates[SKAG_STATE::STATE_Idle] =  new CSkagState_Idle(this);
	m_pStates[SKAG_STATE::STATE_Death] = new CSkagState_Dead(this);
	m_pStates[SKAG_STATE::STATE_Patrol] = new CSkagState_Patrol(this);
	m_pStates[SKAG_STATE::STATE_Attack_Bite] = new CSkagState_Attack_Bite(this);
	m_pStates[SKAG_STATE::STATE_Attack_Claw] = new CSkagState_Attack_Claw(this);
	m_pStates[SKAG_STATE::STATE_Attack_Tongue] = new CSkagState_Attack_Tongue(this);
	m_pStates[SKAG_STATE::STATE_Attack_Leap] = new CSkagState_Attack_Leap(this);
	m_pStates[SKAG_STATE::STATE_Attack_Charge] = new CSkagState_Attack_Charge(this);
	m_pStates[SKAG_STATE::STATE_Attack_Run_Bite] = new CSkagState_Attack_RunBite(this);
	m_pStates[SKAG_STATE::STATE_Attack_Run_Tongue] = new CSkagState_Attack_RunTongue(this);
	m_pStates[SKAG_STATE::STATE_Provoked] = new CSkagState_Provoked(this);
	m_pStates[SKAG_STATE::STATE_Provoked_Idle] = new CSkagState_Provoked_Idle(this);
	m_pStates[SKAG_STATE::STATE_PhaseLocked] = new CSkagState_Phaselocked(this);
	m_pStates[SKAG_STATE::STATE_Run] = new CSkagState_Run(this);
	m_pStates[SKAG_STATE::STATE_Turn180] = new CSkagState_Turn180(this);
	m_pCurState = m_pStates[SKAG_STATE::STATE_Idle];
	return S_OK;
}


void CSkag::Initialize_BasicStatus(_int iLevel)
{
	m_fBase_HP = 100.f;
	m_fBase_ATK = 15.f;
	m_iMaxHP = m_fBase_HP * powf(m_fGrowthRate, iLevel - 1);
	m_iHP = m_iMaxHP = m_pGameInstance->AddVariance(m_iMaxHP, 0.15f);

	m_iDamage = m_fBase_ATK * powf(m_fGrowthRate, iLevel - 1);
	m_iDamage = m_pGameInstance->AddVariance(m_iDamage, 0.15f);
	m_iDefense = 0.f;

	m_fAttackableDistance = 6.f;
	m_fDetectiveDistance = 40.f;

	m_fLeapCheckTimer = 10.f;
	m_fChargeCheckTimer = 10.f;
}

void CSkag::Set_State(SKAG_STATE eState)
{
	m_ePrevState = m_eCurState;
	m_eCurState = eState;

	m_pCurState->Exit();
	m_pCurState = m_pStates[eState];
	m_pCurState->Enter();
}

void CSkag::Update_State(_float fTimeDelta)
{
	//if (m_ePrevState != m_eCurState)
	//{
	//	m_pCurState->Enter();
	//	m_ePrevState = m_eCurState;
	//}
	m_pCurState->Execute(fTimeDelta);
}

void CSkag::Set_State_Dead()
{
	Set_State(SKAG_STATE::STATE_Death);
}

void CSkag::On_Collision(_uint iMyColID, _uint iHitColID, CCollider* pHitCol)
{
	__super::On_Collision(iMyColID, iHitColID, pHitCol);

	switch (iHitColID)
	{
	case ENUM_CLASS(COL_ID::PLAYER_SKILL_PHASELOCK):
		break;
	default:
		break;
	}
}

void CSkag::OnHit_Phaselock()
{
	Set_State(SKAG_STATE::STATE_PhaseLocked);

}


void CSkag::AttackTimer(_float fTimeDelta)
{
	m_fChargeCheckTimer += fTimeDelta;
	if (m_fChargeCheckTimer > 6.f)
	{
		if (rand() % 2 == 0)
			m_bChargeCheck = true;
		else
			m_fChargeCheckTimer = 0.f;
	}
	m_fLeapCheckTimer += fTimeDelta;
	if (m_fLeapCheckTimer > 5.f)
	{
		if (rand() % 2 == 0)
			m_bLeapCheck = true;
		else
			m_fLeapCheckTimer = 0.f;
	}
}

CSkag* CSkag::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkag* pInstance = new CSkag(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkag::Clone(void* pArg)
{
	CSkag* pInstance = new CSkag(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSkag");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkag::Free()
{
	__super::Free();
	for (auto& State : m_pStates)
		Safe_Release(State);
	Safe_Release(m_pColliderGroundAttackCom);
}
