#include "ColliderManager.h"

CColliderManager::CColliderManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
}

HRESULT CColliderManager::Initialize()
{
	return S_OK;
}

HRESULT CColliderManager::Add_Collider(CCollider* pCollider)
{
	return S_OK;
}

void CColliderManager::Intersect(CCollider* pDst, CCollider* pSrc)
{
}

void CColliderManager::Intersect_Group(CCollider* pDst, CCollider* pSrc)
{
}

CColliderManager* CColliderManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

void CColliderManager::Free()
{
}
