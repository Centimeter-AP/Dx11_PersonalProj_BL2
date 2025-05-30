#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "PlayerState.h"

NS_BEGIN(Engine)	
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CPlayer final : public CGameObject
{
public:
	typedef struct tagPlayerDesc : public CGameObject::DESC
	{
	}DESC;

	enum PLA_AR {
		Crouch, Draw, Grenade_throw, Holster, Idle, Jump_End, Jump_Idle, Jump_Start, R_Banditmade, R_Dahl,
		R_Jakobs, R_Torgue, R_Vladof, Run_F, Run_L, Run_R, Sight_Idle, Sight_Jump_End, Sight_Jump_Idle, Sight_Jump_Start,
		Sight_Walk_F, Sprint, Sprint_DualWield, R_Dahl_Fast, R_Jakobs_Var2, R_Jakobs_Fast, R_Banditmade_Fast, R_Torgue_Fast, R_Vladof_Fast, R_Jakobs_Fast_Var2,
		R_DualWield, Bladed_Melee, Holster_Fast, Incap_F, ADD_Fire_Recoil, PickUp_Equip, R_Banditmade_Var2, R_Banditmade_Fast_Var2 = 37, PLA_AR_END
	};

	enum PLA_STATE {
		STATE_Idle, STATE_Run, STATE_Run_L, STATE_Run_R, STATE_Sprint, STATE_Jump, STATE_Fire, STATE_Reload, STATE_ChangeWeapon, STATE_ThrowGrenade, STATE_Skill_PhaseLock, STATE_END
	};

#pragma region MyFriendMyState

	friend class CPlayerState_Idle;
	friend class CPlayerState_Run;
	friend class CPlayerState_Run_L;
	friend class CPlayerState_Run_R;
	friend class CPlayerState_Sprint;
	friend class CPlayerState_Jump;
	friend class CPlayerState_Fire;
	friend class CPlayerState_Reload;
	friend class CPlayerState_ChangeWeapon;
	friend class CPlayerState_ThrowGrenade;
	friend class CPlayerState_Skill_PhaseLock;

#pragma endregion


private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	void	Key_Input(_float fTimeDelta);
	void	Set_State(PLA_STATE eState);

private:
	void	Update_State(_float fTimeDelta);

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_PartObjects(void* pArg);
	HRESULT Ready_PlayerStates();
	HRESULT Bind_ShaderResources();

private:
	CShader*			m_pShaderCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };
	_float				m_fSensor = {};
	class CPlayerState*	m_pCurState = { nullptr };
	class CPlayerState*	m_pStates[STATE_END] = { nullptr };
	PLA_STATE			m_eCurState = { PLA_STATE::STATE_END };
	PLA_STATE			m_ePrevState = { PLA_STATE::STATE_END };

private:
	_bool				m_isRunning = { false };
	_bool				m_isJumping = { false };
	_bool				m_isPlayingNonLoopAnim = { false };	

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END