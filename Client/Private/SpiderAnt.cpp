#include "SpiderAnt.h"
#include "SpiderAntSpit.h"
#include "SpiderAntState.h"
#include "GameInstance.h"

CSpiderAnt::CSpiderAnt(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster { pDevice, pContext }
{

}

CSpiderAnt::CSpiderAnt(const CSpiderAnt& Prototype)
	: CMonster( Prototype )
{

}

HRESULT CSpiderAnt::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpiderAnt::Initialize(void* pArg)
{
 	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Initialize_BasicStatus(rand() % 3 + 1);

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_SkagStates()))
		return E_FAIL;
	
	//m_pModelCom->Set_Animation(RAKK_ANIM::Flight_BankL, true);
	m_eCurState = STATE_Idle;
	Set_State(m_eCurState);

	m_fAttackableDistance = 3.f;
	m_fDetectiveDistance = 20.f;

	m_iSpineBoneIdx = m_pModelCom->Find_BoneIndex("Spine1");
	m_iHeadBoneIdx = m_pModelCom->Find_BoneIndex("Head");
	m_iTailBoneIdx = m_pModelCom->Find_BoneIndex("Tail6");

	m_pTransformCom->Set_State(STATE::POSITION, m_pNavigationCom->Get_CurCenterPoint());
	return S_OK;
}

void CSpiderAnt::Priority_Update(_float fTimeDelta)
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

EVENT CSpiderAnt::Update(_float fTimeDelta)
{
	//if (KEY_DOWN(DIK_1))
	//{
	//	m_bChargeCheck = true;
	//}
	//if (KEY_DOWN(DIK_2))
	//{
	//	m_bLeapCheck = true;
	//}

	if (KEY_DOWN(DIK_Z))
	{
		Spawn_SpitBullet();
	}
	if (KEY_DOWN(DIK_3))
	{
		Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Attack_Shot1);
	}
	if (KEY_DOWN(DIK_4))
	{
		Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Attack_Shot3);
	}
	if (KEY_DOWN(DIK_5))
	{
		Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Attack_Shot6);
	}
	auto SpineBoneMat = XMLoadFloat4x4(m_pModelCom->Get_CombinedTransformationMatrix(m_iSpineBoneIdx));
	auto HeadBoneMat = XMLoadFloat4x4(m_pModelCom->Get_CombinedTransformationMatrix(m_iHeadBoneIdx));
	m_pColliderCom->Update(SpineBoneMat * m_pTransformCom->Get_WorldMatrix());
	m_pColliderHeadCom->Update(HeadBoneMat * m_pTransformCom->Get_WorldMatrix());
	return __super::Update(fTimeDelta);
}

void CSpiderAnt::Late_Update(_float fTimeDelta)
{
	m_pColliderCom->Set_ColliderColor(RGBA_GREEN);
	m_pColliderHeadCom->Set_ColliderColor(RGBA_GREEN);

	__super::Late_Update(fTimeDelta);
}

HRESULT CSpiderAnt::Render()
{
	return __super::Render();
	if (FAILED(__super::Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CSpiderAnt::Ready_Components(void* pArg)
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(m_iLevelID, TEXT("Prototype_Component_Model_SpiderAnt"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::AABB_DESC AABBDesc = {};
	AABBDesc.pOwner = this;
	AABBDesc.eType = COLLIDER::AABB;
	AABBDesc.iColliderGroup = ENUM_CLASS(COL_GROUP::MONSTER);
	AABBDesc.iColliderID = ENUM_CLASS(COL_ID::MONSTER_SPIDERANT);
	AABBDesc.vExtents = _float3(35.f, 43.f, 35.f);
	AABBDesc.vCenter = _float3(0.f, -17.f, 0.f);


	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBDesc)))
		return E_FAIL;

	CBounding_Sphere::SPHERE_DESC SphereDesc = {};
	SphereDesc.pOwner = this;
	SphereDesc.eType = COLLIDER::AABB;
	SphereDesc.iColliderGroup = ENUM_CLASS(COL_GROUP::MONSTER);
	SphereDesc.iColliderID = ENUM_CLASS(COL_ID::MONSTER_SPIDERANT_HEAD);
	SphereDesc.fRadius = 20.f;
	SphereDesc.vCenter = _float3(0.f, 0.f, 0.f);

	/* For.Com_HeadCollider */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_ColliderHead"), reinterpret_cast<CComponent**>(&m_pColliderHeadCom), &SphereDesc)))
		return E_FAIL;

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

HRESULT CSpiderAnt::Ready_SkagStates()
{
	m_pStates[SPIDERANT_STATE::STATE_Idle] =  new CSpiderAntState_Idle(this);
	m_pStates[SPIDERANT_STATE::STATE_Death] = new CSpiderAntState_Dead(this);
	m_pStates[SPIDERANT_STATE::STATE_Patrol] = new CSpiderAntState_Patrol(this);
	m_pStates[SPIDERANT_STATE::STATE_Attack_Shot1] = new CSpiderAntState_Attack_Shot1(this);
	m_pStates[SPIDERANT_STATE::STATE_Attack_Shot3] = new CSpiderAntState_Attack_Shot3(this);
	m_pStates[SPIDERANT_STATE::STATE_Attack_Shot6] = new CSpiderAntState_Attack_Shot6(this);
	m_pStates[SPIDERANT_STATE::STATE_Attack_Claw] = new CSpiderAntState_Attack_Claw(this);
	m_pStates[SPIDERANT_STATE::STATE_Attack_Leap] = new CSpiderAntState_Attack_Leap(this);
	m_pStates[SPIDERANT_STATE::STATE_Attack_Charge] = new CSpiderAntState_Attack_Charge(this);
	m_pStates[SPIDERANT_STATE::STATE_Provoked_Idle] = new CSpiderAntState_Provoked_Idle(this);
	m_pStates[SPIDERANT_STATE::STATE_Run] = new CSpiderAntState_Run(this);
	m_pStates[SPIDERANT_STATE::STATE_PhaseLocked] = new CSpiderAntState_Phaselocked(this);


	m_pCurState = m_pStates[SPIDERANT_STATE::STATE_Idle];
	return S_OK;
}

void CSpiderAnt::Initialize_BasicStatus(_int iLevel)
{
	m_fBase_HP = 90.f;
	m_fBase_ATK = 13.f;
	m_iMaxHP = m_fBase_HP * powf(m_fGrowthRate, iLevel - 1);
	m_iHP = m_iMaxHP = m_pGameInstance->AddVariance(m_iMaxHP, 0.15f);

	m_iDamage = m_fBase_ATK * powf(m_fGrowthRate, iLevel - 1);
	m_iDamage = m_pGameInstance->AddVariance(m_iDamage, 0.15f);
	m_iDefense = 0.f;

	m_fAttackableDistance = 6.f;
	m_fDetectiveDistance = 80.f;

	m_fLeapCheckTimer = 10.f;
	m_fChargeCheckTimer = 10.f;
}

void CSpiderAnt::Set_State(SPIDERANT_STATE eState)
{
	m_ePrevState = m_eCurState;
	m_eCurState = eState;

	m_pCurState->Exit();
	m_pCurState = m_pStates[eState];
	m_pCurState->Enter();
}

void CSpiderAnt::Update_State(_float fTimeDelta)
{
	//if (m_ePrevState != m_eCurState)
	//{
	//	m_pCurState->Enter();
	//	m_ePrevState = m_eCurState;
	//}
	m_pCurState->Execute(fTimeDelta);
}

void CSpiderAnt::Set_State_Dead()
{
	Set_State(SPIDERANT_STATE::STATE_Death);
}

HRESULT CSpiderAnt::Spawn_SpitBullet()
{
	auto TailBoneMat = XMLoadFloat4x4(m_pModelCom->Get_CombinedTransformationMatrix(m_iTailBoneIdx)) * m_pTransformCom->Get_WorldMatrix();

	CSpiderAntSpit::DESC SpitDesc = {};
	SpitDesc.bHasTransformPreset = true;
	SpitDesc.iLevelID = ENUM_CLASS(LEVEL::GAMEPLAY);
	SpitDesc.fSpeedPerSec = 10.f;
	SpitDesc.iDamage = m_iDamage;
	_vector targetPos = m_pTarget->Get_Transform()->Get_State(STATE::POSITION);
	XMStoreFloat3(&SpitDesc.vTargetPos, XMVectorSetY(targetPos, targetPos.m128_f32[1]));

	XMStoreFloat4x4(&SpitDesc.PresetMatrix, XMMatrixTranslation(TailBoneMat.r[3].m128_f32[0], TailBoneMat.r[3].m128_f32[1], TailBoneMat.r[3].m128_f32[2]));


	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SpiderAntSpit"),
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_MonBullet"), &SpitDesc)))
		return E_FAIL;
}

void CSpiderAnt::On_Collision(_uint iMyColID, _uint iHitColID, CCollider* pHitCol)
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

void CSpiderAnt::OnHit_Phaselock()
{
	Set_State(SPIDERANT_STATE::STATE_PhaseLocked);
}


void CSpiderAnt::AttackTimer(_float fTimeDelta)
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

CSpiderAnt* CSpiderAnt::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSpiderAnt* pInstance = new CSpiderAnt(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpiderAnt::Clone(void* pArg)
{
	CSpiderAnt* pInstance = new CSpiderAnt(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpiderAnt");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpiderAnt::Free()
{
	__super::Free();
	for (auto& State : m_pStates)
		Safe_Release(State);
}
