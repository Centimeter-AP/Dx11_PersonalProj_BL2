#pragma once

#include "Base.h"

/* 레벨별로 구분하여 사본객체를 보관한다. */
/* 리스트들로 보관한다. 리스트 == 객체들 == 내가 나누고 싶은 기준에따라 그룹짓는다. */

NS_BEGIN(Engine)

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex);
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, _uint iPartID, const _wstring& strComponentTag, _uint iIndex);

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Clear(_uint iLevelIndex);

private:
	_uint								m_iNumLevels = {};
	typedef unordered_map<_wstring, class CLayer*> LAYERS;
	LAYERS* m_pLayers = { nullptr };
	class CGameInstance*				m_pGameInstance = { nullptr };

public:
	class CLayer* Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);
	class CGameObject* Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iVectorIndex);
	void Clear_Layer(_uint iLevelIndex, const _wstring& strLayerTag);
	void Active_Layer(_uint iLevelIndex, const _wstring& strLayerTag, _bool bActive);

public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

NS_END