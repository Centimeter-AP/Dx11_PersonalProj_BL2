#include "UI_AmmoPannel.h"
#include "GameInstance.h"
#include "UI_Ammo.h"
#include "UI_Ammo_Bar.h"

CUI_AmmoPannel::CUI_AmmoPannel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CUI_AmmoPannel::CUI_AmmoPannel(const CUI_AmmoPannel& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CUI_AmmoPannel::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_AmmoPannel::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-10.f));

	if (FAILED(Ready_PartObjects(pArg)))
		return E_FAIL;


	return S_OK;
}

void CUI_AmmoPannel::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Priority_Update(fTimeDelta);
	}
}

EVENT CUI_AmmoPannel::Update(_float fTimeDelta)
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

void CUI_AmmoPannel::Late_Update(_float fTimeDelta)
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

HRESULT CUI_AmmoPannel::Render()
{

	return S_OK;
}

HRESULT CUI_AmmoPannel::Ready_PartObjects(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	CUI_Ammo::DESC			UIAmmoDesc;
	UIAmmoDesc.fX = g_iWinSizeX * 0.5f;
	UIAmmoDesc.fY = g_iWinSizeY * 0.5f;
	UIAmmoDesc.fSizeX = 240.f;
	UIAmmoDesc.fSizeY = 36.0f;
	UIAmmoDesc.iUIDepth = ENUM_CLASS(UI_DEPTH::UI_AMMO);
	UIAmmoDesc.pParentObject = this;
	UIAmmoDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix4x4Ptr();
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("PartObject_Ammo"), TEXT("Prototype_GameObject_UI_Ammo"), &UIAmmoDesc)))
		return E_FAIL;

	CUI_Ammo_Bar::DESC	UIAmmoBarDesc;
	UIAmmoBarDesc.fX = g_iWinSizeX * 0.5f;
	UIAmmoBarDesc.fY = g_iWinSizeY * 0.5f;
	UIAmmoBarDesc.fSizeX = 198.f;
	UIAmmoBarDesc.fSizeY = 36.0f;
	UIAmmoBarDesc.iAmmo = pDesc->iAmmo;
	UIAmmoBarDesc.iMaxAmmo = pDesc->iMaxAmmo;
	UIAmmoBarDesc.iUIDepth = ENUM_CLASS(UI_DEPTH::UI_AMMOBAR);
	UIAmmoBarDesc.pParentObject = this;
	UIAmmoBarDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix4x4Ptr();
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("PartObject_Ammo_Bar"), TEXT("Prototype_GameObject_UI_Ammo_Bar"), &UIAmmoBarDesc)))
		return E_FAIL;

	return S_OK;
}

CUI_AmmoPannel* CUI_AmmoPannel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_AmmoPannel* pInstance = new CUI_AmmoPannel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_HPShieldPannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_AmmoPannel::Clone(void* pArg)
{
	CUI_AmmoPannel* pInstance = new CUI_AmmoPannel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_HPShieldPannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_AmmoPannel::Free()
{
	__super::Free();
}
