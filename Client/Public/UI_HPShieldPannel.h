#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CUI_HPShieldPannel final : public CUIObject
{
public:
	typedef struct tagUIHPDesc : public CUIObject::UIOBJECT_DESC
	{
		_int* iHP = { nullptr };
		_int* iMaxHP = { nullptr };
		_float* fShield = { nullptr };
		_float* fMaxShield = { nullptr };
	}DESC;

private:
	CUI_HPShieldPannel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_HPShieldPannel(const CUI_HPShieldPannel& Prototype);
	virtual ~CUI_HPShieldPannel() = default;

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
	static CUI_HPShieldPannel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};
NS_END
