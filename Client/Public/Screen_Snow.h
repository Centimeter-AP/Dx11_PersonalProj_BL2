#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CScreen_Snow final : public CUIObject
{
public:
	typedef struct tagScreenSnowDesc : public CUIObject::UIOBJECT_DESC
	{

	}DESC;

private:
	CScreen_Snow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CScreen_Snow(const CScreen_Snow& Prototype);
	virtual ~CScreen_Snow() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTextureMaskCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_int	m_iTileX{}, m_iTileY{};
	_int	m_iTileIdx{};
	_float2 m_fTileSize{};
	_float2 m_fOffset{};
	_float	m_fDropTicker = {};
	_float	m_fCoolDown = {};
	_bool	m_bDrop = {false};
	_float	m_fOpacity = {1.f};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CScreen_Snow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END
