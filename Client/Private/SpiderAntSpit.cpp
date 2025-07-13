#include "SpiderAntSpit.h"
#include "GameInstance.h"
#include "WebBallParticle.h"

CSpiderAntSpit::CSpiderAntSpit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet{pDevice, pContext}
{
}

CSpiderAntSpit::CSpiderAntSpit(const CSpiderAntSpit& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CSpiderAntSpit::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpiderAntSpit::Initialize(void* pArg)
{
	if (FAILED(CGameObject::Initialize(pArg)))
		return E_FAIL;

	DESC* pDesc = static_cast<CSpiderAntSpit::DESC*>(pArg);

	m_vTargetPos = pDesc->vTargetPos;
	m_iDamage = pDesc->iDamage;


	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;
	if (FAILED(Ready_PartObjects(pArg)))
		return E_FAIL;



	m_pTransformCom->Scaling(0.5f, 0.5f, 0.5f);
	Launch_Projectile(m_vTargetPos, 50.f);
	return S_OK;
}

void CSpiderAntSpit::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Priority_Update(fTimeDelta);
	}
}

EVENT CSpiderAntSpit::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	if (m_isActive == false)
		return EVN_NONE;

	Update_Projectile(fTimeDelta);
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());


	__super::Update(fTimeDelta);

	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Update(fTimeDelta);
	}


	return EVN_NONE;
}

void CSpiderAntSpit::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
	m_pColliderCom->Set_ColliderColor(RGBA_CYAN);
	__super::Late_Update(fTimeDelta);
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Late_Update(fTimeDelta);
	}
}

HRESULT CSpiderAntSpit::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(MESH_DEFAULT)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CSpiderAntSpit::Ready_Components(void* pArg)
{
	CBounding_Sphere::SPHERE_DESC SphereDesc = {};
	SphereDesc.pOwner = this;
	SphereDesc.eType = COLLIDER::SPHERE;
	SphereDesc.iColliderGroup = ENUM_CLASS(COL_GROUP::MON_BULLET);
	SphereDesc.iColliderID = ENUM_CLASS(COL_ID::MONSTER_SPIDERANT_SPIT);
	SphereDesc.fRadius = 0.1f;
	SphereDesc.vCenter = _float3(0.f, 0.f, 0.f);

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_WebBall"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_WebBall"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"), // 총 전용 셰이더로 바꾸던지 패스를 바꾸던지
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpiderAntSpit::Ready_PartObjects(void* pArg)
{
	CWebBallParticle::DESC			ParticleDesc;
	ParticleDesc.pParentObject = this;
	ParticleDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix4x4Ptr();
	ParticleDesc.iLevelID = ENUM_CLASS(LEVEL::STATIC);
	ParticleDesc.bHasTransformPreset = true;
	XMStoreFloat4x4(&ParticleDesc.PresetMatrix, XMMatrixIdentity());
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC),
		TEXT("PartObject_Effect_Particle"), TEXT("Prototype_GameObject_WebBallParticle"), &ParticleDesc)))
		return E_FAIL;

	return S_OK;
}

// 이거 어떡하냐 
void CSpiderAntSpit::Launch_Projectile(const _float3& targetPos, _float fSpeed)
{
	// 1) 시작점 P0, 목표점 Pt
	_vector P0 = m_pTransformCom->Get_State(STATE::POSITION);
	_vector Pt = XMVectorSetW(XMLoadFloat3(&targetPos), 1.f);

	// 2) 수평 거리 Dh, 높이 차 Dy
	_vector	  D = XMVectorSubtract(Pt, P0);
	_float    gx = XMVectorGetX(D);
	_float    gy = XMVectorGetY(D);
	_float    gz = XMVectorGetZ(D);
	_float    Dh = sqrtf(gx * gx + gz * gz);
	_float    Dy = gy;
	_float    v = fSpeed;
	_float    g = -XMVectorGetY(m_vGravity); // e.g. 9.8

	// 3) 판별식 및 θ 계산 (고각/저각 중 하나 선택)
	_float v2 = v * v;
	_float under = v2 * v2 - g * (g * Dh * Dh + 2 * Dy * v2);
	if (under < 0) {
		// 사거리 부족
		m_bIsProjectile = false;
		m_bDead = true;
		return;
	}
	_float root = sqrtf(under);
	// 고각 θ1, 저각 θ2
	//_float theta = atanf((v2 + root) / (g * Dh));
	_float thetaH = atanf((v2 + root) / (g * Dh));  // 고각
	_float thetaL = atanf((v2 - root) / (g * Dh));  // 저각


	// 4) 초기 속도 벡터 v0
	_vector dirH = XMVector3Normalize(XMVectorSet(gx, 0.f, gz, 0.f));
	_vector v0_h = XMVectorScale(dirH, v * cosf(thetaL));
	_vector v0_v = XMVectorSet(0.f, v * sinf(thetaL), 0.f, 0.f);
	m_vVelocity = XMVectorAdd(v0_h, v0_v);

	// 5) 발사 플래그
	m_bIsProjectile = true;
}

void CSpiderAntSpit::Update_Projectile(_float fTimeDelta)
{
	if (!m_bIsProjectile)
		return;

	// 1) 속도에 중력 가속 누적: v = v + g * dt
	m_vVelocity = XMVectorAdd(m_vVelocity,
		XMVectorScale(m_vGravity, fTimeDelta));

	// 2) 위치 이동: P = P + v * dt
	_vector vPos = m_pTransformCom->Get_State(STATE::POSITION);
	vPos = XMVectorAdd(vPos,
		XMVectorScale(m_vVelocity, fTimeDelta));
	m_pTransformCom->Set_State(STATE::POSITION, vPos);

	m_fLifeTime += fTimeDelta;
	if (m_fLifeTime > 2.f)
		m_bDead = true;
}

CSpiderAntSpit* CSpiderAntSpit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSpiderAntSpit* pInstance = new CSpiderAntSpit(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSpiderAntSpit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpiderAntSpit::Clone(void* pArg)
{
	CSpiderAntSpit* pInstance = new CSpiderAntSpit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpiderAntSpit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpiderAntSpit::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
}
