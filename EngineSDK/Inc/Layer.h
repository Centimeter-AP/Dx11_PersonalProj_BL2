#pragma once

#include "GameObject.h"

/* 객체들을 모아놓는다. */

NS_BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	class CComponent* Get_Component(const _wstring& strComponentTag, _uint iIndex);
	class CComponent* Get_Component(_uint iPartID, const _wstring& strComponentTag, _uint iIndex);

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

	CGameObject* Find_Object(_uint iVectorIndex);
	list<CGameObject*>& Get_LayerObjectLists() { return m_GameObjects; }
	list<CGameObject*>* Get_LayerObjectListsPtr() { return &m_GameObjects; }

private:
	list<CGameObject*>			m_GameObjects;
	class CGameInstance*		m_pGameInstance = { nullptr };

public:
	void Clear();

public:
	static CLayer* Create();
	virtual void Free();
};

NS_END