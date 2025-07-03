#include "UI_HPShieldPannel.h"
#include "GameInstance.h"
#include "UI_HP.h"
#include "UI_Shield.h"

CUI_HPShieldPannel::CUI_HPShieldPannel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CUI_HPShieldPannel::CUI_HPShieldPannel(const CUI_HPShieldPannel& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CUI_HPShieldPannel::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_HPShieldPannel::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI_HPShieldPannel::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Priority_Update(fTimeDelta);
	}
}

EVENT CUI_HPShieldPannel::Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return EVN_NONE;
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Update(fTimeDelta);
	}
	return EVN_NONE;
}

void CUI_HPShieldPannel::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return ;
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Late_Update(fTimeDelta);
	}
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI_HPShieldPannel::Render()
{

	return S_OK;
}

HRESULT CUI_HPShieldPannel::Ready_PartObjects(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	CUI_HP::DESC			UIHPDesc;
	UIHPDesc.fX = 180.f;
	UIHPDesc.fY = 650.f;
	UIHPDesc.fSizeX = 240.f;
	UIHPDesc.fSizeY = 36.0f;
	UIHPDesc.iMaxHP = pDesc->iMaxHP;
	UIHPDesc.iHP = pDesc->iHP;
	UIHPDesc.iUIDepth = ENUM_CLASS(UI_DEPTH::UI_HPBAR);
	UIHPDesc.pParentObject = this;
	UIHPDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix4x4Ptr();
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("PartObject_HP"), TEXT("Prototype_GameObject_UI_HP"), &UIHPDesc)))
		return E_FAIL;


	CUI_Shield::DESC			UIShieldDesc;
	UIShieldDesc.fX = 23.5f;
	UIShieldDesc.fY = 15.f;
	UIShieldDesc.fSizeX = 193.f;
	UIShieldDesc.fSizeY = 34.0f;
	UIShieldDesc.fShield = pDesc->fShield;
	UIShieldDesc.fMaxShield = pDesc->fMaxShield;
	UIShieldDesc.pParentObject = this;
	UIShieldDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix4x4Ptr();
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("PartObject_Shield"), TEXT("Prototype_GameObject_UI_Shield"), &UIShieldDesc)))
		return E_FAIL;

	return S_OK;
}

CUI_HPShieldPannel* CUI_HPShieldPannel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_HPShieldPannel* pInstance = new CUI_HPShieldPannel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_HPShieldPannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_HPShieldPannel::Clone(void* pArg)
{
	CUI_HPShieldPannel* pInstance = new CUI_HPShieldPannel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_HPShieldPannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_HPShieldPannel::Free()
{
	__super::Free();
}
