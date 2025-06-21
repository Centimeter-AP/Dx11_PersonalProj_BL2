#pragma once

#include "Skag.h"
#include "GameInstance.h"

NS_BEGIN(Client)

class CSkagState abstract : public CBase
{
public:
	CSkagState(class CSkag* pOwner)
		: m_pOwner(pOwner),
		m_pGameInstance(CGameInstance::Get_Instance())
	{
		//m_pOwnerNavi = m_pOwner->m_pNavigationCom;
		m_pTarget = m_pOwner->m_pTarget;
		m_pTargetTransform = m_pTarget->Get_Transform();
		//Safe_AddRef(m_pGameInstance);
	}
	virtual ~CSkagState() = default;

public:
	virtual void Enter() PURE;
	virtual void Execute(_float fTimeDelta) PURE;
	virtual void Exit() PURE;
	virtual void Free() override {
		__super::Free(); 
		//Safe_Release(m_pGameInstance);
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
			// ���� ���� ��..
			//auto Dist =/* fabs*/(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTarget->Get_Transform()->Get_State(STATE::POSITION))));

			//cout << "Dist : " << Dist << endl;

			m_fTestTick = 0.f;
		}
	}
#endif // _CONSOLE

protected:
	CSkag*			m_pOwner;
	//CNavigation*	m_pOwnerNavi = { nullptr };
	CGameObject*	m_pTarget = { nullptr };
	CTransform*		m_pTargetTransform = { nullptr };

	_float			m_fElapsedTime = {};
	CGameInstance*	m_pGameInstance = { nullptr };
	_bool			m_bChangeStateCall = { false };

	_float			m_fTestTick = {};

	const _float	m_fSkillCheck = 5.f;
};


/*
	Idle - Patrol - (������ �ȴٸ�)Perch
	  �� Provoked -> Provoked_Idle (���� �߿� ����) (Idle���¿����� ���� ��Ÿ�� üũ �� state �纯��)
							 �� (�Ÿ� ��� ���� ���� �б�) Attack_Charge, Run, Leap
							 �� Run -> RunBite / RunTongue
							 �� (��������� �� �߰Ÿ����� �߰� �б� - �ѹ� üũ �� Exit�� �� bool�ʱ�ȭ) Attack_Leap
							 �� (�ٰŸ����� ������ ���� �� ����) Attack_Bite, Attack_Claw
	  �� �ǰݽ� Ȯ�� ������ Dodge? (�ϴ� x)
	  �� �̷� �ù�

	  �� ������ ������?

	  �⺻������ Run���� ���� ��Ÿ����� �ٰ��� �� ����
	  ���� �� ����
	  leap�� ���� �������� �˻��ϴ��� �� �𸣰��� �ٵ� �ѹ��� üũ �� false�� �� ���������ϰ� run
	  charge�� �� �� ���¿��� �ѹ� �˻�?
	  ������ �ְ� �ѹ� �ɸ��� ������ �ѹ��� ������ ����..
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

		// �ٷιٷ� ��ȯ ���ִ� �뵵�� ��ġ�⸸ �ص� �ɵ�?
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);
		m_fCooldown += fTimeDelta;
		if (m_fCooldown > 0.5f)
		{
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

// ChargeStart�� �и��ϸ� ���� �� ��������..
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
				//XMStoreFloat3(&vChargePos, m_pTargetTransform->Get_State(STATE::POSITION));
			}
		}
		if (ChargeStatus[1] == false)
		{   // �غ� �߿� �� ������
			m_pOwner->m_pTransformCom->LookAtLerp(m_pTargetTransform->Get_State(STATE::POSITION), fTimeDelta, 8.f);
		}
		if (ChargeStatus[1] == true && ChargeStatus[2] == false)
		{
			fChargingTime += fTimeDelta; // �ּ� ���� �ð� ��Ƶα�
			if (fChargingTime > fForceChargingTime)
			{
				auto Dist = fabs(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTargetTransform->Get_State(STATE::POSITION))));
				if (Dist >= 10.f || fChargingTime > 4.f)
				{
					ChargeStatus[2] = true;
					Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Charge_Stop, false);
				}
			}
			//m_pOwner->m_pTransformCom->Go_Target(XMVectorSetW(XMLoadFloat3(&vChargePos), 1.f), fTimeDelta * 1.2f, 1.f);
			m_pOwner->m_pTransformCom->LookAtLerp(m_pTargetTransform->Get_State(STATE::POSITION), fTimeDelta, 2.5f);

			m_pOwner->m_pTransformCom->Go_Straight(fTimeDelta * 2.f);
		}
		if (ChargeStatus[2] == true)
		{
			//m_pOwner->m_pTransformCom->Go_Target(XMVectorSetW(XMLoadFloat3(&vChargePos), 1.f), fTimeDelta * fDecel, 1.f);
			m_pOwner->m_pTransformCom->Go_Straight(fTimeDelta * fDecel);
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
				Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Leap_End, false);
			}
			else if (LeapStatus[0] == true)
			{
				LeapStatus[1] = true;
				Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Leap_Idle, false);
				//XMStoreFloat3(&vLeapPos, m_pTargetTransform->Get_State(STATE::POSITION));
			}
		}
		if (LeapStatus[2] == true)
		{
			m_pOwner->m_pTransformCom->Go_Straight(fTimeDelta * fDecel);
			fDecel *= 0.95f;
		}
		else if (LeapStatus[1] == true)
		{
			m_pOwner->m_pTransformCom->Go_Target(m_pTargetTransform->Get_State(STATE::POSITION), fTimeDelta * 3.f, 1.f);
			m_pOwner->m_pTransformCom->LookAtLerp(m_pTargetTransform->Get_State(STATE::POSITION), fTimeDelta, 8.f);
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
		Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Run_Bite, false);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked_Idle);
		}
		m_pOwner->m_pTransformCom->Go_Straight(fTimeDelta);
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
		Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Run_Tongue, false);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked_Idle);
		}
		m_pOwner->m_pTransformCom->Go_Straight(fTimeDelta);

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
	}
	virtual void Execute(_float fTimeDelta) override
	{
		Console(fTimeDelta);
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
		if (Dist < m_pOwner->m_fAttackableDistance)
		{
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Attack_Run_Bite);
			return;
				//m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Attack_Tongue);
		}

		m_pOwner->m_pTransformCom->LookAtLerp(m_pTarget->Get_Transform()->Get_State(STATE::POSITION), fTimeDelta, 8.f);
		m_pOwner->m_pTransformCom->Go_Straight(/*m_pTargetTransform->Get_State(STATE::POSITION), */fTimeDelta/*, 1.f*/);
		
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }

private:
	const _float m_fChargeDist = { 50.f };
	const _float m_fLeapDist = { 30.f };
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
