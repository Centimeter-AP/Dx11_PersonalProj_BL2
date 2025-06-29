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

private:
	CShader* m_pShaderCom = { nullptr };
	enum TEXTYPE { TYPE_BARBACK, TYPE_BAR, TYPE_END };
	CTexture* m_pTextureCom[2] = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CUI_Phaselock* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};
NS_END
