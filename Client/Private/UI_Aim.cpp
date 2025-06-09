#include "UI_Aim.h"
#include "GameInstance.h"

CUI_Aim::CUI_Aim(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CUI_Aim::CUI_Aim(const CUI_Aim& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CUI_Aim::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Aim::Initialize(void* pArg)
{
	UI_AIM_DESC* pDesc = static_cast<UI_AIM_DESC*>(pArg);

	pDesc->fSizeX = 1.f;
	pDesc->fSizeY = 1.f;
	pDesc->fX = g_iWinSizeX * 0.5f;
	pDesc->fY = g_iWinSizeY * 0.5f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_Aim::Priority_Update(_float fTimeDelta)
{
}

EVENT CUI_Aim::Update(_float fTimeDelta)
{
    return EVENT();
}

void CUI_Aim::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Aim::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Aim::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Aim"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	return S_OK;

}

CUI_Aim* CUI_Aim::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Aim* pInstance = new CUI_Aim(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Aim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Aim::Clone(void* pArg)
{
	CUI_Aim* pInstance = new CUI_Aim(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Aim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Aim::Free()
{
}
