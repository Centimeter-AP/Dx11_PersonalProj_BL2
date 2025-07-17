#include "UI_Exp.h"
#include "GameInstance.h"

CUI_Exp::CUI_Exp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CUI_Exp::CUI_Exp(const CUI_Exp& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CUI_Exp::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Exp::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_Exp::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
}

EVENT CUI_Exp::Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return EVN_NONE;

	return EVN_NONE;
}

void CUI_Exp::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return ;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CUI_Exp::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	_float fOpacity = 1.f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fOpacity", &fOpacity, sizeof(_float))))
		return E_FAIL;
	for (size_t i = 0; i < TYPE_END; ++i)
	{
		if (nullptr == m_pTextureCom[i])
			continue;
		if (FAILED(m_pTextureCom[i]->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
			return E_FAIL;
		//if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		//	return E_FAIL; 

		if (FAILED(m_pShaderCom->Begin(2)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
		//_wstring str = L"LV";
		//m_pGameInstance->Draw_Font(TEXT("Font_Compacta"), L"LV", _float2(m_fX - m_fSizeX * 0.5f + 30.f, m_fY + 5.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.7f);
		//m_pGameInstance->Draw_Font(TEXT("Font_Compacta"), L"01", _float2(m_fX - m_fSizeX * 0.5f + 60.f, m_fY), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.8f);
		m_pGameInstance->Draw_Font(TEXT("Font_Compacta"), L"Trickster", _float2(m_fX - m_fSizeX * 0.5f + 50.f, m_fY + 5.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.7f);

	}

	return S_OK;
}

HRESULT CUI_Exp::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Exp"),
		TEXT("Com_Texture_Bar"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_BAR]))))
		return E_FAIL;
		/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Exp_Back"), 
		TEXT("Com_Texture_BarBack"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_BARBACK]))))
		return E_FAIL;

	return S_OK;
}

CUI_Exp* CUI_Exp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Exp* pInstance = new CUI_Exp(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Exp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Exp::Clone(void* pArg)
{
	CUI_Exp* pInstance = new CUI_Exp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Exp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Exp::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	for (size_t i = 0; i < TYPE_END; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
	}
}
