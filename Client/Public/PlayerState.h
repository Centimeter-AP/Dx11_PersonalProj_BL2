#pragma once
#include "Player.h"
#include "AssaultRifle.h"
#include "Pistol.h"

NS_BEGIN(Engine)
class CNavigation;
NS_END

NS_BEGIN(Client) 


class CPlayerState abstract : public CBase
{
public:
	CPlayerState(class CPlayer* pOwner)
		: m_pOwner(pOwner),
		m_pGameInstance(CGameInstance::Get_Instance())
	{
		m_pOwnerNavi = m_pOwner->m_pNavigationCom;
		Safe_AddRef(m_pOwnerNavi);
		m_pIsGrounded = m_pOwner->m_pGravityCom->Is_Grounded_Ptr();
	}
	virtual ~CPlayerState() = default;

public:
	//상태 진입 시 한 번 호출
	virtual void Enter() PURE;
	//해당 상태일 때 계속 호출
	virtual void Execute(_float fTimeDelta) PURE;
	//상태 퇴장 시 한 번 호출
	virtual void Exit() PURE;
	virtual void Free() override { __super::Free(); Safe_Release(m_pOwnerNavi);/*Safe_Release(m_pGameInstance);*/ }
	void Delete_CurNavigation() {
		if (m_pOwnerNavi) {
			Safe_Release(m_pOwnerNavi);
			m_pOwnerNavi = nullptr;
		}
	}
	void Replace_Navigation(CNavigation* pNewNavi) {
		if (m_pOwnerNavi == nullptr)
		{
			m_pOwnerNavi = pNewNavi;
			Safe_AddRef(m_pOwnerNavi);
		}
	}
	void Check_Reloadable() {
		if (m_pOwner->m_iAmmo[m_pOwner->m_eCurWeapon])
			m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Reload);
		else
			m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
	}

protected:
	void Default_Movements(_float fTimeDelta) {
		if (KEY_PRESSING(DIK_W))
			m_pOwner->m_pTransformCom->Go_Straight_Hover(fTimeDelta, m_pOwnerNavi, *m_pIsGrounded);
		if (KEY_PRESSING(DIK_S))
			m_pOwner->m_pTransformCom->Go_Backward_Hover(fTimeDelta, m_pOwnerNavi, *m_pIsGrounded);
		if (KEY_PRESSING(DIK_A))
			m_pOwner->m_pTransformCom->Go_Left(fTimeDelta, m_pOwnerNavi, *m_pIsGrounded);
		if (KEY_PRESSING(DIK_D))
			m_pOwner->m_pTransformCom->Go_Right(fTimeDelta, m_pOwnerNavi, *m_pIsGrounded);
	}

protected:
	CPlayer*		m_pOwner;
	CNavigation*	m_pOwnerNavi = { nullptr };
	const _bool*	m_pIsGrounded = { nullptr };
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
	const _float m_fSpeedMultiplier = { 2.f };
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
	_bool	m_isLanded = { false };
};

class CPlayerState_Fire final : public CPlayerState
{
public:
	CPlayerState_Fire(class CPlayer* pOwner)
		: CPlayerState(pOwner) {
	}
	virtual ~CPlayerState_Fire() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

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

class CPlayerState_DrawWeapon final : public CPlayerState
{
public:
	CPlayerState_DrawWeapon(class CPlayer* pOwner)
		: CPlayerState(pOwner) {
	}
	virtual ~CPlayerState_DrawWeapon() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CPlayerState_HolsterWeapon final : public CPlayerState
{
public:
	CPlayerState_HolsterWeapon(class CPlayer* pOwner)
		: CPlayerState(pOwner) {
	}
	virtual ~CPlayerState_HolsterWeapon() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CPlayerState_ThrowGrenade final : public CPlayerState
{
public:
	CPlayerState_ThrowGrenade(class CPlayer* pOwner)
		: CPlayerState(pOwner) {
	}
	virtual ~CPlayerState_ThrowGrenade() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};

class CPlayerState_Skill_PhaseLock final : public CPlayerState
{
public:
	CPlayerState_Skill_PhaseLock(class CPlayer* pOwner)
		: CPlayerState(pOwner) {
	}
	virtual ~CPlayerState_Skill_PhaseLock() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
private:
	_bool m_bSuccess = { false };
	_bool m_bCatched = { false };
	_float3  vTargetPos = {};
};

class CPlayerState_CloseAttack final : public CPlayerState
{
public:
	CPlayerState_CloseAttack(class CPlayer* pOwner)
		: CPlayerState(pOwner) {
	}
	virtual ~CPlayerState_CloseAttack() = default;

public:
	virtual void Enter() override;
	virtual void Execute(_float fTimeDelta) override;
	virtual void Exit() override;
	virtual void Free() override { __super::Free(); }
};



NS_END;
