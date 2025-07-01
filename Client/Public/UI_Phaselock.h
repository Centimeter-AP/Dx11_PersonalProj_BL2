#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CUI_Phaselock final : public CUIObject
{
public:
	typedef struct tagUIAimDesc : public CUIObject::UIOBJECT_DESC
	{
		_float fDuration = {};
	}DESC;

private:
	CUI_Phaselock(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Phaselock(const CUI_Phaselock& Prototype);
	virtual ~CUI_Phaselock() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

	void Set_UsingUI() { m_isActive = true;  m_bCheckTime = true; m_fTime = 0.f; }
	void Set_NotUsingUI() { m_isActive = false;  m_bCheckTime = false; m_fTime = 0.f; }

private:
	CShader* m_pShaderCom = { nullptr };
	enum TEXTYPE { TYPE_BARBACK, TYPE_BAR, TYPE_END };
	CTexture* m_pTextureCom[2] = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float	m_fDuration = {3.f};
	_float	m_fTime = {};
	_bool	m_bCheckTime = false;

private:
	HRESULT Ready_Components();

public:
	static CUI_Phaselock* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};
NS_END
