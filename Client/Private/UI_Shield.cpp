#include "UI_Shield.h"
#include "GameInstance.h"

CUI_Shield::CUI_Shield(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CUI_Shield::CUI_Shield(const CUI_Shield& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CUI_Shield::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Shield::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_Shield::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
}

EVENT CUI_Shield::Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return EVN_NONE;

	return EVN_NONE;
}

void CUI_Shield::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return ;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CUI_Shield::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	for (size_t i = 1; i < TYPE_END; ++i)
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

	}
	return S_OK;
}

HRESULT CUI_Shield::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Icon_Shield"),
		TEXT("Com_Texture_Icon"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_ICON]))))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Shield"),
		TEXT("Com_Texture_Bar"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_BAR]))))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_ShieldBack"),
		TEXT("Com_Texture_BarBack"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_BARBACK]))))
		return E_FAIL;

	return S_OK;
}

CUI_Shield* CUI_Shield::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Shield* pInstance = new CUI_Shield(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Shield");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Shield::Clone(void* pArg)
{
	CUI_Shield* pInstance = new CUI_Shield(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Shield");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Shield::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	for (size_t i = 0; i < TYPE_END; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
	}
}
