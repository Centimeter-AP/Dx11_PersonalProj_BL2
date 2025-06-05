#include "Collider_Manager.h"
#include "Collider.h"
#include "GameObject.h"

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
			//if (true == pSrcCollider->Intersect(pDstCollider))
			//{
			//	pSrcCollider->Get_Owner()->On_Collision(pDstCollider->Get_ColliderID());
			//	pDstCollider->Get_Owner()->On_Collision(pSrcCollider->Get_ColliderID());
			//}
		}
	}
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

	Safe_Delete_Array(m_pColliders);
}
