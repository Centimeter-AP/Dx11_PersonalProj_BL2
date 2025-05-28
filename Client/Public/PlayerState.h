#pragma once
#include "Player.h"
#include "AssaultRifle.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Base.h"


NS_BEGIN(Client) 


class CPlayerState abstract : public CBase
{
public:
	CPlayerState(class CPlayer* pOwner)
		: m_pOwner(pOwner),
		m_pGameInstance(CGameInstance::Get_Instance())
	{
		Safe_AddRef(m_pGameInstance);
	}
	virtual ~CPlayerState() = default;

public:
	//상태 진입 시 한 번 호출
	virtual void Enter() PURE;
	//해당 상태일 때 계속 호출
	virtual void Execute(_float fTimeDelta) PURE;
	//상태 퇴장 시 한 번 호출
	virtual void Exit() PURE;
	virtual void Free() override { __super::Free(); Safe_Release(m_pGameInstance); }

protected:
	CPlayer*		m_pOwner;
	_float			m_fElapsedTime = {};
	CGameInstance*	m_pGameInstance = { nullptr };
};


class CPlayerState_Idle final : public CPlayerState
{
public:
	CPlayerState_Idle(class CPlayer* pOwner)
		: CPlayerState(pOwner) {}
	virtual ~CPlayerState_Idle() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CPlayerState_Run final : public CPlayerState
{
public:
	CPlayerState_Run(class CPlayer* pOwner)
		: CPlayerState(pOwner) {
	}
	virtual ~CPlayerState_Run() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CPlayerState_Run_R final : public CPlayerState
{
public:
	CPlayerState_Run_R(class CPlayer* pOwner)
		: CPlayerState(pOwner) {
	}
	virtual ~CPlayerState_Run_R() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CPlayerState_Run_L final : public CPlayerState
{
public:
	CPlayerState_Run_L(class CPlayer* pOwner)
		: CPlayerState(pOwner) {
	}
	virtual ~CPlayerState_Run_L() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CPlayerState_Sprint final : public CPlayerState
{
public:
	CPlayerState_Sprint(class CPlayer* pOwner)
		: CPlayerState(pOwner) {
	}
	virtual ~CPlayerState_Sprint() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
private:
	const _float m_fSpeedMultiplier = { 1.3f };
};

class CPlayerState_Jump final : public CPlayerState
{
public:
	CPlayerState_Jump(class CPlayer* pOwner)
		: CPlayerState(pOwner) {
	}
	virtual ~CPlayerState_Jump() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
private:
	_bool	m_isJumping = { false };
};

//class CPlayerState_Fire final : public CPlayerState
//{
//public:
//	CPlayerState_Fire(class CPlayer* pOwner)
//		: CPlayerState(pOwner) {
//	}
//	virtual ~CPlayerState_Fire() = default;
//
//public:
//	virtual void Enter() override;
//	virtual void Execute(_float fTimeDelta) override;
//	virtual void Exit() override;
//	virtual void Free() override { __super::Free(); }
//};
//
class CPlayerState_Reload final : public CPlayerState
{
public:
	CPlayerState_Reload(class CPlayer* pOwner)
		: CPlayerState(pOwner) {
	}
	virtual ~CPlayerState_Reload() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

//class CPlayerState_ChangeWeapon final : public CPlayerState
//{
//public:
//	CPlayerState_ChangeWeapon(class CPlayer* pOwner)
//		: CPlayerState(pOwner) {
//	}
//	virtual ~CPlayerState_ChangeWeapon() = default;
//
//public:
//	virtual void Enter() override;
//	virtual void Execute(_float fTimeDelta) override;
//	virtual void Exit() override;
//	virtual void Free() override { __super::Free(); }
//};
//
//class CPlayerState_ThrowGrenade final : public CPlayerState
//{
//public:
//	CPlayerState_ThrowGrenade(class CPlayer* pOwner)
//		: CPlayerState(pOwner) {
//	}
//	virtual ~CPlayerState_ThrowGrenade() = default;
//
//public:
//	virtual void Enter() override;
//	virtual void Execute(_float fTimeDelta) override;
//	virtual void Exit() override;
//	virtual void Free() override { __super::Free(); }
//};
//
//class CPlayerState_Skill_PhaseLock final : public CPlayerState
//{
//public:
//	CPlayerState_Skill_PhaseLock(class CPlayer* pOwner)
//		: CPlayerState(pOwner) {
//	}
//	virtual ~CPlayerState_Skill_PhaseLock() = default;
//
//public:
//	virtual void Enter() override;
//	virtual void Execute(_float fTimeDelta) override;
//	virtual void Exit() override;
//	virtual void Free() override { __super::Free(); }
//};







NS_END;
