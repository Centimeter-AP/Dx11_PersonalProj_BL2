#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CScreen_Hit final : public CUIObject
{
public:
	typedef struct tagScreenHitDesc : public CUIObject::UIOBJECT_DESC
	{
		_bool* pPlayerOnShield = { nullptr };
	}DESC;

private:
	CScreen_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CScreen_Hit(const CScreen_Hit& Prototype);
	virtual ~CScreen_Hit() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

	void Show_Effect(_fvector vHitPos);
	void Calc_HitPos(_fvector vHitPos);

private:
	CShader*			m_pShaderCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };

private:
	_bool*		m_pPlayerOnShield = { nullptr };
	_bool		m_bHit = { false };
	_float		m_fCoolDown = {};	// 이펙트 자체 쿨다운
	_float		m_fShowTicker = {}; // 이펙트 보여지는 시간
	_float		m_fOpacity = {1.f};
	_float4		m_vColorPercent = { 1.f, 0.f, 0.f, 1.f };
	enum HITPOS { HITPOS_LEFT, HITPOS_RIGHT, HITPOS_UP, HITPOS_DOWN };
	HITPOS		m_eHitPos = HITPOS::HITPOS_LEFT; 

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CScreen_Hit* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END
