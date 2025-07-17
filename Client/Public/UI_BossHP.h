#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CUI_BossHP final : public CUIObject
{
public:
	typedef struct tagUIHPDesc : public CUIObject::UIOBJECT_DESC
	{
		_int* iHP = { nullptr };
		_int* iMaxHP = { nullptr };
	}DESC;

private:
	CUI_BossHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_BossHP(const CUI_BossHP& Prototype);
	virtual ~CUI_BossHP() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	HRESULT Render_Bar();
	HRESULT Render_Back();

private:
	enum TYPE {TYPE_BACK, TYPE_BAR, TYPE_END};
	CShader* m_pShaderCom[TYPE_END] = {nullptr};
	CTexture* m_pTextureCom[TYPE_END] = {nullptr};
	CVIBuffer_Rect* m_pVIBufferCom[TYPE_END] = {nullptr};

	_int* m_iHP = { nullptr };
	_int* m_iMaxHP = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CUI_BossHP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};
NS_END
