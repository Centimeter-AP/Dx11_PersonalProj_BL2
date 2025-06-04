#pragma once

#include "Pistol.h"
#include "Client_Defines.h"
#include "GameInstance.h"

// 어디 적어둘 곳이 애매하군
// AR: Jakobs
// Pistol: Hyperion


NS_BEGIN(Client)

class CPSTState abstract : public CBase
{
public:
	CPSTState(class CPistol* pOwner)
		: m_pOwner(pOwner),
		m_pGameInstance(CGameInstance::Get_Instance())
	{
		Safe_AddRef(m_pGameInstance);
	}
	virtual ~CPSTState() = default;

public:
	virtual void Enter() PURE;
	virtual void Execute(_float fTimeDelta) PURE;
	virtual void Exit() PURE;
	virtual void Free() override { __super::Free(); Safe_Release(m_pGameInstance); }

protected:
	class CPistol*	m_pOwner;
	_float			m_fElapsedTime = {};
	CGameInstance*	m_pGameInstance = { nullptr };
};

class CPSTState_Idle final : public CPSTState
{
public:
	CPSTState_Idle(class CPistol* pOwner)
		: CPSTState(pOwner) {}
	virtual ~CPSTState_Idle() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CPSTState_Draw final : public CPSTState
{
public:
	CPSTState_Draw(class CPistol* pOwner)
		: CPSTState(pOwner) {
	}
	virtual ~CPSTState_Draw() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CPSTState_Fire final : public CPSTState
{
public:
	CPSTState_Fire(class CPistol* pOwner)
		: CPSTState(pOwner) {
	}
	virtual ~CPSTState_Fire() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CPSTState_Reload final : public CPSTState
{
public:
	CPSTState_Reload(class CPistol* pOwner)
		: CPSTState(pOwner) {
		m_pOwner = static_cast<CPistol*>(pOwner);
	}
	virtual ~CPSTState_Reload() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CPSTState_Reload_Fast final : public CPSTState
{
public:
	CPSTState_Reload_Fast(class CPistol* pOwner)
		: CPSTState(pOwner) {
	}
	virtual ~CPSTState_Reload_Fast() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};




NS_END;
