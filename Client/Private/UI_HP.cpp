#include "UI_HP.h"
#include "GameInstance.h"
#include "UI_HP_Bar.h"

CUI_HP::CUI_HP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CUI_HP::CUI_HP(const CUI_HP& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CUI_HP::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_HP::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix4x4Ptr()) * XMLoadFloat4x4(m_pParentMatrix));

	//if (FAILED(Ready_PartObjects(pArg)))
	//	return E_FAIL;

	//m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 0.f, 0.f,1.f));
	//일단 이렇게 하니까 잘됨 , ㅡ홍승훈ㅡ
	// 포지션에 음수 들어가있음 그래서 렌더는 잘되는데 이상한 위치(마녀아님)에 있었음
	return S_OK;
}

void CUI_HP::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;

	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Priority_Update(fTimeDelta);
	}
}

EVENT CUI_HP::Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return EVN_NONE;

	//XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix4x4Ptr()) * XMLoadFloat4x4(m_pParentMatrix));

	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Update(fTimeDelta);
	}
	return EVN_NONE;
}

void CUI_HP::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return ;

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Late_Update(fTimeDelta);
	}
}

HRESULT CUI_HP::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	_float fPercentage = {};
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fPercentage", &fPercentage, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(POSTEX_ALPHABLEND)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;
	return S_OK;
}

HRESULT CUI_HP::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Left"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_HP::Ready_PartObjects(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);


	CUI_HP_Bar::DESC	UIHPDesc;
	UIHPDesc.fX = 180.f;
	UIHPDesc.fY = 650.f;
	UIHPDesc.fSizeX = 198.f;
	UIHPDesc.fSizeY = 36.0f;
	UIHPDesc.iHP = pDesc->iHP;
	UIHPDesc.iMaxHP = pDesc->iMaxHP;
	UIHPDesc.pParentObject = this;
	UIHPDesc.pParentMatrix = &m_CombinedWorldMatrix;
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("PartObject_HP_Bar"), TEXT("Prototype_GameObject_UI_HP_Bar"), &UIHPDesc)))
		return E_FAIL;

	return S_OK;
}

CUI_HP* CUI_HP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_HP* pInstance = new CUI_HP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_HP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_HP::Clone(void* pArg)
{
	CUI_HP* pInstance = new CUI_HP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_HP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_HP::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
