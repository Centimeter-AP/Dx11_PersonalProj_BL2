#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CUI_AmmoPannel final : public CUIObject
{
public:
	typedef struct tagUIHPDesc : public CUIObject::UIOBJECT_DESC
	{
		_int* iAmmo = { nullptr };
		_int* iMaxAmmo = { nullptr };
	}DESC;

private:
	CUI_AmmoPannel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_AmmoPannel(const CUI_AmmoPannel& Prototype);
	virtual ~CUI_AmmoPannel() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_PartObjects(void* pArg);

public:
	static CUI_AmmoPannel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};
NS_END
