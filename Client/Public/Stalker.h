#pragma once

#include "Monster.h"


NS_BEGIN(Client)

class CStalker final : public CMonster
{
public:
	typedef struct tagStalkerDesc : public CMonster::DESC
	{
		//_float3 fSpawnPoint = {};

	}DESC;

	enum STALKER_ANIM {
		Fly01, Fly02, Fly04, Glide01, Launch, Rot180, FlightToWorldIdle, FlightPoseL, FlightPoseR, Flinch, GroundIdle, Hover, Landing1,
		Landing2, Takeoff, WorldIdle1, WorldIdle2, WorldIdle3, WorldIdle4, WorldIdle_Scatter1, WorldIdle_Scatter2, Attack, SuicideDive1,
		SuicideDive2, PhaselockLoop, Flight_BankL, Flight_BankR, STALKER_ANIM_END
	};

	enum STALKER_STATE {
		STATE_Idle, STATE_Hover, STATE_Ground_Idle, STATE_Flying_Idle, STATE_Flying_Attack,
		STATE_Flying_SuicidalDive, STATE_PhaseLocked, STATE_Takeoff, STATE_Landing, STATE_Provoked, STATE_Dead, STATE_END
	};

#pragma region MyFriendMyState

	friend class CStalkerState;
	friend class CStalkerState_Idle; //??
	friend class CStalkerState_Dead;
	//friend class
#pragma endregion

private:
	CStalker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStalker(const CStalker& Prototype);
	virtual ~CStalker() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_StalkerStates();

	void Set_State(STALKER_STATE eState);
	void Update_State(_float fTimeDelta);
	virtual void Set_State_Dead();


private:
	class CStalkerState*	m_pCurState = { nullptr };
	class CStalkerState*	m_pStates[STATE_END] = { nullptr };
	STALKER_STATE			m_eCurState = {  STALKER_STATE::STATE_END };
	STALKER_STATE			m_ePrevState = { STALKER_STATE::STATE_END };

public:
	static CStalker* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END