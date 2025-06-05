#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CCollider;
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
		AR_Crouch, AR_Draw, AR_Grenade_throw, AR_Holster, AR_Idle, AR_Jump_End, AR_Jump_Idle, AR_Jump_Start, AR_NONE8, AR_NONE9,
		AR_R_Jakobs, AR_NONE11, AR_NONE12, AR_Run_F, AR_Run_L, AR_Run_R, AR_Sight_Idle, AR_Sight_Jump_End, AR_Sight_Jump_Idle, AR_Sight_Jump_Start,
		AR_Sight_Walk_F, AR_Sprint, AR_Sprint_DualWield, AR_NONE23, AR_R_Jakobs_Var2, AR_R_Jakobs_Fast, AR_NONE26, AR_NONE27, AR_NONE28, AR_R_Jakobs_Fast_Var2,
		AR_R_DualWield, AR_Bladed_Melee, AR_Holster_Fast, AR_Incap_F, AR_ADD_Fire_Recoil, AR_PickUp_Equip, AR_NONE36, AR_NONE37 = 37, PLA_AR_END
	};
	enum PLA_PST {
		PST_Crouch = PLA_AR_END, PST_Draw, PST_Grenade_throw, PST_Holster, PST_Idle, PST_Jump_End, PST_Jump_Idle, PST_Jump_Start, PST_NONE46, PST_NONE47, PST_NONE48, PST_NONE49,
		PST_R_Hyperion, PST_R_Hyperion_Fast, PST_NONE52, PST_NONE53, PST_NONE54, PST_NONE55, PST_NONE56, PST_NONE57, PST_NONE58, PST_NONE59, PST_NONE60, PST_Run_F = 61, PST_Run_L, PST_Run_R, PST_Sight_Idle, PST_Sight_Jump_End, PST_Sight_Jump_Idle, PST_Sight_Jump_Start,
		PST_Sight_Walk_F, PST_Sprint, PST_Sprint_DualWield, PST_NONE71, PST_NONE72, PST_NONE73, PST_Reload_DualWield, PST_Holster_Fast, PST_Incap_F, PST_NONE77, PST_ADD_Fire_Recoil, PST_ADD_Fire_Revolver_Recoil, PST_PickUp_Equip, PST_Fire_Fan_Recoil, PLA_PST_END
	};
	enum PLA_UNARMED {
		UN_Draw = PLA_PST_END, UN_Holster, UN_Idle, UN_Jump_End, UN_Jump_Idle, UN_Jump_Start, UN_Run_F, UN_Sprint, UN_Run_R, UN_Run_L, UN_Walk_F, UN_Incap_F, PLA_UNARMED_END
	};
	enum PLA_BASIC {
		BASIC_PhaseLock_Fail = PLA_UNARMED_END, BASIC_PhaseLock, BASIC_KissOfDeath, BASIC_CloseAttack, BASIC_RollingThunder, PLA_BASIC_END
	};
	enum PLA_ALL {
		ALL_PhaseLock = PLA_BASIC_END, ALL_Respawn = 47 + PLA_BASIC_END, ALL_Ladder_Up = 49 + PLA_BASIC_END, ALL_PhaseLock_Fail = 50 + PLA_BASIC_END, PLA_ALL_END = 58 + PLA_BASIC_END
	};

	enum PLA_STATE {
		STATE_Idle, STATE_Run, STATE_Run_L, STATE_Run_R, STATE_Sprint, STATE_Jump,
		STATE_Fire, STATE_Reload, STATE_HolsterWeapon, STATE_DrawWeapon, STATE_ThrowGrenade, STATE_CloseAttack,
		STATE_Skill_PhaseLock, STATE_SKill_KissOfDeath, STATE_END
	};

	enum WEAPON_TYPE { WTYPE_UNARMED, WTYPE_AR, WTYPE_PISTOL, WTYPE_END };

#pragma region MyFriendMyState

	friend class CPlayerState;
	friend class CPlayerState_Idle;
	friend class CPlayerState_Run;
	friend class CPlayerState_Run_L;
	friend class CPlayerState_Run_R;
	friend class CPlayerState_Sprint;
	friend class CPlayerState_Jump;
	friend class CPlayerState_Fire;
	friend class CPlayerState_Reload;
	friend class CPlayerState_CloseAttack;
	friend class CPlayerState_HolsterWeapon;
	friend class CPlayerState_DrawWeapon;
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
	const _float& Get_Pitch() { return m_fPitch; }

private:
	void	Update_State(_float fTimeDelta);

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_PartObjects(void* pArg);
	HRESULT Ready_Weapons(void* pArg);
	HRESULT Ready_PlayerStates();
	HRESULT Bind_ShaderResources();

private:
	CShader*			m_pShaderCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };
	CCollider*			m_pColCam = { nullptr };

private:
	_float				m_fSensor = {};
	class CPlayerState*	m_pCurState = { nullptr };
	class CPlayerState*	m_pStates[STATE_END] = { nullptr };
	PLA_STATE			m_eCurState = { PLA_STATE::STATE_END };
	PLA_STATE			m_ePrevState = { PLA_STATE::STATE_END };

private: // maybe deprecated 
	_float				m_fPitch = {};
	_float				m_fPreviousPitch = {};
	const _float		m_fPitchLimit = XMConvertToRadians(89.9f); 

private:
	vector<CGameObject*>		m_pWeapons; // 임시 무기 저장 변수, 인벤토리 만들고 꼭 이새끼 날릴 것 !!! 아닌가그냥써야하나??
	WEAPON_TYPE					m_ePrevWeapon = { WTYPE_END };
	WEAPON_TYPE					m_eCurWeapon = { WTYPE_END };
	void Change_Weapon(WEAPON_TYPE eWeaponType);

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END