#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
class CNavigation;
class CGravity;
NS_END

NS_BEGIN(Client)

class CPlayer final : public CGameObject
{
public:
	typedef struct tagPlayerDesc : public CGameObject::DESC
	{
		_int				iNavigationIndex = { -1 };
	}DESC;

	enum PLA_AR {
		AR_Crouch, AR_Draw, AR_Grenade_throw, AR_Holster, AR_Idle, AR_Jump_End, AR_Jump_Idle, AR_Jump_Start, AR_NONE8, AR_NONE9,
		AR_R_Jakobs, AR_NONE11, AR_NONE12, AR_Run_F, AR_Run_L, AR_Run_R, AR_Sight_Idle, AR_Sight_Jump_End, AR_Sight_Jump_Idle, AR_Sight_Jump_Start,
		AR_Sight_Walk_F, AR_Sprint, AR_Sprint_DualWield, AR_NONE23, AR_R_Jakobs_Var2, AR_R_Jakobs_Fast, AR_NONE26, AR_NONE27, AR_NONE28, AR_R_Jakobs_Fast_Var2,
		AR_R_DualWield, AR_Bladed_Melee, AR_Holster_Fast, AR_Incap_F, AR_ADD_Fire_Recoil, AR_PickUp_Equip, AR_NONE36, AR_NONE37 = 37, PLA_AR_END
	};
	enum PLA_PST {
		PST_Crouch = PLA_AR_END, PST_Draw, PST_Grenade_throw, PST_Holster, PST_Idle, PST_Jump_End, PST_Jump_Idle, PST_Jump_Start, PST_NONE46, PST_NONE47, PST_NONE48, PST_NONE49,
		PST_R_Hyperion, PST_R_Hyperion_Fast, PST_R_Jakobs, PST_R_Jakobs_Fast, PST_NONE54, PST_NONE55, PST_NONE56, PST_NONE57, PST_NONE58, PST_NONE59, PST_NONE60, PST_Run_F = 61, PST_Run_L, PST_Run_R, PST_Sight_Idle, PST_Sight_Jump_End, PST_Sight_Jump_Idle, PST_Sight_Jump_Start,
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

	virtual void On_Collision(_uint iMyColID, _uint iHitColID, CCollider* pHitCol) override;

	void Key_Input(_float fTimeDelta);

	void Set_State(PLA_STATE eState);
	void Initialize_BasicStatus();
	HRESULT Change_Level(_uint iLevelIndex);

private: 
	void Update_State(_float fTimeDelta);
	void Raycast_Object();
	void EnemyHPUI_Check();
	//void Ride_Terrain(); // 임시로 지형이라도 타자 // 이제 안 타도 돼요

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_PartObjects(void* pArg);
	HRESULT Ready_Weapons(void* pArg);
	HRESULT Ready_UIObjects(void* pArg);
	HRESULT Ready_Effects(void* pArg);
	HRESULT Ready_PlayerStates();
	HRESULT Bind_ShaderResources();
	
private:
	void Skill_Cooldowns(_float fTimeDelta);

public:
	const _float& Get_Pitch() { return m_fPitch; }

	const _float& Get_PhaselockDuration() { return m_fPhaselockDuration; }
	const _bool Get_PhaselockAble() { return m_bPhaselockAble; }
	const CGameObject* Get_CurWeapon();
	const _uint	Get_CurLevel() { return m_iLevelID; }
	const _float& Get_CurPickedDistance() { return m_fCurPickedDistance; }
	const _float3 Get_CurPickedPos();

private:
	CShader*			m_pShaderCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };
	CGravity*			m_pGravityCom = { nullptr };
	CNavigation*		m_pNavigationCom = { nullptr };
	_int				m_iNavIndex = {};

private: /* ㅋㅋ */
	_int				m_iLevel = { 1 };
	_int				m_iHP = {};
	_int				m_iMaxHP = {};

	// 쉴드 아이템 만들면 빼고, 아님 그냥 플레이어에 붙이고
	_float				m_fShield = {};
	_float				m_fMaxShield = {};
	_bool				m_bShield = { true }; // 이걸따로두는게맞나? 
	_int				m_iShieldRechargeRate = {}; // 1초당 얼마나 차는지
	_float				m_fShieldRechargeDelay = {}; // 일정 시간 이상 피격되지 않으면 채움

	_float				m_fNoHitTimeTicker = {}; // 이녀석이 비피격 상태 초 체크 할 것 (전투 비전투 상태도 체크할까요)
	_bool				m_bCombat = { false }; // 전투/비전투

	void Check_Player_NoHitTime(_float fTimeDelta);
	void Recharge_Shield(_float fTimeDelta);


private: /* 플레이어 스테이트 변수 */
	_float				m_fSensor = {};
	class CPlayerState*	m_pCurState = { nullptr };
	class CPlayerState*	m_pStates[STATE_END] = { nullptr };
	PLA_STATE			m_eCurState = { PLA_STATE::STATE_END };
	PLA_STATE			m_ePrevState = { PLA_STATE::STATE_END };

private: // maybe deprecated /* 카메라 회전 용 변수.. */
	_float				m_fPitch = {};
	_float				m_fPreviousPitch = {};
	const _float		m_fPitchLimit = XMConvertToRadians(89.9f); 

private: /* 레이피킹 변수 */
	CCollider*			m_pLastPickedCollider = { nullptr };
	CCollider*			m_pCurPickedCollider = { nullptr };
	_float				m_fCurPickedDistance = {};
	_uint				m_iCameraBoneIdx = {};

private: /* 달리기 변수 */
	_bool				m_bSprint = { false };
	_float				m_fSpeedMultiplier = { 1.f };

private: /* 스킬 변수 */
	_bool				m_bPhaselockAble = { true };
	_float				m_fPhaselockDuration = {};
	_float				m_fPhaselockCooldown = {};
	_float				m_fPhaselockCooldownTicker = {};
	_float				m_fPhaselockUsableDistance = {};

	void Cooldown_Phaselock(_float fTimeDelta);
	// 시간 안되겟지만 다른 스킬 추가하면 밑에

private:
	_int				m_iAmmo[WTYPE_END] = {};

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