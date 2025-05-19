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

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	CGameObject* Find_Object(_uint iVectorIndex);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

	list<CGameObject*>& Get_LayerObjectLists() { return m_GameObjects; }

private:
	list<CGameObject*>			m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free();
};

NS_END