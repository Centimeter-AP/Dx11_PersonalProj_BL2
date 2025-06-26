#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

CBounding_AABB::CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBounding { pDevice, pContext }
{
}

HRESULT CBounding_AABB::Initialize(const CBounding::BOUNDING_DESC* pDesc)
{
	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	const AABB_DESC* pAABBDesc = static_cast<const AABB_DESC*>(pDesc);

	m_pOriginalDesc = new BoundingBox(pAABBDesc->vCenter, pAABBDesc->vExtents);
	m_pDesc = new BoundingBox(*m_pOriginalDesc);

	return S_OK;
}

void CBounding_AABB::Update(_fmatrix WorldMatrix)
{
	_matrix			TransformMatrix = WorldMatrix;

 	TransformMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(TransformMatrix.r[0]);
	TransformMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(TransformMatrix.r[1]);
	TransformMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(TransformMatrix.r[2]);

	m_pOriginalDesc->Transform(*m_pDesc, TransformMatrix);
}

_bool CBounding_AABB::Intersect(CBounding* pTarget)
{
	_bool		isColl = { false };

	switch (pTarget->Get_Type())
	{
	case COLLIDER::AABB:
		// isColl = m_pDesc->Intersects(*static_cast<CBounding_AABB*>(pTarget)->Get_Desc());
		isColl = Intersect_ToAABB(pTarget);
		break;
	case COLLIDER::OBB:
		isColl = m_pDesc->Intersects(*static_cast<CBounding_OBB*>(pTarget)->Get_Desc());
		break;
	case COLLIDER::SPHERE:
		isColl = m_pDesc->Intersects(*static_cast<CBounding_Sphere*>(pTarget)->Get_Desc());
		break;
	}

	return isColl;
}

_bool CBounding_AABB::Raycast(_fvector vRayOrigin, _fvector vRayDir, _float& fRayDist)
{
	return m_pDesc->Intersects(vRayOrigin, vRayDir, fRayDist); 
}


#ifdef _DEBUG

HRESULT CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	
	DX::Draw(pBatch, *m_pDesc, vColor);

	return S_OK;
}

#endif

_bool CBounding_AABB::Intersect_ToAABB(CBounding* pTarget)
{
	_float3		vSourMin = Compute_Min();
	_float3		vSourMax = Compute_Max();

	_float3		vDestMin = static_cast<CBounding_AABB*>(pTarget)->Compute_Min();
	_float3		vDestMax = static_cast<CBounding_AABB*>(pTarget)->Compute_Max();

	/* 너비상으로 겹쳤냐? */
	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
		return false;

	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
		return false;

	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
		return false;

	// 충돌 O → 침투 벡터 계산
	_float3 push = Get_Penetration(*static_cast<CBounding_AABB*>(pTarget));

	// 양쪽에 침투 벡터 저장 (한 쪽은 반대로)
	m_vPenetratedVector = push;
	static_cast<CBounding_AABB*>(pTarget)->m_vPenetratedVector = { -push.x, -push.y, -push.z };

	return true;
}

_float3 CBounding_AABB::Compute_Min()
{
	return _float3(m_pDesc->Center.x - m_pDesc->Extents.x, 
		m_pDesc->Center.y - m_pDesc->Extents.y,
		m_pDesc->Center.z - m_pDesc->Extents.z);
}

_float3 CBounding_AABB::Compute_Max()
{
	return _float3(m_pDesc->Center.x + m_pDesc->Extents.x,
		m_pDesc->Center.y + m_pDesc->Extents.y,
		m_pDesc->Center.z + m_pDesc->Extents.z);
}

_float3 CBounding_AABB::Get_Penetration(CBounding_AABB& dest)
{
	// 1) 좌표계산
	_float3 aMin = Compute_Min();
	_float3 aMax = Compute_Max();
	_float3 bMin = dest.Compute_Min();
	_float3 bMax = dest.Compute_Max();

	// 2) 축별 Overlap
	_float ox = min(aMax.x, bMax.x) - max(aMin.x, bMin.x);
	_float oy = min(aMax.y, bMax.y) - max(aMin.y, bMin.y);
	_float oz = min(aMax.z, bMax.z) - max(aMin.z, bMin.z);

	// 3) 겹치지 않으면 (0,0,0) 반환
	if (ox <= 0 || oy <= 0 || oz <= 0)
		return { 0,0,0 };

	// 4) 가장 작은 축으로 분리
	if (ox < oy && ox < oz)
		return { ox * ((aMin.x < bMin.x) ? -1.f : +1.f), 0.f, 0.f };
	else if (oy < oz)
		return { 0.f, oy * ((aMin.y < bMin.y) ? -1.f : +1.f), 0.f };
	else
		return { 0.f, 0.f, oz * ((aMin.z < bMin.z) ? -1.f : +1.f) };
}

_float3 CBounding_AABB::ComputeCollisionNormal(CBounding_AABB* pDest)
{
	_float3 aMin = Compute_Min();
	_float3 aMax = Compute_Max();
	_float3 bMin = pDest->Compute_Min();
	_float3 bMax = pDest->Compute_Max();

	// 각 축별 침투량 계산
	float dx = min(aMax.x, bMax.x) - max(aMin.x, bMin.x); 
	float dy = min(aMax.y, bMax.y) - max(aMin.y, bMin.y);
	float dz = min(aMax.z, bMax.z) - max(aMin.z, bMin.z);
	
	// 가장 침투가 작은 축을 충돌 축으로 간주
	if (dx < dy && dx < dz)
		return _float3((m_pDesc->Center.x < pDest->Get_Desc()->Center.x) ? -1.f : 1.f, 0, 0);
	else if (dy < dz)
		return _float3(0, (m_pDesc->Center.y < pDest->Get_Desc()->Center.y) ? -1.f : 1.f, 0);
	else
		return _float3(0, 0, (m_pDesc->Center.z < pDest->Get_Desc()->Center.z) ? -1.f : 1.f);
}


CBounding_AABB* CBounding_AABB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::BOUNDING_DESC* pDesc)
{
	CBounding_AABB* pInstance = new CBounding_AABB(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pDesc)))
	{
		MSG_BOX("Failed to Created : CBounding_AABB");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CBounding_AABB::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginalDesc);
	Safe_Delete(m_pDesc);
}
