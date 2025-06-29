#include "Layer.h"
#include "ContainerObject.h"
#include "GameInstance.h"

CLayer::CLayer()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
}

CComponent* CLayer::Get_Component(const _wstring& strComponentTag, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	
	return (*iter)->Get_Component(strComponentTag);
}

CComponent* CLayer::Get_Component(_uint iPartID, const _wstring& strComponentTag, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	return static_cast<CContainerObject*>(*iter)->Get_Component(iPartID, strComponentTag);
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

CGameObject* CLayer::Find_Object(_uint iIndex)
{
	if (m_GameObjects.empty() || iIndex >= m_GameObjects.size())
		return nullptr;

	auto Iter = m_GameObjects.begin();

	for (_uint i = 0; i < iIndex; ++i)
		Iter++;

	return *Iter;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Priority_Update(fTimeDelta);
	}
}

void CLayer::Clear()
{
	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}

void CLayer::Update(_float fTimeDelta)
{
	for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end(); )
	{
		EVENT Events = { EVENT::EVN_NONE };
		if (nullptr != (*iter))
			Events = (*iter)->Update(fTimeDelta);
		if (Events == EVENT::EVN_DEAD)
		{
			m_pGameInstance->Delete_Collider(*iter);
			Safe_Release(*iter);
			iter = m_GameObjects.erase(iter);
		}
		else
			++iter;
	}
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Late_Update(fTimeDelta);

	}
}

CLayer* CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);
	m_GameObjects.clear();
}
