#pragma once
#include "Weapon.h"
#include "PSTState.h"

NS_BEGIN(Client)
class CPistol final : public CWeapon
{
public:
	typedef struct tagPistolDesc : public CWeapon::DESC
	{

	}DESC;

	enum PST_ANIM {
		Fire_Dahl, Fire_Hyperion, Fire_Jakobs, Fire_Maliwan, Fire_Tediore, Fire_Torgue, Idle_Dahl, Idle_Hyperion, Idle_Jakobs, Idle_Maliwan,
		Idle_Torgue, Fire_BanditMade, Fire_Vladof, R_Jakobs, R_Jakobs_Var2, R_Hyperion, R_V, R_Band, R_Dahl, R_mal, R_Tor, R_BandF,
		R_Hyperion_Fast, R_JaFV2, R_JaF, R_MalF, R_TorF, R_VlaF, R_Tediore, R_Tediore_Fast, Fire_Fan, PST_ANIMATION_END
	};

	enum PST_STATE {
		STATE_Draw, STATE_Fire, STATE_Idle, STATE_Reload, STATE_Reload_Fast, STATE_END
	};

#pragma region MyFriendMyState

	friend class CPSTState_Idle;
	friend class CPSTState_Draw;
	friend class CPSTState_Fire;
	friend class CPSTState_Reload;
	friend class CPSTState_Reload_Fast;

#pragma endregion


private:
	CPistol(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPistol(const CPistol& Prototype);
	virtual ~CPistol() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	void	Set_State(PST_STATE eState);

private:
	void	Update_State(_float fTimeDelta);

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_PSTStates();
	virtual void Initialize_BasicStatus() override;

private:
	class CPSTState*	m_pCurState = { nullptr };
	class CPSTState*	m_pStates[PST_STATE::STATE_END] = { nullptr };
	PST_STATE			m_eCurState = { PST_STATE::STATE_END };
	PST_STATE			m_ePrevState = { PST_STATE::STATE_END };

public:
	static CPistol* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END