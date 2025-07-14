#include "PhaselockHand.h"

#include "GameInstance.h"

CPhaselockHand::CPhaselockHand(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSpriteEffect{ pDevice, pContext }
{

}

CPhaselockHand::CPhaselockHand(const CPhaselockHand& Prototype)
	: CSpriteEffect( Prototype )
{

}

HRESULT CPhaselockHand::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPhaselockHand::Initialize(void* pArg)
{
	CPhaselockHand::DESC* Desc = static_cast<CPhaselockHand::DESC*>(pArg);

	m_fLastTime = Desc->fLastTime;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CPhaselockHand::Priority_Update(_float fTimeDelta)
{

}

EVENT CPhaselockHand::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	m_pTransformCom->Scaling(4.f, 4.f, 4.f);
	m_pTransformCom->Set_Matrix(m_pTransformCom->Billboard());

	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc >= m_fLastTime)
	{
		m_bDead = true;
		return EVN_DEAD;
	}
	
	return EVN_NONE;
}

void CPhaselockHand::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CPhaselockHand::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(POSTEX_PHASELOCK_DISTORT)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPhaselockHand::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_PhaseLockBubble"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPhaselockHand::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	/* dx9 : 장치에 뷰, 투영행렬을 저장해두면 렌더링시 알아서 정점에 Transform해주었다. */
	/* dx11 : 셰이더에 뷰, 투영행렬을 저장해두고 우리가 직접 변환해주어야한다. */

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;



	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeAcc", &m_fTimeAcc, sizeof(_float))))
		return E_FAIL;



	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;

	return S_OK;
}

CPhaselockHand* CPhaselockHand::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPhaselockHand* pInstance = new CPhaselockHand(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPhaselockHand::Clone(void* pArg)
{
	CPhaselockHand* pInstance = new CPhaselockHand(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPhaselockHand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPhaselockHand::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
