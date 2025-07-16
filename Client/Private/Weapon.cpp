#include "Weapon.h"

#include "GameInstance.h"

CWeapon::CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CWeapon::CWeapon(const CWeapon& Prototype)
	: CGameObject( Prototype )
{

}

HRESULT CWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon::Initialize(void* pArg)
{
	//DESC			Desc{};

	//Desc.fRotationPerSec = 0.f;
	//Desc.fSpeedPerSec = 0.f;
	//lstrcpy(Desc.szName, TEXT("Weapon"));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_pPlayerModel = dynamic_cast<CModel*>(m_pParentObject->Get_Component(L"Com_Model"));
	m_pPlayerTransform = m_pParentObject->Get_Transform();
	m_szPlayerCameraBoneName = "R_Weapon_Bone";

	_int iWeaponBoneIndex = m_pPlayerModel->Find_BoneIndex(m_szPlayerCameraBoneName.c_str());
	if (iWeaponBoneIndex < 0) 
		return E_FAIL;
	// ÇÃ·¹ÀÌ¾î ¼ÕÀÇ weapon »À ÀÎµ¦½º °®°í¿È 

	m_pSocketMatrix = m_pPlayerModel->Get_CombinedTransformationMatrix(iWeaponBoneIndex);
	// weapon »ÀÀÇ combinedtransformationmatrix°®¿È
	m_pParentMatrix = m_pPlayerTransform->Get_WorldMatrix4x4Ptr();

	return S_OK;
}

void CWeapon::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Priority_Update(fTimeDelta);
	}
}

EVENT CWeapon::Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return EVN_NONE;
	_matrix matFinal = XMLoadFloat4x4(m_pSocketMatrix) * XMLoadFloat4x4(m_pParentMatrix);

	m_pTransformCom->Set_Matrix(matFinal);
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Update(fTimeDelta);
	}
	return EVN_NONE;
}

void CWeapon::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return ;
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Late_Update(fTimeDelta);
	}
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CWeapon::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
	//	return E_FAIL;

	//const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_Light(0);

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
	//	return E_FAIL;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			continue;
		//m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0);
		m_pModelCom->Bind_Bone_Matrices(m_pShaderCom, "g_BoneMatrices", i);
		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}
	return S_OK;
}

void CWeapon::Reload_Magazine(_int& iPlayerAmmo)
{
	if (iPlayerAmmo < m_iMagazineSize)
	{
		m_iCurAmmoLeft = iPlayerAmmo;
		iPlayerAmmo = 0;
	}
	else
	{
		iPlayerAmmo -= m_iMagazineSize - m_iCurAmmoLeft;
		m_iCurAmmoLeft = m_iMagazineSize;
	}
}

HRESULT CWeapon::Ready_Components(void* pArg)
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"), // ÃÑ Àü¿ë ¼ÎÀÌ´õ·Î ¹Ù²Ù´øÁö ÆÐ½º¸¦ ¹Ù²Ù´øÁö
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Sound */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Sound_Weapon"),
		TEXT("Com_Sound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	return S_OK;
}

void CWeapon::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pSoundCom);
}
