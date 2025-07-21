#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CLogo_BackGround final : public CUIObject
{
public:
	typedef struct tagBackGroundDesc : public CUIObject::UIOBJECT_DESC
	{

	}BACKGROUND_DESC;

private:
	CLogo_BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLogo_BackGround(const CLogo_BackGround& Prototype);
	virtual ~CLogo_BackGround() = default;

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
		
private:
	HRESULT Ready_Components();

public:
	static CLogo_BackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END