#pragma once

#include "Leviathan.h"
#include "GameInstance.h"
#include "Player.h"
#include "BossSmokeParticle.h"
#include "Levi_Bullet.h"

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
		//cout << " Boss State: Engage" << endl;

		Set_OwnerAnim(CLeviathan::LEVI_ANIM::Roar, false);
		m_pOwner->m_pSoundCom->Play("Boss_roar");
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
		//m_pOwner->m_pUI->Set_Active(true);

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
		//cout << " Boss State: Idle" << endl;

		if (m_pOwner->m_bInjured)
			Set_OwnerAnim(CLeviathan::LEVI_ANIM::idle_injured, true);
		else
			Set_OwnerAnim(CLeviathan::LEVI_ANIM::Idle, true);

		m_fCooldown = 0.f;
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);

		if (KEY_DOWN(DIK_1))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Attack_SpitWorm);
		}
		if (KEY_DOWN(DIK_2))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Attack_ThrowRock);
		}
		if (KEY_DOWN(DIK_3))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Attack_Spray);
		}
		if (KEY_DOWN(DIK_4))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Attack_Slam);
		}
		m_fCooldown += fTimeDelta;

		if (m_fCooldown >= 2.f)
		{
			switch (rand() % 4)
			{
			case 0:
				m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Attack_Slam);
				break;
			case 1:
				m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Attack_SpitWorm);
				break;
			case 2:
				//m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Attack_Spray);
				break;
			case 3:
				m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Attack_ThrowRock);
				break;
			default:
				break;
			}
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }

private:
	_float m_fCooldown = {};
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
		//cout << " Boss State: Attack_Spray" << endl;

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

		_float fTrackPos = m_pOwner->m_pModelCom->Get_CurrentTrackPosition();

		if (30.f <= fTrackPos && 80.f >= fTrackPos)
		{
			m_pOwner->m_pColliderGroundAttackCom->Set_Active(true);
		}
		if(80.f < fTrackPos)
		{
			m_pOwner->Set_FirstHit_True();
			m_pOwner->m_pColliderGroundAttackCom->Set_Active(false);
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
		//cout << " Boss State: Attack_Rock" << endl;
		if (m_pOwner->m_bInjured)
			Set_OwnerAnim(CLeviathan::LEVI_ANIM::Injured_rock_throw, false);
		else
			Set_OwnerAnim(CLeviathan::LEVI_ANIM::Rock_Throw, false);
		m_bMakeBullet = false;
		m_bLaunchBullet = false;
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Idle);
		}
		_float fTrackPos = m_pOwner->m_pModelCom->Get_CurrentTrackPosition();
		if (23.f <= fTrackPos && m_bMakeBullet == false)
		{
			m_pOwner->m_pSoundCom->Play("rockbreaklong");
			m_pOwner->Spawn_Bullet(false);
			m_bMakeBullet = true;
		}
		if (95.f <= fTrackPos && m_bLaunchBullet == false)
		{
			CGameObject* pBullet = m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::BOSS), TEXT("Layer_MonBullet"), 0);
			if (nullptr != pBullet)
			{
				_float3 vTargetPos = {};
				XMStoreFloat3(&vTargetPos, m_pTarget->Get_Transform()->Get_State(STATE::POSITION));
				static_cast<CLevi_Bullet*>(pBullet)->Launch_Projectile(vTargetPos, 70.f);
				m_bLaunchBullet = true;
			}
		}

		// 23에 생성
		// 95에 던지기
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
private:
	_bool m_bMakeBullet = { false };
	_bool m_bLaunchBullet = { false };
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
		//cout << " Boss State: Attack_slam" << endl;
		m_bSlam = false;
		if (m_pOwner->m_bInjured)
			Set_OwnerAnim(CLeviathan::LEVI_ANIM::Injured_tongue_ground_slam, false);
		else
			rand() % 2? 
			Set_OwnerAnim(CLeviathan::LEVI_ANIM::Tongue_ground_slam, false):
			Set_OwnerAnim(CLeviathan::LEVI_ANIM::ground_slam, false); // tongue는....?음
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Idle);
		}

		//79
		_float fTrackPos = m_pOwner->m_pModelCom->Get_CurrentTrackPosition();
		if (79.f <= fTrackPos && m_bSlam == false)
		{
			m_pOwner->m_pColliderGroundAttackCom->Set_Active(true);
			static_cast<CPlayer*>(m_pTarget)->Set_State(CPlayer::STATE_Jump);

			CBossSmokeParticle::DESC desc = {};
			desc.iLevelID = ENUM_CLASS(LEVEL::STATIC);
			desc.bHasTransformPreset = true;
			XMStoreFloat4x4(&desc.PresetMatrix, XMMatrixTranslation(128.f, 104.f, 106.f));
			if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_BossSmokeParticle"),
				ENUM_CLASS(LEVEL::STATIC), L"Layer_Effect", &desc)))
				return;

			rand() % 2 ? m_pOwner->m_pSoundCom->Play("slam1") : m_pOwner->m_pSoundCom->Play("slam2");
			m_bSlam = true;
		}
		if (99.f <= fTrackPos)
		{
			m_pOwner->Set_FirstHit_True();
			m_pOwner->m_pColliderGroundAttackCom->Set_Active(false);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
private:
	_bool m_bSlam = { false };
	
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
		//cout << " Boss State: Attack_Spitworm" << endl;

		Set_OwnerAnim(CLeviathan::LEVI_ANIM::Spit_worm, false);
		m_bSpawnBullet = false;
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Idle);
		}
		//50
		_float fTrackPos = m_pOwner->m_pModelCom->Get_CurrentTrackPosition();
		if (50.f <= fTrackPos && m_bSpawnBullet == false)
		{
			//m_pOwner->m_pColliderGroundAttackCom->Set_Active(true);
			m_pOwner->Spawn_Bullet();
			m_bSpawnBullet = true;
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
public:
	_bool m_bSpawnBullet = { false };
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
		//cout << " Boss State: Damaged" << endl;

		if (m_pOwner->m_bInjured)
			Set_OwnerAnim(CLeviathan::LEVI_ANIM::Injured_damage, false);
		else
			Set_OwnerAnim(CLeviathan::LEVI_ANIM::Damage, false);
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

#define STUN_START	0
#define STUN_IDLE	1
#define STUN_END	2
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
		//cout << " Boss State: stun_left" << endl;

		Set_OwnerAnim(CLeviathan::LEVI_ANIM::lefteye_start, false);
		m_iStunStatus = STUN_START;
		m_fStunTicker = 0.f;
		m_pOwner->m_pSoundCom->Play("roar1");
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			switch (m_iStunStatus)	
			{
			case STUN_START:
				m_iStunStatus = STUN_IDLE;
				Set_OwnerAnim(CLeviathan::LEVI_ANIM::idle_lefteye, true);
				break;
			case STUN_IDLE:
				break;
			case STUN_END:
				m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Idle);
				break;
			default:
				break;
			}
		}

		if (m_iStunStatus == STUN_IDLE)
		{
			m_fStunTicker += fTimeDelta;
			if (m_fStunTicker >= 3.f)
			{
				Set_OwnerAnim(CLeviathan::LEVI_ANIM::lefteye_end, false);
				m_iStunStatus = STUN_END;
			}
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }

private:
	_float	m_fStunTicker = {};
	_uint	m_iStunStatus = {};
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
		//cout << " Boss State: stun_right" << endl;

		Set_OwnerAnim(CLeviathan::LEVI_ANIM::righteye_start, false);
		m_iStunStatus = STUN_START;
		m_fStunTicker = 0.f;
		m_pOwner->m_pSoundCom->Play("roar1");
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			switch (m_iStunStatus)
			{
			case STUN_START:
				m_iStunStatus = STUN_IDLE;
				Set_OwnerAnim(CLeviathan::LEVI_ANIM::idle_righteye, true);
				break;
			case STUN_IDLE:
				break;
			case STUN_END:
				m_pOwner->Set_State(CLeviathan::LEVI_STATE::STATE_Idle);
				break;
			default:
				break;
			}
		}

		if (m_iStunStatus == STUN_IDLE)
		{
			m_fStunTicker += fTimeDelta;
			if (m_fStunTicker >= 3.f)
			{
				Set_OwnerAnim(CLeviathan::LEVI_ANIM::righteye_end, false);
				m_iStunStatus = STUN_END;
			}
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }

private:
	_float	m_fStunTicker = {};
	_uint	m_iStunStatus = {};
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
		//cout << " Boss State: lookaround" << endl;

		rand() % 3 == 0 ? Set_OwnerAnim(CLeviathan::LEVI_ANIM::Idle_tongue_out_look_around, false) :
						  Set_OwnerAnim(CLeviathan::LEVI_ANIM::Idle_look_around, false);

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
		//cout << "[Dead]" << endl;
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CLeviathan::LEVI_ANIM::Death), false);
		m_pOwner->m_pModelCom->Set_Animation_TickPerSecond(ENUM_CLASS(CLeviathan::LEVI_ANIM::Death), 15.f);
		m_pOwner->m_pSoundCom->Play("roar3");
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (m_pOwner->m_bDead == true)
			return;
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_Dead(); 
			//흠! 일단 빼보고...
			return;
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};







NS_END