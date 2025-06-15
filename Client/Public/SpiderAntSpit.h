#pragma once

#include "Bullet.h"

class CSpiderAntSpit final : public CBullet
{

public:
	typedef struct tagSpiderAntSpitDesc : public CBullet::DESC
	{

	}DESC;

private:
	CSpiderAntSpit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpiderAntSpit(const CSpiderAntSpit& Prototype);
	virtual ~CSpiderAntSpit() = default;

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
	static CSpiderAntSpit* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

