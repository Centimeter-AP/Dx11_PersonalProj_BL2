#pragma once

#include "Client_Defines.h"
#include "SpriteEffect.h"


NS_BEGIN(Client)

class CPhaselockSphere final : public CSpriteEffect
{
private:
	CPhaselockSphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPhaselockSphere(const CPhaselockSphere& Prototype);
	virtual ~CPhaselockSphere() = default;

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

private:
	CTexture* m_pTextureMaskCom = { nullptr };


public:
	static CPhaselockSphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END