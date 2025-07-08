#include "WaterExplode.h"

#include "GameInstance.h"

CWaterExplode::CWaterExplode(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSpriteEffect{ pDevice, pContext }
{

}

CWaterExplode::CWaterExplode(const CWaterExplode& Prototype)
	: CSpriteEffect( Prototype )
{

}

HRESULT CWaterExplode::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWaterExplode::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_vColor = _float4(0.6f, 0.8f, 1.f, 1.f); // ÇÏ´Ã»ö
	//m_pTransformCom->Scaling(10.f, 10.f ,10.f);
	m_isActive = true;
	m_fOpacity = 1.f;
	return S_OK;
}

void CWaterExplode::Priority_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;
}

EVENT CWaterExplode::Update(_float fTimeDelta)
{
	if (!m_isActive)
		return EVN_NONE;
	if (m_bDead)
		return EVN_DEAD;

	m_fShowTicker += fTimeDelta;
	if (m_fShowTicker >= 0.9f)
	{
		m_vColor.w -= fTimeDelta * 5.f;
		if (m_vColor.w <= 0.f)
			m_bDead = true;
	}

	_float3 vScale = m_pTransformCom->Get_Scaled();
	XMStoreFloat3(&vScale, XMLoadFloat3(&vScale) + (m_vMaxScale - XMLoadFloat3(&vScale)) * fTimeDelta * 8.f); /* (m_fShowTicker * 0.02f + 1.f));*/
	m_pTransformCom->Scaling(vScale);
	m_pTransformCom->Set_Matrix(m_pTransformCom->Billboard());

	return EVN_NONE;
}

void CWaterExplode::Late_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return ;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CWaterExplode::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(POSTEX_SOFTEFFECT_COLOR)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CWaterExplode::Show_Effect()
{
	m_isActive = true;
	m_fShowTicker = 0.f;
}

HRESULT CWaterExplode::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_WaterExplode_Spit_Dif"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CWaterExplode::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fOpacity", &m_fOpacity, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;

	return S_OK;
}

CWaterExplode* CWaterExplode::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWaterExplode* pInstance = new CWaterExplode(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWaterExplode::Clone(void* pArg)
{
	CWaterExplode* pInstance = new CWaterExplode(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWaterExplode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWaterExplode::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
