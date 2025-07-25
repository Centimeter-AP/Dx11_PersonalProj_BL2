#pragma once
#include "Base.h"


NS_BEGIN(Engine)

class CCamera_Manager final : public CBase
{
private:
	CCamera_Manager() {};
	virtual ~CCamera_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Clear(_uint iLevelIndex);

	HRESULT Add_Camera(class CCamera* pCamera);

private:
	class CGameInstance*						m_pGameInstance = { nullptr };
	unordered_map<_wstring, class CCamera*>		m_Cameras;

public:
	class CGameObject* Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iVectorIndex);

public:
	static CCamera_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

NS_END

