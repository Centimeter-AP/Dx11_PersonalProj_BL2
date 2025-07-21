#include "LoadingUI.h"
#include "LoadingWeapon.h"
#include "GameInstance.h"

CLoadingUI::CLoadingUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CLoadingUI::CLoadingUI(const CLoadingUI& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CLoadingUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoadingUI::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);
	m_iLevelTextureIdx = pDesc->iLevelTextureIdx;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	if (FAILED(Ready_LoadingWeapon()))
		return E_FAIL;

	return S_OK;
}

void CLoadingUI::Priority_Update(_float fTimeDelta)
{

}

EVENT CLoadingUI::Update(_float fTimeDelta)
{

	return EVN_NONE;
}

void CLoadingUI::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY, this);
}

HRESULT CLoadingUI::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iLevelTextureIdx)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingUI::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_LoadingUI"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingUI::Ready_LoadingWeapon()
{
	CLoadingWeapon::DESC				LoadingDesc{};

	LoadingDesc.iLevelID = ENUM_CLASS(LEVEL::LOADING);

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_LoadingWeapon"),
		ENUM_CLASS(LEVEL::LOADING), TEXT("Layer_LoadingUI"), &LoadingDesc)))
		return E_FAIL;
	return S_OK;
}

CLoadingUI* CLoadingUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLoadingUI* pInstance = new CLoadingUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoadingUI::Clone(void* pArg)
{
	CLoadingUI* pInstance = new CLoadingUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLoadingUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoadingUI::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
