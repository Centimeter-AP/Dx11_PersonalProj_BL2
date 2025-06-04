#include "GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

CGameObject::CGameObject(const CGameObject& Prototype)
	: m_pDevice { Prototype.m_pDevice }
	, m_pContext { Prototype.m_pContext }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

CComponent* CGameObject::Get_Component(const _wstring& strComponentTag)
{
	auto	iter = m_Components.find(strComponentTag);
	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

HRESULT CGameObject::Initialize_Prototype()
{	

	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	CGameObject::DESC* pDesc = static_cast<CGameObject::DESC*>(pArg);
	m_szName = pDesc->szName;
	m_strVIBufferTag = pDesc->strVIBufferTag;
	m_pParentObject = pDesc->pParentObject;
	m_pParentMatrix = pDesc->pParentMatrix;
	m_iLevelID = pDesc->iLevelID;

	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;	

	if (FAILED(m_pTransformCom->Initialize(pArg)))
		return E_FAIL;

	m_Components.emplace(g_strTransformTag, m_pTransformCom);

	Safe_AddRef(m_pTransformCom);


	return S_OK;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;
}

EVENT CGameObject::Update(_float fTimeDelta)
{
	if (!m_isActive)
		return EVN_NONE;
	if (m_bDead)
		return EVN_DEAD;
	return EVN_NONE;
}

void CGameObject::Late_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;
}

HRESULT CGameObject::Render()
{
	if (!m_isActive)
		return S_OK;
	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg)
{
	CComponent*	pComponent = static_cast<CComponent*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_COMPONENT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	return S_OK;
}

HRESULT CGameObject::Add_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPartObjKey, const _wstring& strPrototypeTag, void* pArg)
{
	CGameObject* pPartObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pPartObject)
		return E_FAIL;

	m_PartObjects.emplace(strPartObjKey, pPartObject);

	return S_OK;
}

HRESULT CGameObject::Add_PartObject(const _wstring& strPartObjKey, CGameObject* pPartObject)
{
	if (nullptr == pPartObject)
		return E_FAIL;

	m_PartObjects.emplace(strPartObjKey, pPartObject);

	return S_OK;
}

CGameObject* CGameObject::Replace_PartObject(const _wstring& strPartObjKey, CGameObject* pReplaceObject)
{
	if (nullptr == pReplaceObject)
		return nullptr;

	auto	iter = m_PartObjects.find(strPartObjKey);
	if (iter == m_PartObjects.end())
		return nullptr;

	CGameObject* ReplacedPart = iter->second;
	iter->second = pReplaceObject;

	return ReplacedPart;
}

CGameObject* CGameObject::Replace_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPartObjKey, const _wstring& strPrototypeTag, void* pArg)
{
	auto	iter = m_PartObjects.find(strPartObjKey);
	if (iter == m_PartObjects.end())
		return nullptr;

	CGameObject* pPartObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pPartObject)
		return nullptr;

	CGameObject* ReplacedPart = iter->second;
	iter->second = pPartObject;

	return ReplacedPart;
}



CGameObject* CGameObject::Find_PartObject(const _wstring& strPartObjKey)
{
	auto	iter = m_PartObjects.find(strPartObjKey);
	if (iter == m_PartObjects.end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pTransformCom);

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
