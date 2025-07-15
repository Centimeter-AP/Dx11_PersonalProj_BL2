#include "PhaselockSwirl.h"
#include "GameInstance.h"

CPhaselockSwirl::CPhaselockSwirl(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSpriteEffect{ pDevice, pContext }
{

}

CPhaselockSwirl::CPhaselockSwirl(const CPhaselockSwirl& Prototype)
	: CSpriteEffect( Prototype )
{

}

HRESULT CPhaselockSwirl::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPhaselockSwirl::Initialize(void* pArg)
{
	CPhaselockSwirl::DESC* Desc = static_cast<CPhaselockSwirl::DESC*>(pArg);

	//m_fLastTime = Desc->fLastTime;
	m_pSocketMatrix = Desc->pSocketMatrix;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Matrix(XMMatrixIdentity());
	m_pTransformCom->Scaling(1.f, 1.f, 1.f);

	return S_OK;
}

void CPhaselockSwirl::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
}

EVENT CPhaselockSwirl::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	if (m_isActive == false)
		return EVN_NONE;

	m_pTransformCom->Scaling_Ratio(0.93f, 0.93f, 0.93f);

	if (m_pSocketMatrix != nullptr)
	{
		_matrix		BoneMatrix = XMLoadFloat4x4(m_pSocketMatrix);

		for (size_t i = 0; i < 3; i++)
			BoneMatrix.r[i] = XMVector3Normalize(BoneMatrix.r[i]);


		XMStoreFloat4x4(&m_CombinedWorldMatrix,
			XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix4x4Ptr()) * BoneMatrix * XMLoadFloat4x4(m_pParentMatrix)
		);
	}
	XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Compute_Billboard(XMLoadFloat4x4(&m_CombinedWorldMatrix)));
	XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Rotation_Billboard(XMLoadFloat3(reinterpret_cast<_float3*>(m_CombinedWorldMatrix.m[2])), 
		XMLoadFloat4x4(&m_CombinedWorldMatrix), m_fAngle));
	m_fAngle += fTimeDelta;

	//XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Turn_Billboard(XMLoadFloat4x4(&m_CombinedWorldMatrix), fTimeDelta * 200.f));
	return EVN_NONE;
}

void CPhaselockSwirl::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return ;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CPhaselockSwirl::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(POSTEX_SOFTEFFECT)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CPhaselockSwirl::Show_Effect()
{
	m_isActive = true;
	m_bDead = false;
	//m_pTransformCom->Scaling(4.f, 4.f, 4.f);
}

HRESULT CPhaselockSwirl::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_EnergySwirl"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	
	///* For.Com_Texture */
	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_EnergySwirl"),
	//	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureSwirlCom))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CPhaselockSwirl::Bind_ShaderResources()
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

	/* dx9 : 장치에 뷰, 투영행렬을 저장해두면 렌더링시 알아서 정점에 Transform해주었다. */
	/* dx11 : 셰이더에 뷰, 투영행렬을 저장해두고 우리가 직접 변환해주어야한다. */

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeAcc", &m_fTimeAcc, sizeof(_float))))
	//	return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;

	return S_OK;
}

CPhaselockSwirl* CPhaselockSwirl::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPhaselockSwirl* pInstance = new CPhaselockSwirl(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPhaselockSwirl::Clone(void* pArg)
{
	CPhaselockSwirl* pInstance = new CPhaselockSwirl(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPhaselockSwirl");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPhaselockSwirl::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureSwirlCom);
}
