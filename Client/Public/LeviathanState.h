#pragma once

#include "Leviathan.h"
#include "GameInstance.h"

NS_BEGIN(Client)

class CLeviathanState abstract : public CBase
{
public:
	CLeviathanState(class CLeviathan* pOwner)
		: m_pOwner(pOwner),
		m_pGameInstance(CGameInstance::Get_Instance())
	{
		Safe_AddRef(m_pGameInstance);
	}
	virtual ~CLeviathanState() = default;

public:
	virtual void Enter() PURE;
	virtual void Execute(_float fTimeDelta) PURE;
	virtual void Exit() PURE;
	virtual void Free() override { __super::Free(); Safe_Release(m_pGameInstance); }

protected:
	CLeviathan*		m_pOwner;
	CGameObject*	m_pTarget = { nullptr };

	_float			m_fElapsedTime = {};
	CGameInstance*	m_pGameInstance = { nullptr };
};

class CLeviathanState_Idle final : public CLeviathanState
{
public:
	CLeviathanState_Idle(class CLeviathan* pOwner)
		: CLeviathanState(pOwner) {}
	virtual ~CLeviathanState_Idle() = default;

public:
	virtual void Enter() override
	{

	}
	virtual void Execute(_float fTimeDelta) override
	{

	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

NS_END