#pragma once

#include "Monster.h"


NS_BEGIN(Client)

class CRakk final : public CMonster
{
public:
	typedef struct tagRakkDesc : public CMonster::DESC
	{
		_float3 fSpawnPoint = {};

	}DESC;

	enum RAKK_ANIM {
		Fly01, Fly02, Fly04, Glide01, Launch, Rot180, FlightToWorldIdle, FlightPoseL, FlightPoseR, Flinch, GroundIdle, Hover, Landing1,
		Landing2, Takeoff, WorldIdle1, WorldIdle2, WorldIdle3, WorldIdle4, WorldIdle_Scatter1, WorldIdle_Scatter2, Attack, SuicideDive1,
		SuicideDive2, PhaselockLoop, Flight_BankL, Flight_BankR, RAKK_ANIM_END
	};

	enum RAKK_STATE {
		STATE_Idle, STATE_Hover, STATE_Ground_Idle, STATE_Flying_Idle, STATE_Flying_Attack,
		STATE_Flying_SuicidalDive, STATE_PhaseLocked, STATE_Takeoff, STATE_Landing, STATE_Provoked, STATE_Dead, STATE_END
	};

#pragma region MyFriendMyState

	friend class CRakkState;
	friend class CRakkState_Idle; //??
	friend class CRakkState_Flying_Idle;
	friend class CRakkState_Provoked;
	friend class CRakkState_Hover;
	friend class CRakkState_Ground_Idle;
	friend class CRakkState_Attack;
	friend class CRakkState_SuicideDive;
	friend class CRakkState_Phaselocked;
	friend class CRakkState_Takeoff;
	friend class CRakkState_Landing;
	friend class CRakkState_Dead;
	//friend class
#pragma endregion

private:
	CRakk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRakk(const CRakk& Prototype);
	virtual ~CRakk() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_RakkStates();

	void Set_State(RAKK_STATE eState);
	void Update_State(_float fTimeDelta);
	virtual void Set_State_Dead();

private:
	_bool IsOutsideRegion() const;
	void Enforce_ReturnToRegion(_float fTimeDelta);

private:
	_float3		m_vRegionCenter = {0.f, 0.f, 0.f};
	_float		m_fRegionRadius = { 50.f };
	_bool		m_isOutsideRegion = { false };

	// ── Wander 관련 ──
	_float       m_fMaxTurnAngle = {XMConvertToRadians(45.f)};        // 최대 회전 가능 각도 (라디안, 초당)
	_float       m_fWanderChangeInterval = {1.f};// Wander 방향을 재계산할 주기(초 단위)
	_float       m_fTimeSinceLastWander = {}; // 마지막으로 Wander 방향을 변경한 이후 누적된 시간

	_float		 m_fRotatingDegree = {};

private:
	class CRakkState*	m_pCurState = { nullptr };
	class CRakkState*	m_pStates[STATE_END] = { nullptr };
	RAKK_STATE			m_eCurState = { RAKK_STATE::STATE_END };
	RAKK_STATE			m_ePrevState = { RAKK_STATE::STATE_END };

public:
	static CRakk* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END