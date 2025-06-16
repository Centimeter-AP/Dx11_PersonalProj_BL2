#pragma once

#include "SpiderAnt.h"
#include "GameInstance.h"

NS_BEGIN(Client)

class CSpiderAntState abstract : public CBase
{
public:
	CSpiderAntState(class CSpiderAnt* pOwner)
		: m_pOwner(pOwner),
		m_pGameInstance(CGameInstance::Get_Instance())
	{
		m_pTarget = m_pOwner->m_pTarget;
		m_pTargetTransform = m_pTarget->Get_Transform();
		Safe_AddRef(m_pGameInstance);
	}
	virtual ~CSpiderAntState() = default;

public:
	virtual void Enter() PURE;
	virtual void Execute(_float fTimeDelta) PURE;
	virtual void Exit() PURE;
	virtual void Free() override { __super::Free(); Safe_Release(m_pGameInstance); }

	virtual _bool Is_Target_Found() {
		auto Dist = fabs(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTargetTransform->Get_State(STATE::POSITION))));
		return (Dist < m_pOwner->m_fDetectiveDistance);
	}
	virtual _bool Is_Target_Attackable() {
		auto Dist = fabs(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTargetTransform->Get_State(STATE::POSITION))));
		return (Dist < m_pOwner->m_fAttackableDistance);
	}
	virtual void Detect_Target() {
		if (Is_Target_Found())
		{
			m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Provoked);
		}
	}
	virtual void Call_State(CSpiderAnt::SPIDERANT_STATE eNextState) {
		//if (m_)
	}
	void Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM eAnim, _bool isLoop)
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
	CSpiderAnt*		m_pOwner;
	CGameObject*	m_pTarget = { nullptr };
	CTransform*		m_pTargetTransform = { nullptr };

	_float			m_fElapsedTime = {};
	CGameInstance*	m_pGameInstance = { nullptr };
	_bool			m_bChangeStateCall = { false };

	_float			m_fTestTick = {};

	const _float	m_fSkillCheck = 5.f;
};

class CSpiderAntState_Idle final : public CSpiderAntState
{
public:
	CSpiderAntState_Idle(class CSpiderAnt* pOwner)
		: CSpiderAntState(pOwner) {}
	virtual ~CSpiderAntState_Idle() = default;

public:
	virtual void Enter() override
	{
		cout << "Idle" << endl;
		Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Idle_Ready, true);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		//Console(fTimeDelta);
		m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);
		if (Is_Target_Found())
		{
			m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Provoked_Idle);
			return;
		}

	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};


class CSpiderAntState_Patrol final : public CSpiderAntState
{
public:
	CSpiderAntState_Patrol(class CSpiderAnt* pOwner)
		: CSpiderAntState(pOwner) {
	}
	virtual ~CSpiderAntState_Patrol() = default;

public:
	virtual void Enter() override
	{
		cout << "patrol" << endl;
		Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Patrol_F, true);
	}
	virtual void Execute(_float fTimeDelta) override
	{

	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

//class CSpiderAntState_Provoked final : public CSpiderAntState
//{
//public:
//	CSpiderAntState_Provoked(class CSpiderAnt* pOwner)
//		: CSpiderAntState(pOwner) {
//	}
//	virtual ~CSpiderAntState_Provoked() = default;
//
//public:
//	virtual void Enter() override
//	{
//		cout << "provoked" << endl;
//		rand() % 2 ?
//			Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::, false) :
//			Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::, false);
//	}
//	virtual void Execute(_float fTimeDelta) override
//	{
//		Console(fTimeDelta);
//		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
//		{
//			m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Provoked_Idle);
//		}
//	}
//	virtual void Exit() override
//	{
//
//	}
//	virtual void Free() override { __super::Free(); }
//};

class CSpiderAntState_Provoked_Idle final : public CSpiderAntState
{
public:
	CSpiderAntState_Provoked_Idle(class CSpiderAnt* pOwner)
		: CSpiderAntState(pOwner) {
	}
	virtual ~CSpiderAntState_Provoked_Idle() = default;

public:
	virtual void Enter() override
	{
		cout << "provoked_idle" << endl;
		Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Idle_Ready, true);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		//Console(fTimeDelta);
		m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);

		//if (Is_Target_Attackable())
		//{
		//	m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Attack_Claw);
		//}
		//else
		//{
		//	m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Run);
		//}
		if (KEY_DOWN(DIK_1))
		{
			m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Attack_Leap);
		}
		if (KEY_DOWN(DIK_2))
		{
			m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Attack_Charge);
		}
		if (KEY_DOWN(DIK_3))
		{
			m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Attack_Shot1);
		}
		if (KEY_DOWN(DIK_4))
		{
			m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Attack_Shot3);
		}
		if (KEY_DOWN(DIK_5))
		{
			m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Attack_Shot6);
		}


	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};



class CSpiderAntState_Attack_Leap final : public CSpiderAntState
{
public:
	CSpiderAntState_Attack_Leap(class CSpiderAnt* pOwner)
		: CSpiderAntState(pOwner) {
	}
	virtual ~CSpiderAntState_Attack_Leap() = default;

public:
	virtual void Enter() override
	{
		cout << "a_Leap" << endl;
		Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Leap_Start, false);
		LeapStatus[0] = true;
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			if (LeapStatus[2] == true)
				m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Provoked_Idle);
			else if (LeapStatus[1] == true)
			{
				LeapStatus[2] = true;
				Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Leap_End, false);
			}
			else if (LeapStatus[0] == true)
			{
				LeapStatus[1] = true;
				Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Leap_Idle, false);
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
			auto Dist = fabs(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTargetTransform->Get_State(STATE::POSITION))));

			m_pOwner->m_pTransformCom->Go_Target(m_pTargetTransform->Get_State(STATE::POSITION), fTimeDelta * Dist * 0.8f, 1.f);
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


// ChargeStart도 분리하면 좋을 것 같은데요..
class CSpiderAntState_Attack_Charge final : public CSpiderAntState
{
public:
	CSpiderAntState_Attack_Charge(class CSpiderAnt* pOwner)
		: CSpiderAntState(pOwner) {
	}
	virtual ~CSpiderAntState_Attack_Charge() = default;
	//charge
	//	start - loop - (Stop / Strike / hitwall)
public:
	virtual void Enter() override
	{
		cout << "a_charge" << endl;
		Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Attack_Charge_Start, false);
		ChargeStatus[0] = true;
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			if (ChargeStatus[2] == true)
				m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Provoked_Idle);
			else if (ChargeStatus[0] == true)
			{
				ChargeStatus[1] = true;
				Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Attack_Charge_Loop, true);
				//XMStoreFloat3(&vChargePos, m_pTargetTransform->Get_State(STATE::POSITION));
			}
		}
		if (ChargeStatus[1] == false)
		{   // 준비 중에 고개 돌리기
			m_pOwner->m_pTransformCom->LookAtLerp(m_pTargetTransform->Get_State(STATE::POSITION), fTimeDelta, 8.f);
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
					Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Attack_Charge_End, false);
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
	_float fDecel = { 0.9f };
	_float fChargingTime = {};
	const _float fForceChargingTime = { 2.f };
};

class CSpiderAntState_Attack_Charge_HitWall final : public CSpiderAntState
{
public:
	CSpiderAntState_Attack_Charge_HitWall(class CSpiderAnt* pOwner)
		: CSpiderAntState(pOwner) {
	}
	virtual ~CSpiderAntState_Attack_Charge_HitWall() = default;

public:
	virtual void Enter() override
	{
		cout << "a_Charge_HitWall" << endl;
		Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Attack_Charge_Hitwall, false);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Provoked_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CSpiderAntState_Attack_Charge_Strike final : public CSpiderAntState
{
public:
	CSpiderAntState_Attack_Charge_Strike(class CSpiderAnt* pOwner)
		: CSpiderAntState(pOwner) {
	}
	virtual ~CSpiderAntState_Attack_Charge_Strike() = default;

public:
	virtual void Enter() override
	{
		cout << "a_Charge_Strike" << endl;
		Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Attack_Charge_Strike, false);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Provoked_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};


class CSpiderAntState_Attack_Shot1 final : public CSpiderAntState
{
public:
	CSpiderAntState_Attack_Shot1(class CSpiderAnt* pOwner)
		: CSpiderAntState(pOwner) {
	}
	virtual ~CSpiderAntState_Attack_Shot1() = default;

public:
	virtual void Enter() override
	{
		cout << "a_Shot1" << endl;
		Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Attack_1Shot, false);
		bSpawnBullet = false;

	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Provoked_Idle);
		}
		auto test = m_pOwner->m_pModelCom->Get_CurrentTrackPosition();
		if (test >= 30.f)
			return;
		if (20.f < test && bSpawnBullet == false)
		{
			bSpawnBullet = true;
			m_pOwner->Spawn_SpitBullet();
		}

	}
	virtual void Exit() override
	{
		bSpawnBullet = false;
	}
	virtual void Free() override { __super::Free(); }

private:
	_bool bSpawnBullet = { false };
};

class CSpiderAntState_Attack_Shot3 final : public CSpiderAntState
{
public:
	CSpiderAntState_Attack_Shot3(class CSpiderAnt* pOwner)
		: CSpiderAntState(pOwner) {
	}
	virtual ~CSpiderAntState_Attack_Shot3() = default;

public:
	virtual void Enter() override
	{
		cout << "a_Shot3" << endl;
		Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Attack_3Shot, false);
		for (auto& bSpawn : bSpawnBullet)
			bSpawn = false;
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Provoked_Idle);
		}
		auto test = m_pOwner->m_pModelCom->Get_CurrentTrackPosition();
		if (test >= 30.f )
			return;
		if (18.f < test && bSpawnBullet[0] == false)
		{
			bSpawnBullet[0] = true;
			m_pOwner->Spawn_SpitBullet();
		}
		if (21.f < test && bSpawnBullet[1] == false)
		{
			bSpawnBullet[1] = true;
			m_pOwner->Spawn_SpitBullet();
		}
		if (25.f < test && bSpawnBullet[2] == false)
		{
			bSpawnBullet[2] = true;
			m_pOwner->Spawn_SpitBullet();
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }

private:
	_bool bSpawnBullet[3] = {false};
};


class CSpiderAntState_Attack_Shot6 final : public CSpiderAntState
{
public:
	CSpiderAntState_Attack_Shot6(class CSpiderAnt* pOwner)
		: CSpiderAntState(pOwner) {
	}
	virtual ~CSpiderAntState_Attack_Shot6() = default;

public:
	virtual void Enter() override
	{
		cout << "a_Shot6" << endl;
		Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Attack_6Shot, false);
		for (auto& bSpawn : bSpawnBullet)
			bSpawn = false;
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Provoked_Idle);
		}
		auto test = m_pOwner->m_pModelCom->Get_CurrentTrackPosition();
		if (test >= 50.f)
			return;
		if (25.f < test && bSpawnBullet[0] == false)
		{ // 25 30 34 37 42 48
			bSpawnBullet[0] = true;
			m_pOwner->Spawn_SpitBullet();
		}
		if (30.f < test && bSpawnBullet[1] == false)
		{
			bSpawnBullet[1] = true;
			m_pOwner->Spawn_SpitBullet();
		}
		if (34.f < test && bSpawnBullet[2] == false)
		{
			bSpawnBullet[2] = true;
			m_pOwner->Spawn_SpitBullet();
		}
		if (37.f < test && bSpawnBullet[3] == false)
		{
			bSpawnBullet[3] = true;
			m_pOwner->Spawn_SpitBullet();
		}
		if (42.f < test && bSpawnBullet[4] == false)
		{
			bSpawnBullet[4] = true;
			m_pOwner->Spawn_SpitBullet();
		}
		if (48.f < test && bSpawnBullet[5] == false)
		{
			bSpawnBullet[5] = true;
			m_pOwner->Spawn_SpitBullet();
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
private:
	_bool bSpawnBullet[6] = { false };
};



class CSpiderAntState_Attack_Claw final : public CSpiderAntState
{
public:
	CSpiderAntState_Attack_Claw(class CSpiderAnt* pOwner)
		: CSpiderAntState(pOwner) {
	}
	virtual ~CSpiderAntState_Attack_Claw() = default;

public:
	virtual void Enter() override
	{
		cout << "a_claw" << endl;
		Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Attack_Claw_L, false);

	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Provoked_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};


class CSpiderAntState_Run final : public CSpiderAntState
{
public:
	CSpiderAntState_Run(class CSpiderAnt* pOwner)
		: CSpiderAntState(pOwner) {
	}
	virtual ~CSpiderAntState_Run() = default;

public:
	virtual void Enter() override
	{
		cout << "run" << endl;
		rand() % 2 ?
			Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Run_F1, true) :
			Set_OwnerAnim(CSpiderAnt::SPIDERANT_ANIM::Run_F2, true);
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
				m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Attack_Charge);
				return;
			}
		}
		if (Dist < m_fLeapDist)
		{
			if (m_pOwner->m_bLeapCheck)
			{
				m_pOwner->m_fLeapCheckTimer = 0.f;
				m_pOwner->m_bLeapCheck = false;
				m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Attack_Leap);
				return;
			}
		}
		if (Dist < m_pOwner->m_fAttackableDistance)
		{
			//rand() % 2 ?
			//	m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Attack_Run_Bite) :
			//	m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Attack_Run_Tongue);
			//return;
			//m_pOwner->Set_State(CSpiderAnt::SPIDERANT_STATE::STATE_Attack_Tongue);
		}

		m_pOwner->m_pTransformCom->LookAtLerp(m_pTargetTransform->Get_State(STATE::POSITION), fTimeDelta, 8.f);
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




class CSpiderAntState_Dead final : public CSpiderAntState
{
public:
	CSpiderAntState_Dead(class CSpiderAnt* pOwner)
		: CSpiderAntState(pOwner) {
	}
	virtual ~CSpiderAntState_Dead() = default;

public:
	virtual void Enter() override
	{
		cout << "[Dead]" << endl;
		_uint iAnimIdx = {};
		switch (rand()%3)
		{
		case 0:
			iAnimIdx = ENUM_CLASS(CSpiderAnt::SPIDERANT_ANIM::Death_Critical);
			break;
		case 1:
			iAnimIdx = ENUM_CLASS(CSpiderAnt::SPIDERANT_ANIM::Death_Var1);
			break;
		case 2:
			iAnimIdx = ENUM_CLASS(CSpiderAnt::SPIDERANT_ANIM::Death_Var2);
			break;
		default:
			break;
		}
		m_pOwner->m_pModelCom->Set_Animation(iAnimIdx, false);
		m_pOwner->m_pModelCom->Set_Animation_TickPerSecond(iAnimIdx, 10.f);

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
