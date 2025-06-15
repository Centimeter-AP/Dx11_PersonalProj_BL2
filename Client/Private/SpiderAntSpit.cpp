#include "SpiderAntSpit.h"
#include "GameInstance.h"

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
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

void CSpiderAntSpit::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
}

EVENT CSpiderAntSpit::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	if (m_isActive == false)
		return EVN_NONE;

	__super::Update(fTimeDelta);

	return EVN_NONE;
}

void CSpiderAntSpit::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
	__super::Late_Update(fTimeDelta);
}

HRESULT CSpiderAntSpit::Render()
{
	__super::Render();
	return S_OK;
}

HRESULT CSpiderAntSpit::Ready_Components(void* pArg)
{
	CBounding_Sphere::SPHERE_DESC SphereDesc = {};
	SphereDesc.pOwner = this;
	SphereDesc.eType = COLLIDER::SPHERE;
	SphereDesc.iColliderGroup = ENUM_CLASS(COL_GROUP::MON_BULLET);
	SphereDesc.iColliderID = ENUM_CLASS(COL_ID::MONSTER_SPIDERANT_SPIT);
	SphereDesc.fRadius = 10.f;
	SphereDesc.vCenter = _float3(0.f, 0.f, 0.f);

	/* For.Com_HeadCollider */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;
	return S_OK;
}

CSpiderAntSpit* CSpiderAntSpit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CSpiderAntSpit::Clone(void* pArg)
{
	return nullptr;
}

void CSpiderAntSpit::Free()
{
}
