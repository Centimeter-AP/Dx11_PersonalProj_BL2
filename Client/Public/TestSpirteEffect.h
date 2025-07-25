#pragma once

#include "Client_Defines.h"
#include "SpriteEffect.h"


NS_BEGIN(Client)

class CTestSpirteEffect final : public CSpriteEffect
{
private:
	CTestSpirteEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTestSpirteEffect(const CTestSpirteEffect& Prototype);
	virtual ~CTestSpirteEffect() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CTestSpirteEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END