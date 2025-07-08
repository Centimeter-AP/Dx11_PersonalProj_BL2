#pragma once

#include "Client_Defines.h"
#include "SpriteEffect.h"

NS_BEGIN(Client)

class CWaterExplode final : public CSpriteEffect
{
public:
	typedef struct tagWaterExplodeDesc : public CSpriteEffect::DESC
	{

	}DESC;
private:
	CWaterExplode(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWaterExplode(const CWaterExplode& Prototype);
	virtual ~CWaterExplode() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

	void Show_Effect(); // 할필요가없지않나요?

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	CTexture*	m_pTextureMaskCom = { nullptr };
	_float		m_fOpacity = { 1.f };

	_float		m_fShowTicker = {};
	_float4		m_vColor = {};
	_vector		m_vMaxScale = { 200.f, 200.f, 200.f };

public:
	static CWaterExplode* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END