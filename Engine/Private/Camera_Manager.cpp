#include "Camera_Manager.h"

HRESULT CCamera_Manager::Initialize(_uint iNumLevels)
{
    return E_NOTIMPL;
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

CGameObject* CCamera_Manager::Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iVectorIndex)
{
    return nullptr;
}

CCamera_Manager* CCamera_Manager::Create(_uint iNumLevels)
{
    return nullptr;
}

void CCamera_Manager::Free()
{
}
