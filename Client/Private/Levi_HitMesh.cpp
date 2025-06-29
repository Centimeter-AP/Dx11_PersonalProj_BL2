#include "Levi_HitMesh.h"
#include "Leviathan.h"
#include "Model.h"
#include "GameInstance.h"

CLevi_HitMesh::CLevi_HitMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject{ pDevice, pContext }
{
}

CLevi_HitMesh::CLevi_HitMesh(const CLevi_HitMesh& Prototype)
    : CGameObject(Prototype)
{
}

HRESULT CLevi_HitMesh::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLevi_HitMesh::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Initialize_BasicStatus(1);

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	DESC* pDesc = static_cast<DESC*>(pArg);
	CModel* pParentModel = static_cast<CModel*>(m_pParentObject->Get_Component(L"Com_Model"));


	_int iWeaponBoneIndex = pParentModel->Find_BoneIndex(pDesc->strSocketMatrixName.c_str());
	if (iWeaponBoneIndex < 0)
		return E_FAIL;

	m_pSocketMatrix = pParentModel->Get_CombinedTransformationMatrix(iWeaponBoneIndex);

	return S_OK;
}

void CLevi_HitMesh::Priority_Update(_float fTimeDelta)
{
}

EVENT CLevi_HitMesh::Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return EVN_NONE;
	//auto test = m_pTransformCom->Get_WorldMatrix4x4Ref();
	_matrix matFinal = m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pSocketMatrix) * XMLoadFloat4x4(m_pParentMatrix);
	XMStoreFloat4x4(&m_CombinedWorldMatrix, matFinal);
	//m_pTransformCom->Set_Matrix(matFinal);
	//m_pColliderCom->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix)); 

	return EVN_NONE;
}

void CLevi_HitMesh::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CLevi_HitMesh::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
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
	m_pColliderCom->Set_ColliderColor(RGBA_GREEN);
	m_pColliderCom->Render();

	return S_OK;
}

HRESULT CLevi_HitMesh::Ready_Components(void* pArg)
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(m_iLevelID, TEXT("Prototype_Component_Model_Levi_HitMesh"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_Sphere::SPHERE_DESC SphereDesc = {};
	SphereDesc.pOwner = this;
	SphereDesc.eType = COLLIDER::SPHERE;
	SphereDesc.iColliderGroup = ENUM_CLASS(COL_GROUP::MONSTER);
	SphereDesc.iColliderID = ENUM_CLASS(COL_ID::MONSTER_BOSS_LEVIATHAN_HITMESH);
	SphereDesc.fRadius = 20.f;
	SphereDesc.vCenter = _float3(0.f, 0.f, 0.f);
	/* For.Com_ColliderHead */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"), 
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevi_HitMesh::Bind_ShaderResources()
{
    return S_OK;
}

void CLevi_HitMesh::Initialize_BasicStatus(_int iLevel)
{
	m_iHP = m_iMaxHP = 100; // Example value, adjust as needed
}

void CLevi_HitMesh::On_Collision(_uint iMyColID, _uint iHitColID, CCollider* pHitCol)
{

}

CLevi_HitMesh* CLevi_HitMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevi_HitMesh* pInstance = new CLevi_HitMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLevi_HitMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLevi_HitMesh::Clone(void* pArg)
{
	CLevi_HitMesh* pInstance = new CLevi_HitMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLevi_HitMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CLevi_HitMesh::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);
}
