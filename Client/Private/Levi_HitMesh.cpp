#include "Levi_HitMesh.h"
#include "Leviathan.h"
#include "Model.h"

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
}

void CLevi_HitMesh::Priority_Update(_float fTimeDelta)
{
}

EVENT CLevi_HitMesh::Update(_float fTimeDelta)
{
    return EVENT();
}

void CLevi_HitMesh::Late_Update(_float fTimeDelta)
{
}

HRESULT CLevi_HitMesh::Render()
{
    return S_OK;
}

HRESULT CLevi_HitMesh::Ready_Components(void* pArg)
{
    return S_OK;
}

HRESULT CLevi_HitMesh::Bind_ShaderResources()
{
    return S_OK;
}

void CLevi_HitMesh::Initialize_BasicStatus(_int iLevel)
{
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
}
