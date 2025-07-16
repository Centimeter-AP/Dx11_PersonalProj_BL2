#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

NS_BEGIN(Client)
class CDamageFont final : public CUIObject
{
public:
	typedef struct tagDamageFontDesc : public CUIObject::UIOBJECT_DESC
	{
		_wstring		strDamage = {};
		_float3			vWorldPosition = {};
		_float3			vFontColor = {1.f, 1.f, 1.f};
		_bool			bCrit = { false };
	}DESC;

private:
	CDamageFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDamageFont(const CDamageFont& Prototype);
	virtual ~CDamageFont() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_wstring	m_strDamage = {};
	_float3		m_vWorldPosition = {};
	_float		m_fLifeTime = {};
	_float		m_fAge = {};
	_float		m_fVelocity = {};
	_float		m_fXMoveSpeed = {};

	_float		m_fFontScale = { 1.f };
	_float2		m_fFontOrigin = {};
	_float		m_fFontAlpha = {};
	_float2		m_vFontPosition = {};
	_float3		m_vFontColor = {};


private:
	HRESULT Ready_Components();

public:
	static CDamageFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END
