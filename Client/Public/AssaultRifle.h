#pragma once

#include "Weapon.h"

class CAssaultRifle final : public CWeapon
{
public:
	typedef struct tagMonsterDesc : public CWeapon::DESC
	{

	}DESC;

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

