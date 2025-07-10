#pragma once

#include "Client_Defines.h"
#include "SpriteEffect.h"


NS_BEGIN(Client)

class CMuzzleFlash final : public CSpriteEffect
{
public:
	typedef struct tagMuzzleFlashDesc : public CSpriteEffect::DESC
	{
		_float fPosOffsetY = { -0.07f };
	}DESC;
private:
	CMuzzleFlash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMuzzleFlash(const CMuzzleFlash& Prototype);
	virtual ~CMuzzleFlash() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

	void Show_Effect();

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	_int	m_iTileX{}, m_iTileY{};
	_int	m_iTileIdx{};
	_float2 m_fTileSize{};
	_float2 m_fOffset{};
	_float	m_fOpacity = { 1.f };
	_float  m_fPosOffsetY = {};
	_float m_fShowTicker = {};

public:
	static CMuzzleFlash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END