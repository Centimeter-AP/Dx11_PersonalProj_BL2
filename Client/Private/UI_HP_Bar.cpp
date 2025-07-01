#include "UI_HP.h"
#include "GameInstance.h"

CUI_HP::CUI_HP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CUI_HP::CUI_HP(const CUI_HP& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CUI_HP::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_HP::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_iHP = pDesc->iHP;
	m_iMaxHP = pDesc->iMaxHP;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_HP::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
}

EVENT CUI_HP::Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return EVN_NONE;

	return EVN_NONE;
}

void CUI_HP::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return ;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CUI_HP::Render()
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
		_float fPercentage = {};
		if (i == TYPE_BAR)
			fPercentage = 1.f - (*m_iHP / static_cast<_float>(*m_iMaxHP));
		else
			fPercentage = 0.f;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fPercentage", &fPercentage, sizeof(_float))))
			return E_FAIL;

		if (i == TYPE_BAR)
		{
			if (FAILED(m_pShaderCom->Begin(POSTEX_UI_BLEND)))
				return E_FAIL;
		}
		else
			if (FAILED(m_pShaderCom->Begin(POSTEX_ALPHABLEND)))
				return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}

	_wstring strHP = to_wstring(*m_iHP);
	m_pGameInstance->Draw_Font(TEXT("Font_WillowBody"), strHP.c_str(), _float2(m_fX - m_fSizeX * 0.5f + 25.f, m_fY - m_fSizeY * 0.5f + 5.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.5f);

	return S_OK;
}

HRESULT CUI_HP::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Icon_HP"),
		TEXT("Com_Texture_Icon"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_ICON]))))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_HP"),
		TEXT("Com_Texture_Bar"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_BAR]))))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Left"),
		TEXT("Com_Texture_BarBack"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_BARBACK]))))
		return E_FAIL;

	return S_OK;
}

CUI_HP* CUI_HP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_HP* pInstance = new CUI_HP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_HP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_HP::Clone(void* pArg)
{
	CUI_HP* pInstance = new CUI_HP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_HP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_HP::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	for (size_t i = 0; i < TYPE_END; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
	}
}
