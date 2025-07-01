#include "UI_Phaselock.h"
#include "GameInstance.h"

CUI_Phaselock::CUI_Phaselock(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CUI_Phaselock::CUI_Phaselock(const CUI_Phaselock& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CUI_Phaselock::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Phaselock::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_Phaselock::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
}

EVENT CUI_Phaselock::Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return EVN_NONE;
	if (m_bCheckTime)
	{
		m_fTime += fTimeDelta;
		if (m_fTime >= m_fDuration)
		{
			m_fTime = 0.f;
			m_bCheckTime = false;
			m_isActive = false;
		}
	}

	return EVN_NONE;
}

void CUI_Phaselock::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return ;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CUI_Phaselock::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	for (size_t i = 0; i < TYPE_END; ++i)
	{
		if (nullptr == m_pTextureCom[i])
			continue;
		_float fPercentage = 0.f;
		if (i == TYPE_BAR)
		{
			fPercentage = m_fTime / m_fDuration;
		}
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fPercentage", &fPercentage, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pTextureCom[i]->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
			return E_FAIL;
	//if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
	//	return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(4)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Phaselock::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Phaselock_Back"),
		TEXT("Com_Texture_Bar"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_BARBACK]))))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Phaselock"),
		TEXT("Com_Texture_BarBack"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_BAR]))))
		return E_FAIL;

	return S_OK;
}

CUI_Phaselock* CUI_Phaselock::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Phaselock* pInstance = new CUI_Phaselock(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Phaselock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Phaselock::Clone(void* pArg)
{
	CUI_Phaselock* pInstance = new CUI_Phaselock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Phaselock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Phaselock::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	for (size_t i = 0; i < TYPE_END; ++i)
	{
		Safe_Release(m_pTextureCom[i]);
	}
}
