#include "UI_BossHP.h"
#include "GameInstance.h"

CUI_BossHP::CUI_BossHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CUI_BossHP::CUI_BossHP(const CUI_BossHP& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CUI_BossHP::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_BossHP::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	// 294 17
	m_iHP = pDesc->iHP;
	m_iMaxHP = pDesc->iMaxHP;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_BossHP::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
}

EVENT CUI_BossHP::Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return EVN_NONE;

	//XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix4x4Ptr()) * XMLoadFloat4x4(m_pParentMatrix));


	return EVN_NONE;
}

void CUI_BossHP::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return ;

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CUI_BossHP::Render()
{
	if (FAILED(Render_Back()))
		return E_FAIL;
	if (FAILED(Render_Bar()))
		return E_FAIL;
	m_pGameInstance->Draw_Font(TEXT("Font_Compacta"), L"Leviathan", _float2(m_fX - m_fSizeX * 0.5f + 10.f, m_fY + 5.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.5f);

	return S_OK;
}

HRESULT CUI_BossHP::Render_Bar()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom[TYPE_BAR], "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom[TYPE_BAR]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom[TYPE_BAR]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TYPE_BAR]->Bind_ShaderResource(m_pShaderCom[TYPE_BAR], "g_Texture", 0)))
		return E_FAIL;

	_float fPercentage = 1.f - (*m_iHP / static_cast<_float>(*m_iMaxHP));
	if (FAILED(m_pShaderCom[TYPE_BAR]->Bind_RawValue("g_fPercentage", &fPercentage, sizeof(_float))))
		return E_FAIL;
	_float fOpacity = 1.f;
	if (FAILED(m_pShaderCom[TYPE_BAR]->Bind_RawValue("g_fOpacity", &fOpacity, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom[TYPE_BAR]->Begin(POSTEX_UI_BLEND)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom[TYPE_BAR]->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom[TYPE_BAR]->Render()))
		return E_FAIL;
}

HRESULT CUI_BossHP::Render_Back()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom[TYPE_BACK], "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom[TYPE_BACK]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom[TYPE_BACK]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TYPE_BACK]->Bind_ShaderResource(m_pShaderCom[TYPE_BACK], "g_Texture", 0)))
		return E_FAIL;

	_float fPercentage = {0.f};
	if (FAILED(m_pShaderCom[TYPE_BACK]->Bind_RawValue("g_fPercentage", &fPercentage, sizeof(_float))))
		return E_FAIL;
	_float fOpacity = 1.f;
	if (FAILED(m_pShaderCom[TYPE_BACK]->Bind_RawValue("g_fOpacity", &fOpacity, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom[TYPE_BACK]->Begin(POSTEX_UI_BLEND)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom[TYPE_BACK]->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom[TYPE_BACK]->Render()))
		return E_FAIL;
}

HRESULT CUI_BossHP::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader_back"), reinterpret_cast<CComponent**>(&m_pShaderCom[TYPE_BACK]))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_back"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[TYPE_BACK]))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_BossHP_Back"),
		TEXT("Com_Texture_back"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_BACK]))))
		return E_FAIL;

	/**************************************************************/

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader_bar"), reinterpret_cast<CComponent**>(&m_pShaderCom[TYPE_BAR]))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_bar"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[TYPE_BAR]))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_BossHP"),
		TEXT("Com_Texture_bar"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_BAR]))))
		return E_FAIL;

	return S_OK;
}

CUI_BossHP* CUI_BossHP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_BossHP* pInstance = new CUI_BossHP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_BossHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BossHP::Clone(void* pArg)
{
	CUI_BossHP* pInstance = new CUI_BossHP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_BossHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BossHP::Free()
{
	__super::Free();
	for (size_t i = 0; i < TYPE_END; i++)
	{
		Safe_Release(m_pVIBufferCom[i]);
		Safe_Release(m_pShaderCom[i]);
		Safe_Release(m_pTextureCom[i]);
	}
}
