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
		m_pTarget = m_pOwner->m_pTarget;
		Safe_AddRef(m_pGameInstance);
	}
	virtual ~CSkagState() = default;

public:
	virtual void Enter() PURE;
	virtual void Execute(_float fTimeDelta) PURE;
	virtual void Exit() PURE;
	virtual void Free() override { __super::Free(); Safe_Release(m_pGameInstance); }

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
			/* �� ���� ¥������ �ڵ带 */
			/* ������ ���� ���� ������ �� ������ */
			/* ���ʹ� Idle���¶� Provoked���� ū Ʋ�� �з��� ���� ������ Provoke���� ���� �б⸦ ������ �ɷ� */
			/* Provoke������ �ٵ�..�÷��̾����� ���� ������? */
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
			auto Dist =/* fabs*/(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTarget->Get_Transform()->Get_State(STATE::POSITION))));

			cout << "Dist : " << Dist << endl;
			m_fTestTick = 0.f;
		}
	}
#endif // _CONSOLE

protected:
	CSkag*			m_pOwner;
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
		Set_OwnerAnim(CSkag::SKAG_ANIM::Idle, true);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (Is_Target_Found())
		{
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked);
			return;
		}

		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			return;
		}

		m_pOwner->m_pTransformCom->Go_Straight(fTimeDelta);


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
		rand() % 2 ?
			Set_OwnerAnim(CSkag::SKAG_ANIM::Provoked_var1, true) :
			Set_OwnerAnim(CSkag::SKAG_ANIM::Provoked_var2, true);
	}
	virtual void Execute(_float fTimeDelta) override
	{

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
		//Set_OwnerAnim(CSkag::SKAG_ANIM::, true);
		// �ٷιٷ� ��ȯ ���ִ� �뵵�� ��ġ�⸸ �ص� �ɵ�?
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (Is_Target_Attackable())
		{

		}
		else
		{
			
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
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
		rand() % 2 ?
			Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Bite1, true) :
			Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Bite2, true);

	}
	virtual void Execute(_float fTimeDelta) override
	{

	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

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
		Set_OwnerAnim(CSkag::SKAG_ANIM::Attack_Charge_Start, true);
	}
	virtual void Execute(_float fTimeDelta) override
	{

	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

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

	}
	virtual void Execute(_float fTimeDelta) override
	{

	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
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

	}
	virtual void Execute(_float fTimeDelta) override
	{

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

	}
	virtual void Execute(_float fTimeDelta) override
	{

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
		rand() % 2 ?( rand() % 2 ?
		Set_OwnerAnim(CSkag::SKAG_ANIM::Run_F, true) :
		Set_OwnerAnim(CSkag::SKAG_ANIM::Run_F3, true)) :
		Set_OwnerAnim(CSkag::SKAG_ANIM::Run_F_Big1, true);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		auto Dist = fabs(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTargetTransform->Get_State(STATE::POSITION))));

		m_pOwner->AttackTimer(fTimeDelta);

		if (m_pOwner->m_bChargeCheck)
		{
			m_pOwner->m_fChargeCheckTimer = 0.f;
			m_pOwner->m_bChargeCheck = false;
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Attack_Charge);
			return;
		}
		if (m_pOwner->m_bLeapCheck)
		{
			m_pOwner->m_fLeapCheckTimer = 0.f;
			m_pOwner->m_bLeapCheck = false;
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Attack_Leap);
			return;
		}

		
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
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
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CSkag::SKAG_ANIM::PhaseLock_Loop), false);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_Dead();
			return;
		}
		m_pOwner->m_pTransformCom->Go_Down(fTimeDelta);
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

NS_END;
