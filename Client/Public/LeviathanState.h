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
		m_pTarget = m_pOwner->m_pTarget;
		m_pTargetTransform = m_pTarget->Get_Transform();
	}
	virtual ~CLeviathanState() = default;

public:
	virtual void Enter() PURE;
	virtual void Execute(_float fTimeDelta) PURE;
	virtual void Exit() PURE;
	virtual void Free() override { __super::Free(); Safe_Release(m_pGameInstance); }
	void Set_OwnerAnim(CLeviathan::LEVI_ANIM eAnim, _bool isLoop)
	{
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(eAnim), isLoop);
	}
protected:
	CLeviathan*		m_pOwner;
	CGameObject*	m_pTarget = { nullptr };
	CTransform*		m_pTargetTransform = { nullptr };

	_float			m_fElapsedTime = {};
	CGameInstance*	m_pGameInstance = { nullptr };
};


class CLeviathanState_Engage final : public CLeviathanState
{
public:
	CLeviathanState_Engage(class CLeviathan* pOwner)
		: CLeviathanState(pOwner) {}
	virtual ~CLeviathanState_Engage() = default;

public:
	virtual void Enter() override
	{
		cout << " Boss State: Engage" << endl;

		Set_OwnerAnim(CLeviathan::LEVI_ANIM::Roar, false);

	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
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
		cout << " Boss State: Idle" << endl;

		if (m_pOwner->m_bInjured)
			Set_OwnerAnim(CLeviathan::LEVI_ANIM::idle_injured, true);
		else
			Set_OwnerAnim(CLeviathan::LEVI_ANIM::Idle, true);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);
		//m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Idle);
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CLeviathanState_Attack_Spray final : public CLeviathanState
{
public:
	CLeviathanState_Attack_Spray(class CLeviathan* pOwner)
		: CLeviathanState(pOwner) {
	}
	virtual ~CLeviathanState_Attack_Spray() = default;

public:
	virtual void Enter() override
	{
		cout << " Boss State: Attack_Spray" << endl;

		if (m_pOwner->m_bInjured)
			Set_OwnerAnim(CLeviathan::LEVI_ANIM::Injured_slag_spray, false);
		else
			Set_OwnerAnim(CLeviathan::LEVI_ANIM::Spray, false);


	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CLeviathanState_Attack_ThrowRock final : public CLeviathanState
{
public:
	CLeviathanState_Attack_ThrowRock(class CLeviathan* pOwner)
		: CLeviathanState(pOwner) {
	}
	virtual ~CLeviathanState_Attack_ThrowRock() = default;

public:
	virtual void Enter() override
	{
		cout << " Boss State: Idle" << endl;
		if (m_pOwner->m_bInjured)
			Set_OwnerAnim(CLeviathan::LEVI_ANIM::Injured_rock_throw, true);
		else
			Set_OwnerAnim(CLeviathan::LEVI_ANIM::Rock_Throw, true);


	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CLeviathanState_Attack_Slam final : public CLeviathanState
{
public:
	CLeviathanState_Attack_Slam(class CLeviathan* pOwner)
		: CLeviathanState(pOwner) {
	}
	virtual ~CLeviathanState_Attack_Slam() = default;

public:
	virtual void Enter() override
	{
		cout << " Boss State: Idle" << endl;

		Set_OwnerAnim(CLeviathan::LEVI_ANIM::Idle, true);

	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CLeviathanState_Attack_SpitWorm final : public CLeviathanState
{
public:
	CLeviathanState_Attack_SpitWorm(class CLeviathan* pOwner)
		: CLeviathanState(pOwner) {
	}
	virtual ~CLeviathanState_Attack_SpitWorm() = default;

public:
	virtual void Enter() override
	{
		cout << " Boss State: Idle" << endl;

		Set_OwnerAnim(CLeviathan::LEVI_ANIM::Idle, true);

	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};


class CLeviathanState_Damaged final : public CLeviathanState
{
public:
	CLeviathanState_Damaged(class CLeviathan* pOwner)
		: CLeviathanState(pOwner) {
	}
	virtual ~CLeviathanState_Damaged() = default;

public:
	virtual void Enter() override
	{
		cout << " Boss State: Idle" << endl;

		Set_OwnerAnim(CLeviathan::LEVI_ANIM::Idle, true);

	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};


class CLeviathanState_Roar final : public CLeviathanState
{
public:
	CLeviathanState_Roar(class CLeviathan* pOwner)
		: CLeviathanState(pOwner) {
	}
	virtual ~CLeviathanState_Roar() = default;

public:
	virtual void Enter() override
	{
		cout << " Boss State: Idle" << endl;

		Set_OwnerAnim(CLeviathan::LEVI_ANIM::Idle, true);

	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CLeviathanState_Stun_Left final : public CLeviathanState
{
public:
	CLeviathanState_Stun_Left(class CLeviathan* pOwner)
		: CLeviathanState(pOwner) {
	}
	virtual ~CLeviathanState_Stun_Left() = default;

public:
	virtual void Enter() override
	{
		cout << " Boss State: Idle" << endl;

		Set_OwnerAnim(CLeviathan::LEVI_ANIM::Idle, true);

	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};


class CLeviathanState_Stun_Right final : public CLeviathanState
{
public:
	CLeviathanState_Stun_Right(class CLeviathan* pOwner)
		: CLeviathanState(pOwner) {
	}
	virtual ~CLeviathanState_Stun_Right() = default;

public:
	virtual void Enter() override
	{
		cout << " Boss State: Idle" << endl;

		Set_OwnerAnim(CLeviathan::LEVI_ANIM::Idle, true);

	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};


class CLeviathanState_LookAround final : public CLeviathanState
{
public:
	CLeviathanState_LookAround(class CLeviathan* pOwner)
		: CLeviathanState(pOwner) {
	}
	virtual ~CLeviathanState_LookAround() = default;

public:
	virtual void Enter() override
	{
		cout << " Boss State: Idle" << endl;

		Set_OwnerAnim(CLeviathan::LEVI_ANIM::Idle, true);

	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CLeviathanState_Dead final : public CLeviathanState
{
public:
	CLeviathanState_Dead(class CLeviathan* pOwner)
		: CLeviathanState(pOwner) {
	}
	virtual ~CLeviathanState_Dead() = default;

public:
	virtual void Enter() override
	{
		cout << "[Dead]" << endl;
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CLeviathan::LEVI_ANIM::Death), false);
		m_pOwner->m_pModelCom->Set_Animation_TickPerSecond(ENUM_CLASS(CLeviathan::LEVI_ANIM::Death), 15.f);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (m_pOwner->m_bDead == true)
			return;
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			//m_pOwner->Set_Dead();
			//»Ï!
			return;
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};







NS_END