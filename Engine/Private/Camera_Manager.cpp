#include "Camera_Manager.h"

HRESULT CCamera_Manager::Initialize(_uint iNumLevels)
{
    return S_OK;
}

void CCamera_Manager::Priority_Update(_float fTimeDelta)
{
}

void CCamera_Manager::Update(_float fTimeDelta)
{
}

void CCamera_Manager::Late_Update(_float fTimeDelta)
{
}

void CCamera_Manager::Clear(_uint iLevelIndex)
{
}

HRESULT CCamera_Manager::Add_Camera(CCamera* pCamera)
{
	return E_NOTIMPL;
}

CGameObject* CCamera_Manager::Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iVectorIndex)
{
    return nullptr;
}

CCamera_Manager* CCamera_Manager::Create(_uint iNumLevels)
{
	CCamera_Manager* pInstance = new CCamera_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed to Created : CCamera_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Manager::Free()
{
}
