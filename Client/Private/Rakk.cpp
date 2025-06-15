#include "Rakk.h"
#include "RakkState.h"
#include "GameInstance.h"

CRakk::CRakk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster { pDevice, pContext }
{

}

CRakk::CRakk(const CRakk& Prototype)
	: CMonster( Prototype )
{

}

HRESULT CRakk::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRakk::Initialize(void* pArg)
{
 	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	XMStoreFloat3(&m_vRegionCenter, m_pTransformCom->Get_State(STATE::POSITION));

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_RakkStates()))
		return E_FAIL;
	
	//m_pModelCom->Set_Animation(RAKK_ANIM::Flight_BankL, true);
	m_eCurState = STATE_Flying_Idle;
	Set_State(m_eCurState);

	m_fAttackableDistance = 5.f;
	m_fDetectiveDistance = 20.f;


	return S_OK;
}

void CRakk::Priority_Update(_float fTimeDelta)
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

EVENT CRakk::Update(_float fTimeDelta)
{
	if (m_bDead)
		return Death_Delay(fTimeDelta);

	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
	
	return EVN_NONE;
}

void CRakk::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CRakk::Render()
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

	_wstring AnimationFrame = L"TEST";
	//m_pGameInstance->Draw_Font(TEXT("Font_WillowBody"), AnimationFrame.c_str(), _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.5f);


	return S_OK;
}

HRESULT CRakk::Ready_Components(void* pArg)
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

HRESULT CRakk::Ready_RakkStates()
{
	m_pStates[RAKK_STATE::STATE_Idle] = new CRakkState_Idle(this);
	m_pStates[RAKK_STATE::STATE_Flying_Idle] = new CRakkState_Flying_Idle(this);
	m_pStates[RAKK_STATE::STATE_Hover] = new CRakkState_Hover(this);
	m_pStates[RAKK_STATE::STATE_Ground_Idle] = new CRakkState_Ground_Idle(this);
	m_pStates[RAKK_STATE::STATE_Flying_Attack] = new CRakkState_Attack(this);
	m_pStates[RAKK_STATE::STATE_Flying_SuicidalDive] = new CRakkState_SuicideDive(this);
	m_pStates[RAKK_STATE::STATE_PhaseLocked] = new CRakkState_Phaselocked(this);
	m_pStates[RAKK_STATE::STATE_Takeoff] = new CRakkState_Takeoff(this);
	m_pStates[RAKK_STATE::STATE_Landing] = new CRakkState_Landing(this);
	m_pStates[RAKK_STATE::STATE_Provoked] = new CRakkState_Provoked(this);
	m_pStates[RAKK_STATE::STATE_Dead] = new CRakkState_Dead(this);

	m_pCurState = m_pStates[RAKK_STATE::STATE_Flying_Idle];
	return S_OK;
}

void CRakk::Set_State(RAKK_STATE eState)
{
	m_ePrevState = m_eCurState;
	m_eCurState = eState;

	m_pCurState->Exit();
	m_pCurState = m_pStates[eState];
	m_pCurState->Enter();
}

void CRakk::Update_State(_float fTimeDelta)
{
	//if (m_ePrevState != m_eCurState)
	//{
	//	m_pCurState->Enter();
	//	m_ePrevState = m_eCurState;
	//}
	m_pCurState->Execute(fTimeDelta);
}

void CRakk::Set_State_Dead()
{
	Set_State(RAKK_STATE::STATE_Dead);
}

_bool CRakk::IsOutsideRegion() const
{
	_vector vPos = m_pTransformCom->Get_State(STATE::POSITION);
	_vector vCenter = XMLoadFloat3(&m_vRegionCenter);
	_vector vDiff = vPos - vCenter;

	_float distSq = XMVectorGetX(XMVector3LengthSq(vDiff));
	return (distSq > (m_fRegionRadius * m_fRegionRadius));
}

void CRakk::Enforce_ReturnToRegion(_float fTimeDelta)
{
	_vector vPos = m_pTransformCom->Get_State(STATE::POSITION);
	_vector vCenter = XMLoadFloat3(&m_vRegionCenter);
	_vector dirToCenter = XMVectorSubtract(vCenter, vPos);

	_float distToCenter = XMVectorGetX(XMVector3Length(dirToCenter));
	if (distToCenter < 0.01f)
	{
		m_isOutsideRegion = false;
		return;
	}

	// 3) 단위벡터로 정규화
	XMVECTOR normDir = XMVector3Normalize(dirToCenter);

	// 4) m_dir을 즉시 이 방향으로 변경 → 구역 밖에서 돌아올 때 쓰는 로직
	//XMStoreFloat3(&m_dir, normDir);

	// 5) 이 상태에서도 이동 속도는 그대로 적용 → Update()에서 MoveToward(…) 대신
	//    m_dir * (m_fSpeed * fTimeDelta) 로 이동하면 된다.
	//    여기서는 단순히 m_dir을 변경만 하고, Update()에서 동일하게 MoveToward 역할을 수행하게 한다.
}


CRakk* CRakk::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRakk* pInstance = new CRakk(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRakk::Clone(void* pArg)
{
	CRakk* pInstance = new CRakk(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CRakk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRakk::Free()
{
	__super::Free();
	for (auto& State : m_pStates)
		Safe_Release(State);
}
