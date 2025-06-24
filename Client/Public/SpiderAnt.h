#pragma once

#include "Monster.h"


NS_BEGIN(Client)

class CSpiderAnt final : public CMonster
{
public:
	typedef struct tagSpiderAntDesc : public CMonster::DESC
	{

	}DESC;

	enum SPIDERANT_ANIM {
		NONE0, NONE1, NONE2, NONE3, 
		Attack_1Shot, Attack_3Shot, Attack_6Shot,
		NONE4, NONE5, NONE6,
		Attack_Charge_End, Attack_Charge_Hitwall, Attack_Charge_Loop, Attack_Charge_Start, Attack_Charge_Strike,
		Attack_Claw_Both, Attack_Claw_L, Attack_Claw_L2,
		NONE7,
		Burrow_Enter, Burrow_Exit,
		NONE8, NONE9,
		Death_Critical,
		NONE10, NONE11, NONE12, NONE13,
		Death_Var1, Death_Var2, Death_Var3,
		NONE14, NONE15,
		Hardflinch_Head,
		NONE16, NONE17, NONE18,
		Idle_Ready, Idle_Turn180L, Idle_Turn90L, Idle_Turn45L,
		NONE19, NONE20, NONE21, NONE22, NONE23, NONE24, NONE25,
		Leap_End, Leap_Idle, Leap_Start,
		Patrol_F, Patrol_Turn180L, Patrol_Turn90L,
		NONE26, NONE27, NONE28,
		PhaseLock_Fall, PhaseLock_Land, PhaseLock_Lift, PhaseLock_Loop,
		Run_B, Run_F1, Run_F2, Run_L, Run_R, Run_Turn180L, Run_Turn90L,
		NONE29, NONE30, NONE31, NONE32, NONE33, NONE34, NONE35,

		SPIDERANT_ANIM_END
	};

	enum SPIDERANT_STATE {
		STATE_Attack_Charge, STATE_Attack_Claw, STATE_Attack_Leap,
		STATE_Attack_Shot1, STATE_Attack_Shot3, STATE_Attack_Shot6, STATE_Leap,
		STATE_Death, STATE_Hardflinch, STATE_Idle, STATE_Injured_Walk, STATE_Knockback, 
		STATE_Patrol, STATE_PhaseLocked, STATE_Provoked, STATE_Provoked_Idle, STATE_Run,
		STATE_Burrow,
		STATE_END
	};

#pragma region MyFriendMyState

	friend class CSpiderAntState;
	friend class CSpiderAntState_Idle;
	friend class CSpiderAntState_Patrol;
	friend class CSpiderAntState_Attack_Shot1;
	friend class CSpiderAntState_Attack_Shot3;
	friend class CSpiderAntState_Attack_Shot6;
	friend class CSpiderAntState_Attack_Claw;
	friend class CSpiderAntState_Attack_Leap;
	friend class CSpiderAntState_Attack_Charge;
	friend class CSpiderAntState_Attack_Charge_HitWall;
	friend class CSpiderAntState_Attack_Charge_Strike;
	//friend class CSpiderAntState_Provoked;
	friend class CSpiderAntState_Provoked_Idle;
	friend class CSpiderAntState_Phaselocked;/**/
	friend class CSpiderAntState_Hardflinch_Head; /* 머리 맞았을 때 넉백만 추가 */
	friend class CSpiderAntState_Run;
	friend class CSpiderAntState_Dead;
	//friend class
#pragma endregion

private:
	CSpiderAnt(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpiderAnt(const CSpiderAnt& Prototype);
	virtual ~CSpiderAnt() = default;

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

	void Set_State(SPIDERANT_STATE eState);
	void Update_State(_float fTimeDelta);
	virtual void Set_State_Dead();

	HRESULT Spawn_SpitBullet();

private:
	virtual void On_Collision(_uint iMyColID, _uint iHitColID, CCollider* pHitCol) override;

public:
	virtual void OnHit_Phaselock() override;

private:
	_uint m_iSpineBoneIdx = {};
	_uint m_iTailBoneIdx = {};

private:
	class CSpiderAntState*	m_pCurState = { nullptr };
	class CSpiderAntState*	m_pStates[STATE_END] = { nullptr };
	SPIDERANT_STATE			m_eCurState = { SPIDERANT_STATE::STATE_END };
	SPIDERANT_STATE			m_ePrevState = { SPIDERANT_STATE::STATE_END };

private: // 상태 체크용 부모 변수들
	_bool			m_bLeapCheck = { false };
	_bool           m_bChargeCheck = { false };
	_float			m_fLeapCheckTimer = {};
	_float			m_fChargeCheckTimer = {};
	_float			m_fSkillTimer = {};
	
	void AttackTimer(_float fTimeDelta);


public:
	static CSpiderAnt* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END