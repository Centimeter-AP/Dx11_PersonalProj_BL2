#pragma once

#include "Monster.h"


NS_BEGIN(Client)

class CLeviathan final : public CMonster
{
public:
	typedef struct tagLeviathanDesc : public CMonster::DESC
	{

	}DESC;

	enum LEVI_ANIM {
		Idle, Damage, Death, Destroy_node, Idle_Tongue_Out, Roar, Rock_Throw, Slag_Spray, Tongue_ground_slam, ground_slam,
		Idle_tongue_out_look_around, Idle_look_around, Spray, Spit_worm, roar_tongue0, roar_tongue, idle_lefteye, idle_righteye, 
		idle_injured, Damage_to_Idle_Injured, Injured_rock_throw, Injured_tongue_ground_slam, Injured_slag_spray, Injured_damage,
		Injured_death, Preview, swallow_player, lefteye_start, lefteye_end, righteye_start, righteye_end,
		LEVI_ANIM_END
	};

	enum LEVI_STATE {
		STATE_Idle, STATE_Engage,
		STATE_Attack_Spray, STATE_Attack_ThrowRock, STATE_Attack_Slam, STATE_Attack_SpitWorm,
		STATE_Damaged, STATE_Roar, STATE_Stun_Left, STATE_Stun_Right, STATE_LookAround,
		STATE_Dead,
		STATE_END
	};

#pragma region MyFriendMyState

	friend class CLeviathanState;
	friend class CLeviathanState_Engage; //??
	friend class CLeviathanState_Idle; //??
	friend class CLeviathanState_Attack_Spray;
	friend class CLeviathanState_Attack_ThrowRock;
	friend class CLeviathanState_Attack_Slam;
	friend class CLeviathanState_Attack_SpitWorm;
	friend class CLeviathanState_Damaged;
	friend class CLeviathanState_Roar;
	friend class CLeviathanState_Stun_Left;
	friend class CLeviathanState_Stun_Right;
	friend class CLeviathanState_LookAround;
	friend class CLeviathanState_Dead;
#pragma endregion

private:
	CLeviathan(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLeviathan(const CLeviathan& Prototype);
	virtual ~CLeviathan() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_PartObjects(void* pArg);
	HRESULT Ready_HitMeshes(void* pArg);
	HRESULT Ready_LeviathanStates();

	void Set_State(LEVI_STATE eState);
	void Update_State(_float fTimeDelta);

public:
	virtual void On_Collision(_uint iMyColID, _uint iHitColID, CCollider* pHitCol) override;

private:
	class CLeviathanState*	m_pCurState = { nullptr };
	class CLeviathanState*	m_pStates[STATE_END] = { nullptr };
	LEVI_STATE				m_eCurState =  { LEVI_STATE::STATE_END };
	LEVI_STATE				m_ePrevState = { LEVI_STATE::STATE_END };

private:
	_bool					m_bInjured = { false }; // 힛메쉬 한두개 남았을 때 부터?
	

public:
	static CLeviathan* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END