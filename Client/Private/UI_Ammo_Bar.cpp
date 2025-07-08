#include "UI_Ammo_Bar.h"
#include "GameInstance.h"

CUI_Ammo_Bar::CUI_Ammo_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CUI_Ammo_Bar::CUI_Ammo_Bar(const CUI_Ammo_Bar& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CUI_Ammo_Bar::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Ammo_Bar::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);
	
	m_iAmmo[CPlayer::WTYPE_AR] = pDesc->iARAmmo;
	m_iMaxAmmo[CPlayer::WTYPE_AR] = pDesc->iARMaxAmmo;
	m_iMaxMagazine[CPlayer::WTYPE_AR] = 26;
	m_iAmmo[CPlayer::WTYPE_PISTOL] = pDesc->iPstAmmo;
	m_iMaxAmmo[CPlayer::WTYPE_PISTOL] = pDesc->iPstMaxAmmo;
	m_iMaxMagazine[CPlayer::WTYPE_PISTOL] = 6;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix4x4Ptr()) * XMLoadFloat4x4(m_pParentMatrix));
	m_fCurPosX = m_CombinedWorldMatrix._41;
	m_fCurPosY = m_CombinedWorldMatrix._42;

	return S_OK;
}

void CUI_Ammo_Bar::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
}

EVENT CUI_Ammo_Bar::Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return EVN_NONE;

	return EVN_NONE;
}

void CUI_Ammo_Bar::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return ;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CUI_Ammo_Bar::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	_float fPercentage = 1.f - (*m_iAmmo[m_eWeaponType] / static_cast<_float>(m_iMaxMagazine[m_eWeaponType]));
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fPercentage", &fPercentage, sizeof(_float))))
		return E_FAIL;

	_float fOpacity = 1.f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fOpacity", &fOpacity, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Begin(POSTEX_UI_BLEND_RIGHT)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	_wstring strHP = to_wstring(*m_iAmmo[m_eWeaponType]);
	strHP += L" / ";
	strHP += to_wstring(*m_iMaxAmmo[m_eWeaponType]);
	_int len = strHP.length(); // max 8
	m_pGameInstance->Draw_Font(TEXT("Font_WillowBody"), strHP.c_str(),
		_float2(g_iWinSizeX * 0.5f + m_fCurPosX - (len - 8) * 12.f, g_iWinSizeY * 0.5f - m_fCurPosY), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.5f);


	return S_OK;
}

HRESULT CUI_Ammo_Bar::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Ammo"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CUI_Ammo_Bar* CUI_Ammo_Bar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Ammo_Bar* pInstance = new CUI_Ammo_Bar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Ammo_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Ammo_Bar::Clone(void* pArg)
{
	CUI_Ammo_Bar* pInstance = new CUI_Ammo_Bar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Ammo_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Ammo_Bar::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
