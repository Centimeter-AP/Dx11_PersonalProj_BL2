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
	m_pTransformCom->Set_State(Engine::STATE::POSITION, m_pNavigationCom->SetUp_Height(m_pTransformCom->Get_State(Engine::STATE::POSITION), 0.1f));

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

	auto SpineBoneMat = XMLoadFloat4x4(m_pModelCom->Get_CombinedTransformationMatrix(m_iSpineBoneIdx));
	auto HeadBoneMat = XMLoadFloat4x4(m_pModelCom->Get_CombinedTransformationMatrix(m_iHeadBoneIdx));
	m_pColliderCom->Update(SpineBoneMat * m_pTransformCom->Get_WorldMatrix());
	m_pColliderHeadCom->Update(HeadBoneMat * m_pTransformCom->Get_WorldMatrix());
	return __super::Update(fTimeDelta);
}

void CSpiderAnt::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CSpiderAnt::Render()
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
	m_pColliderHeadCom->Set_ColliderColor(RGBA_GREEN);
	m_pColliderCom->Render();
	m_pColliderHeadCom->Render();

	if (m_pNavigationCom != nullptr)
		m_pNavigationCom->Render();

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


	m_pCurState = m_pStates[SPIDERANT_STATE::STATE_Idle];
	return S_OK;
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
	_vector targetPos = m_pTarget->Get_Transform()->Get_State(STATE::POSITION);
	XMStoreFloat3(&SpitDesc.vTargetPos, XMVectorSetY(targetPos, targetPos.m128_f32[1] + 1.f));

	XMStoreFloat4x4(&SpitDesc.PresetMatrix, XMMatrixTranslation(TailBoneMat.r[3].m128_f32[0], TailBoneMat.r[3].m128_f32[1], TailBoneMat.r[3].m128_f32[2]));


	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SpiderAntSpit"),
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_MonBullet"), &SpitDesc)))
		return E_FAIL;
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
