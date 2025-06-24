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
	m_pColliders = new list<class CCollider*>[iNumGroups];
	m_iNumGroups = iNumGroups;

	return S_OK;
}

HRESULT CCollider_Manager::Add_Collider(_uint iGroupID, CCollider* pCollider)
{
	if (iGroupID >= m_iNumGroups)
		return E_FAIL;

	Safe_AddRef(pCollider);
	m_pColliders[iGroupID].push_back(pCollider);


	return S_OK;
}

void CCollider_Manager::Delete_Collider(const CGameObject* pOwner)
{
	for (_uint i = 0; i < m_iNumGroups; ++i)
	{
		for (auto Iter = m_pColliders[i].begin();
			Iter != m_pColliders[i].end();)
		{
			if ((*Iter)->Get_Owner() == pOwner)
			{
				Safe_Release(*Iter);
				Iter = m_pColliders[i].erase(Iter);
				continue;
			}
			Iter++;
		}
	}
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
			if (pSrcCollider != pDstCollider && true == pSrcCollider->Intersect(pDstCollider))
			{
				pSrcCollider->Get_Owner()->On_Collision(pSrcCollider->Get_ColliderID(), pDstCollider->Get_ColliderID(), pDstCollider);
				pDstCollider->Get_Owner()->On_Collision(pDstCollider->Get_ColliderID(), pSrcCollider->Get_ColliderID(), pSrcCollider);
				if (pSrcCollider->Get_Type() == COLLIDER::AABB && pDstCollider->Get_Type() == COLLIDER::AABB)
				{
					_float3 vSrcPen = pSrcCollider->Get_Penetrated();
					pSrcCollider->Get_Owner()->Get_Transform()->Go_Dir(XMLoadFloat3(&vSrcPen), 0.016f);
					_float3 vDstPen = pDstCollider->Get_Penetrated();
					pDstCollider->Get_Owner()->Get_Transform()->Go_Dir(XMLoadFloat3(&vDstPen), 0.016f);
				}
			}
		}
	}
}

_float3 CCollider_Manager::Resolve_Slide_AABB(CCollider* pMyCol, const _float3 vDesiredMove, _uint iGroupID)
{
	for (auto& pDstCollider : m_pColliders[iGroupID])
	{
		if (true == pMyCol->Intersect(pDstCollider))
		{
			_vector vMove = XMLoadFloat3(&vDesiredMove);
			auto normal = pMyCol->ComputeCollisionNormal_AABB(pDstCollider);
			_vector vNormal = XMLoadFloat3(&normal);

			_vector slideVec = vMove - XMVector3Dot(vMove, vNormal) * vNormal;

			_float3 result;
			XMStoreFloat3(&result, slideVec);

			return result;
		}
	}
	return vDesiredMove;
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
	fRayDist = fLastDistance;
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
		for (auto& Collider : m_pColliders[i])
			Safe_Release(Collider);
		m_pColliders[i].clear();
	}
	Safe_Delete_Array(m_pColliders);
}
