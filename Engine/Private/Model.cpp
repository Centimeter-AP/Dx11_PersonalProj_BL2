#include "Component.h"

#include "Animation.h"
#include "Material.h"
#include "Model.h"
#include "Mesh.h"
#include "Bone.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{

}

CModel::CModel(const CModel& Prototype)
	: CComponent { Prototype }	
	, m_iNumMeshes { Prototype.m_iNumMeshes }
	, m_Meshes { Prototype.m_Meshes }
	, m_iNumMaterials { Prototype.m_iNumMaterials }
	, m_Materials { Prototype.m_Materials }
	, m_eType { Prototype.m_eType }
	, m_PreTransformMatrix { Prototype.m_PreTransformMatrix }
	//, m_Bones { Prototype.m_Bones }
	, m_iNumAnimations { Prototype.m_iNumAnimations }
	// , m_Animations { Prototype.m_Animations }
{
	for (auto& pPrototypeBone : Prototype.m_Bones)
		m_Bones.push_back(pPrototypeBone->Clone());		

	for (auto& pMaterial : m_Materials)
		Safe_AddRef(pMaterial);

	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& pPrototypeAnim: Prototype.m_Animations)
		m_Animations.push_back(pPrototypeAnim->Clone());

}

HRESULT CModel::Bind_Material(CShader* pShader, const _char* pConstantName, _uint iMeshIndex, aiTextureType eType, _uint iTextureIndex)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;		

	_uint		iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	return m_Materials[iMaterialIndex]->Bind_ShaderResource(pShader, pConstantName, eType, iTextureIndex);	
}

HRESULT CModel::Bind_Bone_Matrices(CShader* pShader, const _char* pConstantName, _uint iMeshIndex)
{
	return m_Meshes[iMeshIndex]->Bind_Bone_Matrices(pShader, pConstantName, m_Bones);	
}

HRESULT CModel::Initialize_Prototype(MODEL eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{	

	_uint		iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	
	if (MODEL::NONANIM == eType)
		iFlag |= aiProcess_PreTransformVertices;

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);

	if (nullptr == m_pAIScene)
		return E_FAIL;

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

	m_eType = eType;

	if (FAILED(Ready_Bones(m_pAIScene->mRootNode, -1)))
		return E_FAIL;

	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;

	/* 각 애니메이션 마다 이용하고 있는 뼈대들의 시간에 맞는 상태값들을 미리 읽어서 저장해둔다. */
	if (FAILED(Ready_Animations()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{	
	m_Meshes[iMeshIndex]->Bind_Buffers();
	m_Meshes[iMeshIndex]->Render();	

	return S_OK;
}

_bool CModel::Play_Animation(_float fTimeDelta)
{
	_bool		isFinished = { false };
	/* 1. 현재 애니메이션에 맞는 뼈의 상태를 읽어와서 뼈의 TrnasformationMatrix를 갱신해준다. */
	isFinished  = m_Animations[m_iCurrentAnimIndex]->Update_Bones(fTimeDelta, m_Bones, m_isLoop);

	/* 2. 전체 뼐르 순회하면서 뼈들의 ColmbinedTransformationMatixf를 부모에서부터 자식으로 갱신해주낟. */
	for (auto& pBone : m_Bones)
	{
		pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
	}

	/*XMMatrixDecompose()*/

	return isFinished;
}

HRESULT CModel::Ready_Bones(const aiNode* pAINode, _int iParentBoneIndex)
{
	CBone* pBone = CBone::Create(pAINode, iParentBoneIndex);
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);

	_int		iParentIndex = m_Bones.size() - 1;

	for (size_t i = 0; i < pAINode->mNumChildren; i++)
	{
		Ready_Bones(pAINode->mChildren[i], iParentIndex);
	}
	return S_OK;
}

HRESULT CModel::Ready_Bones(FBXDATA& tModelData)
{
	for (size_t i = 0; i < tModelData.iNumBones; i++)
	{
		CBone* pBone = CBone::Create(tModelData.vecBones[i]);
		if (nullptr == pBone)
			return E_FAIL;

		m_Bones.push_back(pBone);
	}
	return S_OK;
}


HRESULT CModel::Ready_Meshes(FBXDATA& tModelData)
{
	m_iNumMeshes = tModelData.iNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, tModelData.vecMeshes[i], m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Meshes()
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, m_pAIScene->mMeshes[i], m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath, FBXDATA& tModelData)
{
	m_iNumMaterials = tModelData.iNumMaterials;

	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pContext, pModelFilePath, tModelData.vecMaterials[i]);
		if (nullptr == pMaterial)
			return E_FAIL;

		m_Materials.push_back(pMaterial);
	}
	return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pContext, pModelFilePath, m_pAIScene->mMaterials[i]);
		if (nullptr == pMaterial)
			return E_FAIL;

		m_Materials.push_back(pMaterial);
	}
	return S_OK;
}

HRESULT CModel::Read_BinaryFBX(const string& filepath, FBXDATA& out)
{
	ifstream ifs(filepath, ios::binary);
	if (!ifs.is_open()) {
		MSG_BOX("너는 파일 열기도 못하는구나");
		return E_FAIL;
	}

	// 1) 메시 개수 읽기
	_uint meshCount = 0;
	ifs.read(reinterpret_cast<_char*>(&meshCount), sizeof(meshCount));
	out.iNumMeshes = meshCount;
	out.vecMeshes.resize(meshCount);

	// 2) 각 메시 복원
	for (size_t i = 0; i < meshCount; ++i)
	{
		auto& mesh = out.vecMeshes[i];

		// 2-1) 고정 크기 값들
		ifs.read(reinterpret_cast<_char*>(&mesh.iMaterialIndex), sizeof(mesh.iMaterialIndex));
		ifs.read(reinterpret_cast<_char*>(&mesh.iNumVertices), sizeof(mesh.iNumVertices));
		ifs.read(reinterpret_cast<_char*>(&mesh.iNumIndices), sizeof(mesh.iNumIndices));

		// 2-2) 인덱스 벡터 복원
		mesh.vecIndices.resize(mesh.iNumIndices);
		if (mesh.iNumIndices > 0)
		{
			ifs.read(reinterpret_cast<_char*>(mesh.vecIndices.data()), mesh.iNumIndices * sizeof(_uint));
		}

		// 2-3) 버텍스 벡터 복원
		mesh.vecVertices.resize(mesh.iNumVertices);
		if (mesh.iNumVertices > 0)
		{
			ifs.read(reinterpret_cast<_char*>(mesh.vecVertices.data()), mesh.iNumVertices * sizeof(VTXMESH));
		}
	}

	// 3) 머티리얼 개수 읽기
	_uint matCount = 0;
	ifs.read(reinterpret_cast<_char*>(&matCount), sizeof(matCount));
	out.iNumMaterials = matCount;
	out.vecMaterials.resize(matCount);

	// 4) 각 머티리얼 복원
	for (size_t i = 0; i < matCount; ++i)
	{
		auto& mat = out.vecMaterials[i];
		// 4-0) 한 머테리얼 내 SRV갯수 읽기
		_uint numSRVs = 0;
		ifs.read(reinterpret_cast<_char*>(&numSRVs), sizeof(numSRVs));
		for (size_t j = 0; j < numSRVs; j++)
		{
			FBX_MATDATA tMatData = {};
			// 4-1) eTexType (enum) 복원
			_uint rawType = 0;
			ifs.read(reinterpret_cast<_char*>(&rawType), sizeof(rawType));
			tMatData.eTexType = static_cast<aiTextureType>(rawType);
			// → enum 은 저장 시 uint 로 변환했으므로, 같은 방법으로 읽고 캐스트합니다.

			// 4-3) 문자열 길이 읽기
			_uint pathLen = 0;
			ifs.read(reinterpret_cast<_char*>(&pathLen), sizeof(pathLen));

			// 4-4) 경로 문자열 읽기
			tMatData.strTexturePath.clear();
			if (pathLen > 0)
			{
				tMatData.strTexturePath.resize(pathLen);
				ifs.read(&tMatData.strTexturePath[0], pathLen);
			}
			// → string 내부는 연속 버퍼이므로 resize + &str[0] 로 읽어들입니다.
			mat.push_back(tMatData);
		}
	}
	if (ifs.good())
		return S_OK;
	else
		return E_FAIL;
}

HRESULT CModel::Ready_Animations()
{
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (size_t i = 0; i < m_iNumAnimations; i++)
	{
		CAnimation* pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], m_Bones);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}

	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, PreTransformMatrix)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& pMaterial : m_Materials)
		Safe_Release(pMaterial);
	m_Materials.clear();

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);	
	m_Meshes.clear();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);
	m_Animations.clear();

	m_Importer.FreeScene();
}
