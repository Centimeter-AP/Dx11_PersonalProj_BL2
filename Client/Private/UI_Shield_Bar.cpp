#include "UI_Shield_Bar.h"
#include "GameInstance.h"

CUI_Shield_Bar::CUI_Shield_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CUI_Shield_Bar::CUI_Shield_Bar(const CUI_Shield_Bar& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CUI_Shield_Bar::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Shield_Bar::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_fShield = pDesc->fShield;
	m_fMaxShield = pDesc->fMaxShield;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix4x4Ptr()) * XMLoadFloat4x4(m_pParentMatrix));
	m_fCurPosX = m_CombinedWorldMatrix._41;
	m_fCurPosY = m_CombinedWorldMatrix._42;
	return S_OK;
}

void CUI_Shield_Bar::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
}

EVENT CUI_Shield_Bar::Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return EVN_NONE;

	return EVN_NONE;
}

void CUI_Shield_Bar::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return ;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CUI_Shield_Bar::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float fOpacity = 1.f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fOpacity", &fOpacity, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	_float fPercentage = 1.f - (*m_fShield / *m_fMaxShield);
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fPercentage", &fPercentage, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(POSTEX_UI_BLEND)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	_wstring strHP = to_wstring(static_cast<_int>(*m_fShield));
	m_pGameInstance->Draw_Font(TEXT("Font_WillowBody"), strHP.c_str(),
		_float2(g_iWinSizeX * 0.5f + m_fCurPosX - m_fSizeX * 0.5f + 2.f, g_iWinSizeY * 0.5f - m_fCurPosY - 4.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.4f);
	return S_OK;
}

HRESULT CUI_Shield_Bar::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Shield"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CUI_Shield_Bar* CUI_Shield_Bar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Shield_Bar* pInstance = new CUI_Shield_Bar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Shield_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Shield_Bar::Clone(void* pArg)
{
	CUI_Shield_Bar* pInstance = new CUI_Shield_Bar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Shield_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Shield_Bar::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
