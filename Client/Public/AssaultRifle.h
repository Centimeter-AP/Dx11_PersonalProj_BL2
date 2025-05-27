#pragma once

#include "Weapon.h"

class CAssaultRifle final : public CWeapon
{
public:
	typedef struct tagMonsterDesc : public CWeapon::DESC
	{

	}DESC;

	enum AR_ANIMATION {
		Draw, Fire1, Fire_Banditmade, Fire_Dahl, Fire_Jakobs, Fire_Torgue, Fire_Vladof, Idle, R_Banditmade, R_Banditmade_Fast,
		R_Banditmade_Fast_Var2, R_Banditmade_Var2, R_Dahl, R_Dahl_Fast, R_Jakobs, R_Jakobs_Fast, R_Jakobs_Fast_Var2, R_Jakobs_Var2, R_Tediore, R_Torgue,
		R_Torgue_Fast, R_Vladof, R_Vladof_Fast = 22, AR_ANIMATION_END
	};

private:
	CAssaultRifle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAssaultRifle(const CAssaultRifle& Prototype);
	virtual ~CAssaultRifle() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();


private:
	HRESULT Ready_Components(void* pArg);

public:
	static CAssaultRifle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

