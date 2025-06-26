#pragma once

#include "Skag.h"
#include "GameInstance.h"
#include <Player.h>

NS_BEGIN(Client)

class CSkagState abstract : public CBase
{
public:
	CSkagState(class CSkag* pOwner)
		: m_pOwner(pOwner),
		m_pGameInstance(CGameInstance::Get_Instance())
	{
		m_pOwnerNavi = m_pOwner->m_pNavigationCom;
		m_pTarget = m_pOwner->m_pTarget;
		m_pTargetTransform = m_pTarget->Get_Transform();
	}
	virtual ~CSkagState() = default;

public:
	virtual void Enter() PURE;
	virtual void Execute(_float fTimeDelta) PURE;
	virtual void Exit() PURE;
	virtual void Free() override {
		__super::Free(); 
	}

	virtual _bool Is_Target_Found(){
		auto Dist = fabs(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTargetTransform->Get_State(STATE::POSITION))));
		return (Dist < m_pOwner->m_fDetectiveDistance);
	}
	virtual _bool Is_Target_Attackable() {
		auto Dist = fabs(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTargetTransform->Get_State(STATE::POSITION))));
		return (Dist< m_pOwner->m_fAttackableDistance);
	}
	virtual void Detect_Target() {
		if (Is_Target_Found())
		{
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked);
		}
	}
	virtual void Call_State(CSkag::SKAG_STATE eNextState) {
		//if (m_)
	}
	void Set_OwnerAnim(CSkag::SKAG_ANIM eAnim, _bool isLoop)
	{
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(eAnim), isLoop);
	}

#ifdef _CONSOLE
	void Console(_float fTimeDelta)
	{
		m_fTestTick += fTimeDelta;
		if (m_fTestTick >= 1.f)
		{
			// 쓰고 싶은 것..
			//auto Dist =/* fabs*/(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTarget->Get_Transform()->Get_State(STATE::POSITION))));

			//cout << "Dist : " << Dist << endl;

			m_fTestTick = 0.f;
		}
	}
#endif // _CONSOLE

protected:
	CSkag*			m_pOwner;
	CNavigation*	m_pOwnerNavi = { nullptr };
	CGameObject*	m_pTarget = { nullptr };
	CTransform*		m_pTargetTransform = { nullptr };

	_float			m_fElapsedTime = {};
	CGameInstance*	m_pGameInstance = { nullptr };
	_bool			m_bChangeStateCall = { false };

	_float			m_fTestTick = {};


	const _float	m_fSkillCheck = 5.f;
};


/*
	Idle - Patrol - (여유가 된다면)Perch
	  ㄴ Provoked -> Provoked_Idle (전투 중에 지속) (Idle상태에서는 공격 쿨타임 체크 후 state 재변경)
							 ㄴ (거리 기반 공격 조건 분기) Attack_Charge, Run, Leap
							 ㄴ Run -> RunBite / RunTongue
							 ㄴ (가까워졌을 때 중거리에서 추가 분기 - 한번 체크 후 Exit할 때 bool초기화) Attack_Leap
							 ㄴ (근거리에서 가만히 있을 때 공격) Attack_Bite, Attack_Claw
	  ㄴ 피격시 확률 적으로 Dodge? (일단 x)
	  ㄴ 이런 

	  몹 종류를 나눌까?

	  기본적으로 Run으로 공격 사거리까지 다가온 후 공격
	  공격 쿨 딱히
	  leap는 무슨 조건으로 검사하는지 잘 모르겠음 근데 한번만 체크 후 false일 시 근접공격하게 run
	  charge는 더 먼 상태에서 한번 검사?
	  무리가 있고 한번 걸리면 무리가 한번에 공격을 수행..
*/

class CSkagState_Idle final : public CSkagState
{
public:
	CSkagState_Idle(class CSkag* pOwner)
		: CSkagState(pOwner) {}
	virtual ~CSkagState_Idle() = default;

public:
	virtual void Enter() override
	{
		cout << "Idle" << endl;
		Set_OwnerAnim(CSkag::SKAG_ANIM::Idle, true);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		//Console(fTimeDelta);
		m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);
		if (Is_Target_Found())
		{
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked);
			return;
		}

	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CSkagState_Patrol final : public CSkagState
{
public:
	CSkagState_Patrol(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Patrol() = default;

public:
	virtual void Enter() override
	{
		cout << "patrol" << endl;
		Set_OwnerAnim(CSkag::SKAG_ANIM::Patrol_Walk_F, true);
	}
	virtual void Execute(_float fTimeDelta) override
	{

	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};


class CSkagState_Hit final : public CSkagState
{
public:
	CSkagState_Hit(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Hit() = default;

public:
	virtual void Enter() override
	{
		cout << "Hit" << endl;
		Set_OwnerAnim(CSkag::SKAG_ANIM::Hardflinch1, true);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CSkagState_Turn180 final : public CSkagState
{
public:
	CSkagState_Turn180(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Turn180() = default;

public:
	virtual void Enter() override
	{
		cout << "turn180" << endl;
		m_pOwner->m_pModelCom->Set_Animation_TickPerSecond(ENUM_CLASS(CSkag::SKAG_ANIM::Dodge_L), 20.f);
		Set_OwnerAnim(CSkag::SKAG_ANIM::Dodge_L, false);
		m_fDecel = 1.f;
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fDecel *= 0.98f;
		m_fDecel = clamp(m_fDecel, 0.3f, 1.f);
		//m_pOwner->m_pTransformCom->Go_Straight_Hover(fTimeDelta * m_fDecel, m_pOwnerNavi);
		//m_pOwner->m_pTransformCom->Go_Right(fTimeDelta * m_fDecel, m_pOwnerNavi);
		m_pOwner->m_pTransformCom->LookAtLerp_NoY(m_pTargetTransform->Get_State(STATE::POSITION), fTimeDelta, 10.f);
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked_Idle);
		}
	}
	virtual void Exit() override
	{
	}
	virtual void Free() override { __super::Free(); }
private:
	_float m_fDecel = {1.f};
};


class CSkagState_Provoked final : public CSkagState
{
public:
	CSkagState_Provoked(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Provoked() = default;

public:
	virtual void Enter() override
	{
		cout << "provoked" << endl;
		rand() % 2 ?
			Set_OwnerAnim(CSkag::SKAG_ANIM::Provoked_var1, false) :
			Set_OwnerAnim(CSkag::SKAG_ANIM::Provoked_var2, false);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		Console(fTimeDelta);
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CSkagState_Provoked_Idle final : public CSkagState
{
public:
	CSkagState_Provoked_Idle(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Provoked_Idle() = default;

public:
	virtual void Enter() override
	{
		cout << "provoked_idle" << endl;
		Set_OwnerAnim(CSkag::SKAG_ANIM::Idle, true);

		// 바로바로 전환 해주는 용도로 거치기만 해도 될듯?
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);
		m_fCooldown += fTimeDelta;
		if (m_fCooldown > 0.5f)
		{
			auto angle = m_pOwner->m_pTransformCom->Compute_Target_Look_Angle(m_pTargetTransform->Get_State(STATE::POSITION));
			auto angle150 = XMConvertToRadians(150);
			if (angle > angle150) //각도 차이 확인
			{
				m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Turn180); return;
			}

			
			if (Is_Target_Attackable())
			{
				rand() % 2 ?
					m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Attack_Bite) :
					m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Attack_Tongue);
			}
			else
			{
				m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Run);
			}
		}
	}
	virtual void Exit() override
	{
		m_fCooldown = 0.f;
	}
	virtual void Free() override { __super::Free(); }

private:
	_float m_fCooldown = {};
};

#pragma region Attack_Patterns

class CSkagState_Attack_Bite final : public CSkagState
{
public:
	CSkagState_Attack_Bite(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Attack_Bite() = default;

public:
	virtual void Enter() override
	{
		cout << "a_bite" << endl;
		rand() % 2 ?
			Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Bite1, false) :
			Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Bite2, false);
		m_pOwner->m_pColliderAttackCom->Set_Active(true);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked_Idle);
		}
	}
	virtual void Exit() override
	{
		m_pOwner->m_pColliderAttackCom->Set_Active(false);
		m_pOwner->Set_FirstHit_True();
	}
	virtual void Free() override { __super::Free(); }
};

class CSkagState_Attack_Tongue final : public CSkagState
{
public:
	CSkagState_Attack_Tongue(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Attack_Tongue() = default;

public:
	virtual void Enter() override
	{
		cout << "a_bite" << endl;
		rand() % 2 ?
			Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Tongue1, false) :
			Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Tongue2, false);
		m_pOwner->m_pColliderAttackCom->Set_Active(true);

	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked_Idle);
		}
	}
	virtual void Exit() override
	{
		m_pOwner->m_pColliderAttackCom->Set_Active(false);
		m_pOwner->Set_FirstHit_True();

	}
	virtual void Free() override { __super::Free(); }
};

// ChargeStart도 분리하면 좋을 것 같은데요..
class CSkagState_Attack_Charge final : public CSkagState
{
public:
	CSkagState_Attack_Charge(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Attack_Charge() = default;
	//charge
	//	start - loop - (Stop / Strike / hitwall)
public:
	virtual void Enter() override
	{
		cout << "a_charge" << endl;
		Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Charge_Start, false);
		ChargeStatus[0] = true;
	}
	virtual void Execute(_float fTimeDelta) override
	{

		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			if (ChargeStatus[2] == true)
				m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked_Idle);
			else if (ChargeStatus[0] == true)
			{
				ChargeStatus[1] = true;
				Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Charge_Loop, true);
				m_pOwner->m_pColliderAttackCom->Set_Active(true);
				//XMStoreFloat3(&vChargePos, m_pTargetTransform->Get_State(STATE::POSITION));
			}
		}
		if (ChargeStatus[1] == false)
		{   // 준비 중에 고개 돌리기
			m_pOwner->m_pTransformCom->LookAtLerp_NoY(m_pTargetTransform->Get_State(STATE::POSITION), fTimeDelta, 8.f);
		}
		if (ChargeStatus[1] == true && ChargeStatus[2] == false)
		{
			fChargingTime += fTimeDelta; // 최소 돌진 시간 잡아두기
			if (fChargingTime > fForceChargingTime)
			{
				auto Dist = fabs(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTargetTransform->Get_State(STATE::POSITION))));
				if (Dist >= 10.f || fChargingTime > 4.f)
				{
					ChargeStatus[2] = true;
					m_pOwner->m_pColliderAttackCom->Set_Active(false);
					Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Charge_Stop, false);
					m_pOwner->Set_FirstHit_True();
				}
			}
			//m_pOwner->m_pTransformCom->Go_Target(XMVectorSetW(XMLoadFloat3(&vChargePos), 1.f), fTimeDelta * 1.2f, 1.f);
			m_pOwner->m_pTransformCom->LookAtLerp_NoY(m_pTargetTransform->Get_State(STATE::POSITION), fTimeDelta, 2.5f);

			m_pOwner->m_pTransformCom->Go_Straight_Hover(fTimeDelta * 2.f, m_pOwnerNavi);
		}
		if (ChargeStatus[2] == true)
		{
			//m_pOwner->m_pTransformCom->Go_Target(XMVectorSetW(XMLoadFloat3(&vChargePos), 1.f), fTimeDelta * fDecel, 1.f);
			m_pOwner->m_pTransformCom->Go_Straight_Hover(fTimeDelta * fDecel, m_pOwnerNavi);
			fDecel *= 0.95f;
		}
	}
	virtual void Exit() override
	{
		for (size_t i = 0; i < 3; i++)
		{
			ChargeStatus[i] = false;
		}
		vChargePos = {};
		fDecel = 0.9f;
		fChargingTime = {};
	}
	virtual void Free() override { __super::Free(); }
private:
	_bool ChargeStatus[3] = { false }; // 0 Start, 1 Loop, 2 Stop
	_float3 vChargePos = {};
	_float fDecel = {0.9f};
	_float fChargingTime = {};
	const _float fForceChargingTime = {2.f};
};

class CSkagState_Attack_Charge_HitWall final : public CSkagState
{
public:
	CSkagState_Attack_Charge_HitWall(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Attack_Charge_HitWall() = default;

public:
	virtual void Enter() override
	{
		cout << "a_Charge_HitWall" << endl;
		Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Charge_Hitwall, false);
		m_pOwner->m_pColliderAttackCom->Set_Active(false);
		m_pOwner->Set_FirstHit_True();
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CSkagState_Attack_Charge_Strike final : public CSkagState
{
public:
	CSkagState_Attack_Charge_Strike(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Attack_Charge_Strike() = default;

public:
	virtual void Enter() override
	{
		cout << "a_Charge_Strike" << endl;
		Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Charge_Strike, false);
		m_pOwner->m_pColliderAttackCom->Set_Active(false);
		m_pOwner->Set_FirstHit_True();
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};


// deprecated
class CSkagState_Attack_Claw final : public CSkagState
{
public:
	CSkagState_Attack_Claw(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Attack_Claw() = default;

public:
	virtual void Enter() override
	{
		cout << "a_claw" << endl;

	}
	virtual void Execute(_float fTimeDelta) override
	{

	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CSkagState_Attack_Leap final : public CSkagState
{
public:
	CSkagState_Attack_Leap(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Attack_Leap() = default;

public:
	virtual void Enter() override
	{
		cout << "a_Leap" << endl;
		Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Leap_Start, false);
		LeapStatus[0] = true;
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			if (LeapStatus[2] == true)
				m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked_Idle);
			else if (LeapStatus[1] == true)
			{
				LeapStatus[2] = true;
				m_pOwner->m_pColliderAttackCom->Set_Active(false);
				m_pOwner->Set_FirstHit_True();
				Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Leap_End, false);
			}
			else if (LeapStatus[0] == true)
			{
				LeapStatus[1] = true;
				Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Leap_Idle, false);
				m_pOwner->m_pColliderAttackCom->Set_Active(true);
				//XMStoreFloat3(&vLeapPos, m_pTargetTransform->Get_State(STATE::POSITION));
			}
		}
		if (LeapStatus[2] == true)
		{
			m_pOwner->m_pTransformCom->Go_Straight_Hover(fTimeDelta * fDecel, m_pOwnerNavi);
			fDecel *= 0.95f;
		}
		else if (LeapStatus[1] == true)
		{
			m_pOwner->m_pTransformCom->Go_Target(m_pTargetTransform->Get_State(STATE::POSITION), fTimeDelta * 3.f, 1.f, m_pOwnerNavi);
			m_pOwner->m_pTransformCom->LookAtLerp_NoY(m_pTargetTransform->Get_State(STATE::POSITION), fTimeDelta, 8.f);
		}
		//else if (LeapStatus[0] == true)
		//{

		//}
	}
	virtual void Exit() override
	{
		for (size_t i = 0; i < 3; i++)
		{
			LeapStatus[i] = false;
		}
		vLeapPos = {};
		fDecel = 0.9f;
		fLeapTime = {};
	}
	virtual void Free() override { __super::Free(); }

private:
	_bool LeapStatus[3] = { false }; // 0 Start, 1 Idle, 2 End
	_float3 vLeapPos = {};
	_float fDecel = { 0.9f };
	_float fLeapTime = {};
	const _float fForceLeapTime = { 1.f };
};

class CSkagState_Attack_RunBite final : public CSkagState
{
public:
	CSkagState_Attack_RunBite(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Attack_RunBite() = default;

public:
	virtual void Enter() override
	{
		cout << "a_runbite" << endl;
		m_pOwner->m_pColliderAttackCom->Set_Active(true);
		Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Run_Bite, false);
		m_pOwner->m_pTransformCom->LookAt_NoY(m_pTarget->Get_Transform()->Get_State(STATE::POSITION));
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->m_pColliderAttackCom->Set_Active(false);
			m_pOwner->Set_FirstHit_True();
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked_Idle);
		}
		m_pOwner->m_pTransformCom->Go_Straight_Hover(fTimeDelta*1.5f, m_pOwnerNavi);
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CSkagState_Attack_RunTongue final : public CSkagState
{
public:
	CSkagState_Attack_RunTongue(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Attack_RunTongue() = default;

public:
	virtual void Enter() override
	{
		cout << "a_runtongue" << endl;
		m_pOwner->m_pColliderAttackCom->Set_Active(true);
		Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Run_Tongue, false);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->m_pColliderAttackCom->Set_Active(false);
			m_pOwner->Set_FirstHit_True();
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked_Idle);
		}
		m_pOwner->m_pTransformCom->Go_Straight_Hover(fTimeDelta, m_pOwnerNavi);

	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

#pragma endregion

class CSkagState_Run final : public CSkagState
{
public:
	CSkagState_Run(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Run() = default;

public:
	virtual void Enter() override
	{
		cout << "run" << endl;
		rand() % 2 ?( rand() % 2 ?
		Set_OwnerAnim(CSkag::SKAG_ANIM::Run_F, true) :
		Set_OwnerAnim(CSkag::SKAG_ANIM::Run_F3, true)) :
		Set_OwnerAnim(CSkag::SKAG_ANIM::Run_F_Big1, true);
		m_pOwner->m_pModelCom->Play_Animation(0.001f);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		Console(fTimeDelta);
		m_pOwner->m_pTransformCom->LookAtLerp_NoY(m_pTarget->Get_Transform()->Get_State(STATE::POSITION), fTimeDelta, 8.f);
		m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);

		auto Dist = fabs(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTargetTransform->Get_State(STATE::POSITION))));

		m_pOwner->AttackTimer(fTimeDelta);

		if (Dist < m_fChargeDist)
		{
			if (m_pOwner->m_bChargeCheck)
			{
				m_pOwner->m_fChargeCheckTimer = 0.f;
				m_pOwner->m_bChargeCheck = false;
				m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Attack_Charge);
				return;
			}
		}
		if (Dist < m_fLeapDist)
		{
			if (m_pOwner->m_bLeapCheck)
			{
				m_pOwner->m_fLeapCheckTimer = 0.f;
				m_pOwner->m_bLeapCheck = false;
				m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Attack_Leap);
				return;
			}
		}
		if (Dist < m_fRunAtkDist)
		{
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Attack_Run_Bite);
			return;
				//m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Attack_Tongue);
		}

		m_pOwner->m_pTransformCom->Go_Straight_Hover(/*m_pTargetTransform->Get_State(STATE::POSITION), */fTimeDelta/*, 1.f*/, m_pOwnerNavi);
		
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }

private:
	const _float m_fChargeDist = { 40.f };
	const _float m_fLeapDist = { 30.f };
	const _float m_fRunAtkDist = { 15.f };
};

class CSkagState_Phaselocked final : public CSkagState
{
public:
	CSkagState_Phaselocked(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Phaselocked() = default;

public:
	virtual void Enter() override
	{
		cout << "Phaselocked" << endl;
		Set_OwnerAnim(CSkag::SKAG_ANIM::PhaseLock_Lift, false);
		//m_pOwner->m_pModelCom->Set_Animation_TickPerSecond(ENUM_CLASS(CSkag::SKAG_ANIM::PhaseLock_Lift), 15.f);
		m_ePhaselockStatus = PL_LIFT;
		m_fPhaselockTicker = 0.f;
		m_pOwner->m_pGravityCom->Set_IsGrounded(false);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			switch (m_ePhaselockStatus)
			{
			case PL_LIFT:
				m_ePhaselockStatus = PL_LOOP;
				Set_OwnerAnim(CSkag::SKAG_ANIM::PhaseLock_Loop, true);
				break;
			case PL_FALL:
			//	m_ePhaselockStatus = PL_LAND;
			//	//m_pOwner->m_pModelCom->Set_Animation_TickPerSecond(ENUM_CLASS(CSkag::SKAG_ANIM::PhaseLock_Land), .f);
			//	Set_OwnerAnim(CSkag::SKAG_ANIM::PhaseLock_Land, false);
				break;
			case PL_LAND:
				//m_pOwner->m_pGravityCom->Set_IsGrounded(true);
				m_pOwner->Set_State(CSkag::STATE_Provoked_Idle);
				break;
			default:
				break;
			}
		}

		switch (m_ePhaselockStatus)
		{
		case PL_LIFT:
			m_pOwner->m_pTransformCom->Go_Up(fTimeDelta * 0.5f);
			break;
		case PL_LOOP:
			m_fPhaselockTicker += fTimeDelta;
			if (m_fPhaselockTicker >= static_cast<CPlayer*>(m_pTarget)->Get_PhaselockDuration())
			{
				m_ePhaselockStatus = PL_FALL;
				m_pOwner->m_pModelCom->Set_Animation_TickPerSecond(ENUM_CLASS(CSkag::SKAG_ANIM::PhaseLock_Fall), 3.f); 
				Set_OwnerAnim(CSkag::SKAG_ANIM::PhaseLock_Fall, false);
			}
			break;
		case PL_FALL:
			m_pOwner->m_pGravityCom->Update(fTimeDelta);
			if (m_pOwner->m_pGravityCom->Is_Grounded())
			{
				m_ePhaselockStatus = PL_LAND;
				//m_pOwner->m_pModelCom->Set_Animation_TickPerSecond(ENUM_CLASS(CSkag::SKAG_ANIM::PhaseLock_Land), 45.f);
				Set_OwnerAnim(CSkag::SKAG_ANIM::PhaseLock_Land, false);
			}
			break;
		case PL_LAND:
			break;
		default:
			break;
		}
	}
	virtual void Exit() override
	{
	}
	virtual void Free() override { __super::Free(); }
private:
	enum PL_STATUS { PL_LIFT, PL_LOOP, PL_FALL, PL_LAND };
	PL_STATUS	m_ePhaselockStatus = {};

	_float m_fPhaselockTicker = {}; // 플레이어의 스킬 시전시간 따라가게 처리 할 것 
};

class CSkagState_Dead final : public CSkagState
{
public:
	CSkagState_Dead(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Dead() = default;

public:
	virtual void Enter() override
	{
		cout << "[Dead]" << endl;
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CSkag::SKAG_ANIM::Death_var1), false);
		m_pOwner->m_pModelCom->Set_Animation_TickPerSecond(ENUM_CLASS(CSkag::SKAG_ANIM::Death_var1), 15.f);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (m_pOwner->m_bDead == true)
			return;
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_Dead();
			return;
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

NS_END;
