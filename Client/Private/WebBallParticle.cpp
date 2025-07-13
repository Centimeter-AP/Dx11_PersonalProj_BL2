#include "WebBallParticle.h"

#include "GameInstance.h"

CWebBallParticle::CWebBallParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{

}

CWebBallParticle::CWebBallParticle(const CWebBallParticle& Prototype)
	: CPartObject( Prototype )
{

}

HRESULT CWebBallParticle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWebBallParticle::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	//m_pSocketMatrix = pDesc->pSocketMatrix;
	//m_pParentState = pDesc->pParentState;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CWebBallParticle::Priority_Update(_float fTimeDelta)
{

}

EVENT CWebBallParticle::Update(_float fTimeDelta)
{
	
	m_pVIBufferCom->Spread(fTimeDelta);
	if (m_pParentMatrix != nullptr)
	{
		
		_matrix		BoneMatrix = XMLoadFloat4x4(m_pParentMatrix);

		for (size_t i = 0; i < 3; i++)
			BoneMatrix.r[i] = XMVector3Normalize(BoneMatrix.r[i]);


		XMStoreFloat4x4(&m_CombinedWorldMatrix,
			XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix4x4Ptr()) * BoneMatrix/* * XMLoadFloat4x4(m_pParentMatrix)*/
		);
	}


	return EVN_NONE;
}

void CWebBallParticle::Late_Update(_float fTimeDelta)
{
	/* WeightBlend */	
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONLIGHT, this); 
}

HRESULT CWebBallParticle::Render()
{
	if (m_pParentMatrix != nullptr)
	{

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
			return E_FAIL;
	}
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
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

HRESULT CWebBallParticle::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosInstance"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_WebBallParticle"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_WebBall"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CWebBallParticle* CWebBallParticle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWebBallParticle* pInstance = new CWebBallParticle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWebBallParticle::Clone(void* pArg)
{
	CWebBallParticle* pInstance = new CWebBallParticle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWebBallParticle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWebBallParticle::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
