#pragma once

#include "AssaultRifle.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Base.h"

// 어디 적어둘 곳이 애매하군
// AR: Jakobs
// Pistol: Hyperion
// SMG가 몰름 


NS_BEGIN(Client)

class CARState abstract : public CBase
{
public:
	CARState(class CAssaultRifle* pOwner)
		: m_pOwner(pOwner),
		m_pGameInstance(CGameInstance::Get_Instance())
	{
		Safe_AddRef(m_pGameInstance);
	}
	virtual ~CARState() = default;

public:
	virtual void Enter() PURE;
	virtual void Execute(_float fTimeDelta) PURE;
	virtual void Exit() PURE;
	virtual void Free() override { __super::Free(); Safe_Release(m_pGameInstance); }

protected:
	class CAssaultRifle*	m_pOwner;
	_float			m_fElapsedTime = {};
	CGameInstance*	m_pGameInstance = { nullptr };
};

class CARState_Idle final : public CARState
{
public:
	CARState_Idle(class CAssaultRifle* pOwner)
		: CARState(pOwner) {}
	virtual ~CARState_Idle() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CARState_Draw final : public CARState
{
public:
	CARState_Draw(class CAssaultRifle* pOwner)
		: CARState(pOwner) {
	}
	virtual ~CARState_Draw() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CARState_Fire final : public CARState
{
public:
	CARState_Fire(class CAssaultRifle* pOwner)
		: CARState(pOwner) {
	}
	virtual ~CARState_Fire() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CARState_Reload final : public CARState
{
public:
	CARState_Reload(class CAssaultRifle* pOwner)
		: CARState(pOwner) {
		m_pOwner = static_cast<CAssaultRifle*>(pOwner);
	}
	virtual ~CARState_Reload() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CARState_Reload_Fast final : public CARState
{
public:
	CARState_Reload_Fast(class CAssaultRifle* pOwner)
		: CARState(pOwner) {
	}
	virtual ~CARState_Reload_Fast() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};




NS_END;
