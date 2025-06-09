#include "Collider_Manager.h"
#include "Collider.h"
#include "GameObject.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"


CCollider_Manager::CCollider_Manager()
{

}

HRESULT CCollider_Manager::Initialize(_uint iNumGroups)
{
	m_pColliders = new 	list<class CCollider*>[iNumGroups];
	m_iNumGroups = iNumGroups;

	return S_OK;
}

HRESULT CCollider_Manager::Add_Collider(_uint iGroupID, CCollider* pCollider)
{
	if (iGroupID >= m_iNumGroups)
		return E_FAIL;

	m_pColliders[iGroupID].push_back(pCollider);


	return S_OK;
}

void CCollider_Manager::Intersect(CCollider* pDst, CCollider* pSrc)
{
	if (true == pDst->Intersect(pSrc))
	{
		//pSrc->Get_Owner()->On_Collision(pDst->Get_ColliderID());
		//pDst->Get_Owner()->On_Collision(pSrc->Get_ColliderID());
	}
}

void CCollider_Manager::Intersect_Group(_uint iSrcGroupID, _uint iDstGroupID)
{
	for (auto& pSrcCollider : m_pColliders[iSrcGroupID])
	{
		for (auto& pDstCollider : m_pColliders[iDstGroupID])
		{
			CCollider_Manager::Intersect(pDstCollider, pSrcCollider);
			if (true == pSrcCollider->Intersect(pDstCollider))
			{
				pSrcCollider->Get_Owner()->On_Collision(pDstCollider->Get_ColliderID());
				pDstCollider->Get_Owner()->On_Collision(pSrcCollider->Get_ColliderID());
			}
		}
	}
}

CCollider* CCollider_Manager::Raycast(_fvector vRayOrigin, _fvector vRayDir, _float fRayLength, _uint iColliderGroupID, _float& fRayDist)
{
	CCollider* pCurCollider{ nullptr };
	_float3 vCurPos{}, vCurNormal{};
	_float fCurLength{};

	_float fLastDistance = {};

	for (auto& pCollider : m_pColliders[iColliderGroupID])
	{
		// 범위 밖에 있음
		auto vColPos = pCollider->Get_Pos();
		auto Length = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vColPos) - vRayOrigin));
		if (Length > fRayLength + pCollider->Get_MaxLength())
			continue;

		_float fCurDistance = {};
		if (pCollider->Raycast(vRayOrigin, vRayDir, fCurDistance))
		{
			if (fCurDistance < fLastDistance || fLastDistance == 0.f) 
			{
				fLastDistance = fCurDistance;
				pCurCollider = pCollider;
			}

			//fCurLength = (CCollider::m_vLast_Collision_Pos - vRayOrigin).Length();
			//if (fCurLength < fResult)
			//{
			//	fResult = fCurLength;
			//	vCurNormal = CCollider::m_vLast_Collision_Depth;
			//	vCurPos = CCollider::m_vLast_Collision_Pos;
			//	pCurCollider = pCollider;
			//	ColliderID = pCollider->Get_ID();
			//}
			//else
			//{
			//	CCollider::m_vLast_Collision_Depth = vCurNormal;
			//	CCollider::m_vLast_Collision_Pos = vCurPos;
			//}
		}
	}

	return pCurCollider;
}

CCollider_Manager* CCollider_Manager::Create(_uint iNumGroups)
{
	CCollider_Manager* pInstance = new CCollider_Manager();

	if (FAILED(pInstance->Initialize(iNumGroups)))
	{
		MSG_BOX("Failed to Created : CCollider_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CCollider_Manager::Free()
{
	__super::Free();
	for (_uint i = 0; i < m_iNumGroups; ++i)
	{
		for (auto Collider : m_pColliders[i])
			Safe_Release(Collider);
		m_pColliders[i].clear();
	}
	Safe_Delete_Array(m_pColliders);
}
