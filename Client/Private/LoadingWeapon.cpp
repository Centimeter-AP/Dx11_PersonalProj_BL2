#include "LoadingWeapon.h"
#include "GameInstance.h"

CLoadingWeapon::CLoadingWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CLoadingWeapon::CLoadingWeapon(const CLoadingWeapon& Prototype)
	: CGameObject( Prototype )
{

}

HRESULT CLoadingWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoadingWeapon::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);
	if (pDesc == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;


	//m_pTransformCom->Scaling(100.f, 100.f, 100.f);
	m_pModelCom->Set_Animation(0, false);
	m_pModelCom->Set_Animation_TickPerSecond_All(0.001f);
	m_pModelCom->Play_Animation(0.01f);
	return S_OK;
}

void CLoadingWeapon::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * 20.f);
}

EVENT CLoadingWeapon::Update(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);
	m_pTransformCom->Set_Matrix(XMMatrixIdentity());
	static _float loadingAngle = 0;
	loadingAngle += fTimeDelta;  // 매 프레임 회전
	_matrix world = XMMatrixScaling(0.008f, 0.008f, 0.008f) * XMMatrixRotationY(loadingAngle) * XMMatrixTranslation(1.3f, -0.8f, 3.0f);
	m_pTransformCom->Set_Matrix(world);
	return __super::Update(fTimeDelta);
}

void CLoadingWeapon::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONLIGHT, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CLoadingWeapon::Render()
{
	_float4x4 ViewMatrix, ProjMatrix = {};

	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&ProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 1280.f / 720.f, 0.1f, 100.f));


	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &ProjMatrix)))
		return E_FAIL;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			continue;
		m_pModelCom->Bind_Bone_Matrices(m_pShaderCom, "g_BoneMatrices", i);
		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLoadingWeapon::Ready_Components(void* pArg)
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_LoadingWeapon"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;


	return S_OK;
}

CLoadingWeapon* CLoadingWeapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLoadingWeapon* pInstance = new CLoadingWeapon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoadingWeapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLoadingWeapon::Clone(void* pArg)
{
	CLoadingWeapon* pInstance = new CLoadingWeapon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLoadingWeapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoadingWeapon::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
