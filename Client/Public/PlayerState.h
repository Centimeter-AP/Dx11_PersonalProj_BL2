#pragma once
#include "State.h"
#include "Player.h"

NS_BEGIN(Client)

class CPlayerState_Idle final : public CState
{
public:
	CPlayerState_Idle(class CPlayer* pOwner)
		: CState(pOwner) {}
	virtual ~CPlayerState_Idle() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CPlayerState_Run final : public CState
{
public:
	CPlayerState_Run(class CPlayer* pOwner)
		: CState(pOwner) {
	}
	virtual ~CPlayerState_Run() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CPlayerState_Run_R final : public CState
{
public:
	CPlayerState_Run_R(class CPlayer* pOwner)
		: CState(pOwner) {
	}
	virtual ~CPlayerState_Run_R() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CPlayerState_Run_L final : public CState
{
public:
	CPlayerState_Run_L(class CPlayer* pOwner)
		: CState(pOwner) {
	}
	virtual ~CPlayerState_Run_L() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CPlayerState_Sprint final : public CState
{
public:
	CPlayerState_Sprint(class CPlayer* pOwner)
		: CState(pOwner) {
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

class CPlayerState_Jump final : public CState
{
public:
	CPlayerState_Jump(class CPlayer* pOwner)
		: CState(pOwner) {
	}
	virtual ~CPlayerState_Jump() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

//class CPlayerState_Fire final : public CState
//{
//public:
//	CPlayerState_Fire(class CPlayer* pOwner)
//		: CState(pOwner) {
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
//class CPlayerState_Reload final : public CState
//{
//public:
//	CPlayerState_Reload(class CPlayer* pOwner)
//		: CState(pOwner) {
//	}
//	virtual ~CPlayerState_Reload() = default;
//
//public:
//	virtual void Enter() override;
//	virtual void Execute(_float fTimeDelta) override;
//	virtual void Exit() override;
//	virtual void Free() override { __super::Free(); }
//};
//
//class CPlayerState_ChangeWeapon final : public CState
//{
//public:
//	CPlayerState_ChangeWeapon(class CPlayer* pOwner)
//		: CState(pOwner) {
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
//class CPlayerState_ThrowGrenade final : public CState
//{
//public:
//	CPlayerState_ThrowGrenade(class CPlayer* pOwner)
//		: CState(pOwner) {
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
//class CPlayerState_Skill_PhaseLock final : public CState
//{
//public:
//	CPlayerState_Skill_PhaseLock(class CPlayer* pOwner)
//		: CState(pOwner) {
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
