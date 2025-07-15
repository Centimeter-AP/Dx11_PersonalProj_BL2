#pragma once

#include "Monster.h"


NS_BEGIN(Client)

class CSkag final : public CMonster
{
public:
	typedef struct tagSkagDesc : public CMonster::DESC
	{
		//_float3 fSpawnPoint = {};

	}DESC;
	// 667, 668, 767, 808, 936, 853, 770
	enum SKAG_ANIM {
		NONE0, NONE1, NONE2,
		Attack_Bite1, Attack_Bite2,
		Attack_Charge_Hitwall, Attack_Charge_Loop, Attack_Charge_Start, Attack_Charge_Stop, Attack_Charge_Strike,
		Attack_Claw_L, Attack_Claw_R,
		Attack_ElementBreath, Attack_ElementBreath_LR,
		Attack_Leap_End, Attack_Leap_Idle, Attack_Leap_Start,
		Attack_Radius,
		Attack_Run_Bite, Attack_Run_Tongue,
		Attack_Spit,
		Attack_Tongue1, Attack_Tongue2,
		Cringe_DOTShock, Death_Corrosive, Death_Fire, Death_Head, Death_Shock, Death_var1, Death_var2,
		Dodge_L, GetUp_L, Hardflinch1, Hardflinch2, HardflinchHead,
		Idle, Idle_Injured, Idle_Patrol, Idle_Turn45_L,
		Injured_Walk_B, Injured_Walk_F, Injured_Walk_L, Injured_Walk_R,
		Knockback_Loop, Knockback_Recover,
		Leap_End, Leap_Idle, Leap_Start,
		Patrol_Turn180_L, Patrol_Turn90_L, Patrol_Walk_F,
		Perch_Barf_Loop, Perch_Barf_Start, Perch_Barf_Stop,
		Perch_Pee_End, Perch_Pee_Loop, Perch_Pee_Start,
		Perch_Rolling_End, Perch_Rolling_Loop, Perch_Rolling_Start,
		Perch_Sleep_End, Perch_Sleep_Loop, Perch_Sleep_Start,
		PhaseLock_Fall, PhaseLock_Land, PhaseLock_Lift, PhaseLock_Loop, //2301
		Provoked_var1, Provoked_var2,
		Retreat_Den1, Retreat_Den2,
		Roar_Crit, Roar_Howl,
		Run_F, Run_F3, Run_F_Big1, Run_Turn180_L, Run_Turn90_L,
		Signal_Element_Sent,
		Spawn1, Spawn_Run, Spawn_Drop,
		Patrol_WalkBig_F, Injured_Turn180_L, Injured_Turn90_L,
		SKAG_ANIM_END
	};

	enum SKAG_STATE {
		STATE_Attack_Bite, STATE_Attack_Charge, STATE_Attack_Charge_Strike, STATE_Attack_Claw, STATE_Attack_ElementBreath, STATE_Attack_Leap,
		STATE_Attack_Radius, STATE_Attack_Run_Bite, STATE_Attack_Run_Tongue, STATE_Attack_Spit, STATE_Attack_Tongue,
		STATE_Death, STATE_Dodge, STATE_GetUp, STATE_Hardflinch, STATE_Idle, STATE_Injured_Walk, STATE_Knockback, STATE_Leap,
		STATE_Patrol, STATE_Perch_DONTUSE, STATE_PhaseLocked, STATE_Provoked, STATE_Provoked_Idle, STATE_Retreat, STATE_Roar, STATE_Run,
		STATE_Spawn, STATE_Turn180,

		/*STATE_Idle, STATE_Hover, STATE_Ground_Idle, STATE_Flying_Idle, STATE_Flying_Attack,
		STATE_Flying_SuicidalDive, STATE_PhaseLocked, STATE_Takeoff, STATE_Landing, STATE_Provoked, STATE_Dead, */STATE_END
	};

#pragma region MyFriendMyState

	friend class CSkagState;
	friend class CSkagState_Idle;
	friend class CSkagState_Patrol;
	friend class CSkagState_Attack_Bite;
	friend class CSkagState_Attack_Claw;
	friend class CSkagState_Attack_Tongue;
	friend class CSkagState_Attack_Leap;
	friend class CSkagState_Attack_Charge;
	friend class CSkagState_Attack_Charge_HitWall;
	friend class CSkagState_Attack_Charge_Strike;
	friend class CSkagState_Attack_RunBite;
	friend class CSkagState_Attack_RunTongue;
	friend class CSkagState_Provoked;
	friend class CSkagState_Provoked_Idle;
	friend class CSkagState_Phaselocked;/**/
	friend class CSkagState_Hardflinch_Head; /* 머리 맞았을 때 넉백만 추가 */
	friend class CSkagState_Dodge; /* 예정은 없음 */
	friend class CSkagState_Run;
	friend class CSkagState_Turn180;
	friend class CSkagState_Hit;
	friend class CSkagState_Dead;
	//friend class
#pragma endregion

private:
	CSkag(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkag(const CSkag& Prototype);
	virtual ~CSkag() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_SkagStates();
	virtual void Initialize_BasicStatus(_int iLevel) override;

	void Set_State(SKAG_STATE eState);
	void Update_State(_float fTimeDelta);
	virtual void Set_State_Dead();

private:
	virtual void On_Collision(_uint iMyColID, _uint iHitColID, CCollider* pHitCol) override;

public:
	virtual void OnHit_Phaselock() override;


private:
	_uint m_iSpineBoneIdx = {};

private:
	CCollider*			m_pColliderGroundAttackCom = { nullptr };

	class CSkagState*	m_pCurState = { nullptr };
	class CSkagState*	m_pStates[STATE_END] = { nullptr };
	SKAG_STATE			m_eCurState = { SKAG_STATE::STATE_END };
	SKAG_STATE			m_ePrevState = { SKAG_STATE::STATE_END };

private: // 상태 체크용 부모 변수들
	_bool			m_bLeapCheck = { false };
	_bool           m_bChargeCheck = { false };
	_float			m_fLeapCheckTimer = {};
	_float			m_fChargeCheckTimer = {};
	
	void AttackTimer(_float fTimeDelta);

public:
	static CSkag* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END