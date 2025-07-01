#include "Levi_Bullet.h"
#include "GameInstance.h"

CLevi_Bullet::CLevi_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet{pDevice, pContext}
{
}

CLevi_Bullet::CLevi_Bullet(const CLevi_Bullet& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CLevi_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLevi_Bullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;


	Launch_Projectile(m_vTargetPos, 70.f);
	return S_OK;
}

void CLevi_Bullet::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
}

EVENT CLevi_Bullet::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	if (m_isActive == false)
		return EVN_NONE;

	Update_Projectile(fTimeDelta);
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	__super::Update(fTimeDelta);

	return EVN_NONE;
}

void CLevi_Bullet::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
	m_pColliderCom->Set_ColliderColor(RGBA_CYAN);
	__super::Late_Update(fTimeDelta);
}

HRESULT CLevi_Bullet::Render()
{
	//__super::Render(); �θ� �� ��� ��� �ּ�..


	return S_OK;
}

HRESULT CLevi_Bullet::Ready_Components(void* pArg)
{
	CBounding_Sphere::SPHERE_DESC SphereDesc = {};
	SphereDesc.pOwner = this;
	SphereDesc.eType = COLLIDER::SPHERE;
	SphereDesc.iColliderGroup = ENUM_CLASS(COL_GROUP::MON_BULLET);
	SphereDesc.iColliderID = ENUM_CLASS(COL_ID::MONSTER_BOSS_LEVIATHAN_ATK);
	SphereDesc.fRadius = 8.f;
	SphereDesc.vCenter = _float3(0.f, 0.f, 0.f);

	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

void CLevi_Bullet::Launch_Projectile(const _float3& targetPos, _float fSpeed)
{
	// 1) ������ P0, ��ǥ�� Pt
	_vector P0 = m_pTransformCom->Get_State(STATE::POSITION);
	_vector Pt = XMVectorSetW(XMLoadFloat3(&targetPos), 1.f);

	// 2) ���� �Ÿ� Dh, ���� �� Dy
	_vector	  D = XMVectorSubtract(Pt, P0);
	_float    gx = XMVectorGetX(D);
	_float    gy = XMVectorGetY(D);
	_float    gz = XMVectorGetZ(D);
	_float    Dh = sqrtf(gx * gx + gz * gz);
	_float    Dy = gy;
	_float    v = fSpeed;
	_float    g = -XMVectorGetY(m_vGravity); // e.g. 9.8

	// 3) �Ǻ��� �� �� ��� (��/���� �� �ϳ� ����)
	_float v2 = v * v;
	_float under = v2 * v2 - g * (g * Dh * Dh + 2 * Dy * v2);
	if (under < 0) {
		// ��Ÿ� ����
		//m_bIsProjectile = false;
		//m_bDead = true;
		//return;
		under = 0.1f;
	}
	_float root = sqrtf(under);
	// �� ��1, ���� ��2
	//_float theta = atanf((v2 + root) / (g * Dh));
	_float thetaH = atanf((v2 + root) / (g * Dh));  // ��
	_float thetaL = atanf((v2 - root) / (g * Dh));  // ����


	// 4) �ʱ� �ӵ� ���� v0
	_vector dirH = XMVector3Normalize(XMVectorSet(gx, 0.f, gz, 0.f));
	_vector v0_h = XMVectorScale(dirH, v * cosf(thetaL));
	_vector v0_v = XMVectorSet(0.f, v * sinf(thetaL), 0.f, 0.f);
	m_vVelocity = XMVectorAdd(v0_h, v0_v);

	// 5) �߻� �÷���
	m_bIsProjectile = true;
}

void CLevi_Bullet::Update_Projectile(_float fTimeDelta)
{
	if (!m_bIsProjectile)
		return;

	// 1) �ӵ��� �߷� ���� ����: v = v + g * dt
	m_vVelocity = XMVectorAdd(m_vVelocity,
		XMVectorScale(m_vGravity, fTimeDelta));

	// 2) ��ġ �̵�: P = P + v * dt
	_vector vPos = m_pTransformCom->Get_State(STATE::POSITION);
	vPos = XMVectorAdd(vPos,
		XMVectorScale(m_vVelocity, fTimeDelta));
	m_pTransformCom->Set_State(STATE::POSITION, vPos);

	m_fLifeTime += fTimeDelta;
	if (m_fLifeTime > 2.f)
		m_bDead = true;
}

CLevi_Bullet* CLevi_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevi_Bullet* pInstance = new CLevi_Bullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLevi_Bullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLevi_Bullet::Clone(void* pArg)
{
	CLevi_Bullet* pInstance = new CLevi_Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLevi_Bullet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevi_Bullet::Free()
{
	__super::Free();
}
