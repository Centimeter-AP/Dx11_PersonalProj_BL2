#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CLoadingUI final : public CUIObject
{
public:
	typedef struct tagLoadingDesc : public CUIObject::UIOBJECT_DESC
	{
		_uint iLevelTextureIdx = {};
	}DESC;
private:
	CLoadingUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLoadingUI(const CLoadingUI& Prototype);
	virtual ~CLoadingUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_LoadingWeapon();

private:
	CShader*		m_pShaderCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };
	CVIBuffer_Rect*	m_pVIBufferCom = { nullptr };
	_uint			m_iLevelTextureIdx = {};

public:
	static CLoadingUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

NS_END
