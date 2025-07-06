#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CUI_EnemyHP final : public CUIObject
{
public:
	typedef struct tagUIAimDesc : public CUIObject::UIOBJECT_DESC
	{

	}DESC;

private:
	CUI_EnemyHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_EnemyHP(const CUI_EnemyHP& Prototype);
	virtual ~CUI_EnemyHP() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Show_UI(CGameObject* pEnemy)
	{
		if (pEnemy != nullptr)
		m_pOwner = pEnemy;
		m_isActive = true;
		m_bRender = true;
		m_fRenderTime = 0.f;
		m_fOpacity = 0.f;
	}
	void Hide_UI() {
		m_bRender = false; m_fRenderTime = 0.f; m_fOpacity = 0.f;
	}
	//void Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }
	//void Set_Render(_bool bRender) {
	//	m_isActive = true;
	//	m_bRender = bRender; 
	//	m_fRenderTime = 0.f; 
	//	m_fOpacity = 0.f;
	//}

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom[2] = {nullptr};
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	_float4x4 m_BarWorldMatrix = {};

private:
	CGameObject* m_pOwner = { nullptr };
	_bool	m_bRender = { false }; // 밖에서 렌더하라고 호출하기 위한 용도
	_float	m_fOpacity = { 1.f };
	_float	m_fRenderTime = {};

	_int*	m_pHP = { nullptr };
	_int* m_pMaxHP = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CUI_EnemyHP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};
NS_END
