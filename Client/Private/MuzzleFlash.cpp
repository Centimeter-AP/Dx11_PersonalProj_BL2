#include "MuzzleFlash.h"

#include "GameInstance.h"

CMuzzleFlash::CMuzzleFlash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSpriteEffect{ pDevice, pContext }
{

}

CMuzzleFlash::CMuzzleFlash(const CMuzzleFlash& Prototype)
	: CSpriteEffect( Prototype )
{

}

HRESULT CMuzzleFlash::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMuzzleFlash::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	//256 (512¶ó¼­)
	m_iTileX = 2;
	m_iTileY = 2;
	m_fTileSize.x = 1.0f / _float(m_iTileX);
	m_fTileSize.y = 1.0f / _float(m_iTileY);
	m_pTransformCom->Rotation(m_pTransformCom->Get_State(STATE::UP), XMConvertToRadians(90.f));
	m_pTransformCom->Scaling(0.5f, 0.5f, 0.5f);
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&pDesc->vPosOffset),1.f));
	//m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(1.f, -0.07f, 0.f, 1.f));
	m_iTileIdx = rand() % (m_iTileX * m_iTileY);
	m_fOffset.x = (m_iTileIdx % m_iTileX) * m_fTileSize.x;
	m_fOffset.y = (m_iTileIdx / m_iTileX) * m_fTileSize.y;

	m_isActive = false;
	return S_OK;
}

void CMuzzleFlash::Priority_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;
}

EVENT CMuzzleFlash::Update(_float fTimeDelta)
{
	if (!m_isActive)
		return EVN_NONE;
	else
	{
		m_fShowTicker += fTimeDelta;
		if (m_fShowTicker >= 0.05f)
		{
			m_iTileIdx = rand() % (m_iTileX * m_iTileY);
			m_fOffset.x = (m_iTileIdx % m_iTileX) * m_fTileSize.x;
			m_fOffset.y = (m_iTileIdx / m_iTileX) * m_fTileSize.y;
			m_fShowTicker = 0.f;
			m_isActive = false;
			m_pGameInstance->Delete_LastLight();
		}
		_vector scale, rotQuat, trans;
		XMMatrixDecompose(&scale, &rotQuat, &trans, XMLoadFloat4x4(m_pParentMatrix));
		_matrix rotMat = XMMatrixRotationQuaternion(rotQuat);
		_matrix transMat = XMMatrixTranslationFromVector(trans);
		_matrix matNoScale = rotMat * transMat;
		XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix4x4Ptr()) * matNoScale);
	}
	return EVN_NONE;
}

void CMuzzleFlash::Late_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return ;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_BLEND, this);
}

HRESULT CMuzzleFlash::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(POSTEX_GRID_PICK)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CMuzzleFlash::Show_Effect()
{
	m_isActive = true;
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, 1.f);
	LightDesc.fRange = 50.f;
	LightDesc.vDiffuse = _float4(0.7f, 0.49f, 0.1f, 1.f);
	LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 0.f);
	LightDesc.vSpecular = _float4(1.f, 0.78f, 0.6f, 0.f);

	m_pGameInstance->Add_Light(LightDesc);
}

HRESULT CMuzzleFlash::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_MuzzleFlash"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMuzzleFlash::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	//if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTileSize", &m_fTileSize, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTileOffset", &m_fOffset, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fOpacity", &m_fOpacity, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;

	return S_OK;
}

CMuzzleFlash* CMuzzleFlash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMuzzleFlash* pInstance = new CMuzzleFlash(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMuzzleFlash::Clone(void* pArg)
{
	CMuzzleFlash* pInstance = new CMuzzleFlash(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMuzzleFlash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMuzzleFlash::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
