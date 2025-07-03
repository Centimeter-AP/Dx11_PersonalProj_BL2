#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CUI_HP_Bar final : public CUIObject
{
public:
	typedef struct tagUIHPDesc : public CUIObject::UIOBJECT_DESC
	{
		_int* iHP = { nullptr };
		_int* iMaxHP = { nullptr };
	}DESC;

private:
	CUI_HP_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_HP_Bar(const CUI_HP_Bar& Prototype);
	virtual ~CUI_HP_Bar() = default;

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
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_int* m_iHP = { nullptr };
	_int* m_iMaxHP = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CUI_HP_Bar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};
NS_END
