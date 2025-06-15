#include "Bullet.h"
#include "GameInstance.h"

CBullet::CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject{pDevice, pContext}
{
}

CBullet::CBullet(const CBullet& Prototype)
    : CGameObject(Prototype)
{
}

HRESULT CBullet::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBullet::Initialize(void* pArg)
{
	//DESC			Desc{};

	//Desc.fRotationPerSec = 0.f;
	//Desc.fSpeedPerSec = 0.f;
	//lstrcpy(Desc.szName, TEXT("Weapon"));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBullet::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
}

EVENT CBullet::Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return EVN_NONE;

	return EVN_NONE;
}

void CBullet::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CBullet::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_Light(0);

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

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

HRESULT CBullet::Ready_Components(void* pArg)
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"), // 총 전용 셰이더로 바꾸던지 패스를 바꾸던지
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

void CBullet::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
