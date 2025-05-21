#include "GameInstance.h"
#include "ConvertTool.h"

//ImGuiFileDialog g_ImGuiFileDialog;
//ImGuiFileDialog::Instance() �̷� �̱������� ����� ���� ���ϰ� ��


CConvertTool::CConvertTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImGuiTool{ pDevice, pContext }
{

}

CConvertTool::CConvertTool(const CConvertTool& Prototype)
	: CImGuiTool(Prototype)
{
}

HRESULT CConvertTool::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CConvertTool::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	savePath = R"(C:\Users\CMAP\Documents\Dx11_Personal_Projects\3d\testing)";


	return S_OK;
}

void CConvertTool::Priority_Update(_float fTimeDelta)
{

}

void CConvertTool::Update(_float fTimeDelta)
{
}

void CConvertTool::Late_Update(_float fTimeDelta)
{
}

HRESULT CConvertTool::Render()
{
	if (m_pWindowData->ShowConvertMenu)
	{
		if (FAILED(Render_ConvertTool()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CConvertTool::Render_ConvertTool()
{
	SetNextWindowSize(ImVec2(200, 300));
	Begin("Convert Tools", &m_pWindowData->ShowConvertMenu, NULL);
	
	IGFD::FileDialogConfig config;
	if (Button("Load NonAnim File"))
	{
		m_isAnim = false;
		config.path = R"(C:\Users\CMAP\Documents\Dx11_Personal_Projects\3d\Borderlands2 Exports\Borderlands2_ALL\Frost_P\StaticMesh3\FBX)";
		//config.path = R"(C:\Users\CMAP\Documents\Dx11_Personal_Projects\3d\Borderlands2 Exports\Frost_StaticMesh)";
		config.countSelectionMax = 0; // ������

		IFILEDIALOG->OpenDialog("FBXDialog", "Select Non Anim Staticmesh FBX Files", ".fbx", config);
	}

	if (Button("Load Anim File"))
	{
		m_isAnim = true;
		config.path = R"(C:\Users\CMAP\Documents\Dx11_Personal_Projects\3d\Borderlands2 Exports\Borderlands2_ALL\Frost_P\StaticMesh3\FBX)";
		config.countSelectionMax = 0; // ������

		IFILEDIALOG->OpenDialog("FBXDialog", "Select Anim Skeletalmesh FBX Files", ".fbx", config);
	}

	if (IFILEDIALOG->Display("FBXDialog"))
	{
		if (IFILEDIALOG->IsOk())
		{
			auto selections = IFILEDIALOG->GetSelection();
			// ó��
			// first: ���ϸ�.Ȯ����
			// second: ��ü ��� (���ϸ�����)
			if (!selections.empty())
			{
				for (auto FilePath : selections)
				{
					if (false == m_isAnim)
						Convert_NonAnimFBX(FilePath.second.data());
					else
						Convert_AnimFBX(FilePath.second.data());
				}
			}
			Copy_MaterialTextures();
		}
		IFILEDIALOG->Close();
	}

	SeparatorText("Find Object");
	End();
	return S_OK;
}

HRESULT CConvertTool::Convert_NonAnimFBX(const _char* pModelFilePath)
{
	FBXDATA m_tFBXData = { };
	if (FAILED(Ready_FBXData(pModelFilePath, m_tFBXData)))
		return E_FAIL;
	string saveFileName = savePath + "\\" + m_tFBXData.strFBXName + ".bin";
	ofstream ofs(saveFileName, ios::binary);
	if (!ofs.is_open())
	{
		MSG_BOX("��Ѿ��");
		return E_FAIL;
	}

//	�߰��ٸ� �ؿ�
//****�޽�
	ofs.write(reinterpret_cast<const char*>(&m_tFBXData.iNumMeshes),sizeof(_uint)); // �޽� �?
	for (auto meshdata : m_tFBXData.vecMeshes)										// �޽� ��ü �� ���ƿ�
	{
		ofs.write(reinterpret_cast<const char*>(&meshdata.iMaterialIndex), sizeof(_uint));	// ���׸��� �ε��� �����ؿ�
		ofs.write(reinterpret_cast<const char*>(&meshdata.iNumVertices), sizeof(_uint));	// ���ؽ� ��� �����ؿ�
		ofs.write(reinterpret_cast<const char*>(&meshdata.iNumIndices), sizeof(_uint));		// �ε��� ��� �����ؿ�
		ofs.write(reinterpret_cast<const char*>(meshdata.vecIndices.data()), meshdata.iNumIndices * sizeof(_uint));		// �ε��� ���� �迭�� ��¥�����ؿ�
		ofs.write(reinterpret_cast<const char*>(meshdata.vecVertices.data()), meshdata.iNumVertices * sizeof(VTXMESH)); // ���ؽ� ���� ����ü �迭�� ��¥�����ؿ�~����
	}
//****���׸���
	ofs.write(reinterpret_cast<const char*>(&m_tFBXData.iNumMaterials),sizeof(_uint));	// ���׸��� �?
	for (auto material : m_tFBXData.vecMaterials)										// ���׸��� ��ü �� ���ƿ�
	{
		_uint numSRVs = material.size();
		ofs.write(reinterpret_cast<const char*>(&numSRVs), sizeof(_uint));						// ���׸��� �ȿ� srv�� � �ִ��� �����ؿ�(����ü�������ϱ�����!!)			
		for (auto matdata : material)															// �����ؿ�	
		{
			_uint texType = static_cast<_uint>(matdata.eTexType);								// �ؽ��� Ÿ�� uint�� �ٲ����
			ofs.write(reinterpret_cast<const char*>(&texType), sizeof(_uint));					// �����ؿ�
			_uint pathLength = static_cast<_uint>(matdata.strTexturePath.size());				// ���ڿ� ���� ���ؿ�
			ofs.write(reinterpret_cast<const char*>(&pathLength), sizeof(_uint));				// ���ڿ� ���� �����ؿ�
			if (pathLength > 0)
				ofs.write(matdata.strTexturePath.data(), pathLength);							// ���ڿ� �����ؿ�
		}							
	}

	return S_OK;

}

HRESULT CConvertTool::Convert_AnimFBX(const _char* pModelFilePath)
{
	path ModelPath = pModelFilePath;
	path saveFileName = savePath / ModelPath.stem();
	saveFileName.replace_extension(".bin");
	ofstream ofs(saveFileName, ios::binary);
	if (!ofs.is_open())
	{
		MSG_BOX("��Ѿ��");
		return E_FAIL;
	}
	if (FAILED(Ready_FBXData(pModelFilePath, ofs)))
		return E_FAIL;

	int		iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;


	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);

	if (nullptr == m_pAIScene)
	{
		MSG_BOX("������� �������� !!! �������̾� !!!");
		return E_FAIL;
	}


/********************����********************/
/**** �� -> �޽� -> ���׸��� -> �ִϸ��̼� ****/
/*******************************************/

/**********�� ����**********/
	vector<string> BoneNames;
	Write_BoneData(m_pAIScene->mRootNode, -1, BoneNames, ofs);

/**********�޽� ����**********/
	ofs.write(reinterpret_cast<const char*>(&m_pAIScene->mNumMeshes), sizeof(_uint)); // �޽� �?
	for (size_t i = 0; i < m_pAIScene->mNumMeshes; i++)
	{
		aiMesh* pAIMesh = m_pAIScene->mMeshes[i];
		
		ofs.write(reinterpret_cast<const char*>(&pAIMesh->mName.length), sizeof(_uint));	// �޽� �̸� ���� �����ؿ�
		ofs.write(reinterpret_cast<const char*>(&pAIMesh->mName.data), pAIMesh->mName.length);		// �޽� �̸� �����ؿ�
		
		ofs.write(reinterpret_cast<const char*>(&pAIMesh->mMaterialIndex), sizeof(_uint));	// ���׸��� �ε��� �����ؿ�

		_uint iNumVertices = pAIMesh->mNumVertices;
		ofs.write(reinterpret_cast<const char*>(&iNumVertices), sizeof(_uint));	// ���ؽ� ��� �����ؿ�

		_uint iNumIndices = pAIMesh->mNumFaces * 3;
		ofs.write(reinterpret_cast<const char*>(&iNumIndices), sizeof(_uint));		// �ε��� ��� �����ؿ�

		vector<VTXANIMMESH> pVertices;
		pVertices.reserve(iNumVertices);
		for (size_t j = 0; j < iNumVertices; j++)
		{
			memcpy(&pVertices[j].vPosition, &pAIMesh->mVertices[j], sizeof(_float3));
			memcpy(&pVertices[j].vNormal, &pAIMesh->mNormals[j], sizeof(_float3));
			memcpy(&pVertices[j].vTangent, &pAIMesh->mTangents[j], sizeof(_float3));
			memcpy(&pVertices[j].vTexcoord, &pAIMesh->mTextureCoords[0][j], sizeof(_float2));
		}

		_uint iNumBones = pAIMesh->mNumBones;
		ofs.write(reinterpret_cast<const char*>(&iNumBones), sizeof(_uint));		// �� �޽��� ������ �ִ� ���� ��� �����ؿ�
		vector<_float4x4> OffsetMatrices;
		vector<_uint> BoneIndices;
		for (size_t j = 0; j < iNumBones; j++)
		{
			aiBone* pAIBone = pAIMesh->mBones[j];
			_float4x4		OffsetMatrix;
			memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
			XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));

			OffsetMatrices.push_back(OffsetMatrix);

			_uint	iBoneIndex = {};
			auto	iter = find_if(BoneNames.begin(), BoneNames.end(), [&](string BoneName)->_bool
				{
					if (!strcmp(BoneName.c_str(), pAIBone->mName.C_Str()))
						return true;

					++iBoneIndex;

					return false;
				});

			BoneIndices.push_back(iBoneIndex);
			_uint		iNumWeights = pAIBone->mNumWeights;
			ofs.write(reinterpret_cast<const char*>(&iNumWeights), sizeof(_uint));		// ������ ���� ������ ��ĥ �� �ִ� ���ؽ��� ������ �����ؿ� /* i��° ���� � �������� ������ �ִµ�?*/

			for (_uint k = 0; k < iNumWeights; k++)
			{
				/* i��° ���� ���⤷�� �ִ� k��° ������ ���� */
				aiVertexWeight	AIWeight = pAIBone->mWeights[k];

				if (0.f == pVertices[AIWeight.mVertexId].vBlendWeights.x)
				{
					/* �� �޽ÿ��� ������ �ִ� ���� �� i��° ���� �� �������� ������ �ֳ�. */
					pVertices[AIWeight.mVertexId].vBlendIndices.x = i;
					pVertices[AIWeight.mVertexId].vBlendWeights.x = AIWeight.mWeight;
				}

				else if (0.f == pVertices[AIWeight.mVertexId].vBlendWeights.y)
				{
					pVertices[AIWeight.mVertexId].vBlendIndices.y = i;
					pVertices[AIWeight.mVertexId].vBlendWeights.y = AIWeight.mWeight;
				}

				else if (0.f == pVertices[AIWeight.mVertexId].vBlendWeights.z)
				{
					pVertices[AIWeight.mVertexId].vBlendIndices.z = i;
					pVertices[AIWeight.mVertexId].vBlendWeights.z = AIWeight.mWeight;
				}

				else if (0.f == pVertices[AIWeight.mVertexId].vBlendWeights.w)
				{
					pVertices[AIWeight.mVertexId].vBlendIndices.w = i;
					pVertices[AIWeight.mVertexId].vBlendWeights.w = AIWeight.mWeight;
				}
			}
			if (0 == iNumBones)
			{
				iNumBones = 1;

				_uint	iBoneIndex = {};

				auto	iter = find_if(BoneNames.begin(), BoneNames.end(), [&](string BoneName)->_bool
					{
						if (!strcmp(BoneName.c_str(), pAIBone->mName.C_Str()))
							return true;

						++iBoneIndex;

						return false;
					});


				BoneIndices.push_back(iBoneIndex);

				_float4x4		OffsetMatrix;
				XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());
				OffsetMatrices.push_back(OffsetMatrix);
			}
		}

		ofs.write(reinterpret_cast<const char*>(OffsetMatrices.data()), sizeof(_float4x4) * OffsetMatrices.size());		// ������ ��� �迭 �����ؿ�
		ofs.write(reinterpret_cast<const char*>(BoneIndices.data()), sizeof(_uint) * BoneIndices.size());				// ������ ������ ��ġ�� ������ �ε����� ��������� ���� �迭 �����ؿ�
		ofs.write(reinterpret_cast<const char*>(pVertices.data()), iNumVertices * sizeof(VTXANIMMESH));					// ���ؽ� ���� ����ü �迭�� �����ؿ�
		
		vector<_uint> pIndices;
		pIndices.reserve(iNumIndices);

		for (size_t j = 0; j < m_pAIScene->mMeshes[i]->mNumFaces; j++)	// �ε��� ä����
		{
			pIndices.push_back(pAIMesh->mFaces[j].mIndices[0]);
			pIndices.push_back(pAIMesh->mFaces[j].mIndices[1]);
			pIndices.push_back(pAIMesh->mFaces[j].mIndices[2]);
		}
		ofs.write(reinterpret_cast<const char*>(pIndices.data()), iNumIndices * sizeof(_uint));		// �ε��� ���� �迭�� ��¥�����ؿ�
	}
	// �ƴ� ���� �ȴ��ݾ� �̰� �� �޽����? ����, 


/**********���׸��� ����**********/
	_uint iNumMaterials = m_pAIScene->mNumMaterials;								
	ofs.write(reinterpret_cast<const char*>(&iNumMaterials), sizeof(_uint));			// ���׸��� ��� �����ؿ�
	for (size_t i = 0; i < iNumMaterials; ++i)											// ���׸��� ��ü �� ���ƿ�
	{
		aiMaterial* pMaterial = m_pAIScene->mMaterials[i];
		vector<FBX_MATDATA> tMaterialData;
		for (_uint j = 1; j < AI_TEXTURE_TYPE_MAX; j++)				// �������Ŀ�
		{
			_uint iNumSRVs = pMaterial->GetTextureCount(static_cast<aiTextureType>(j));
			for (size_t k = 0; k < iNumSRVs; k++)
			{
				FBX_MATDATA tMat = {};
				aiString     strTexturePath;
				if (FAILED(m_pAIScene->mMaterials[i]->GetTexture(static_cast<aiTextureType>(j), k, &strTexturePath)))
					return E_FAIL;

				_char       szFullPath[MAX_PATH] = {};
				_char       szFileName[MAX_PATH] = {};
				_char       szDriveName[MAX_PATH] = {};
				_char       szDirName[MAX_PATH] = {};
				_char       szExt[MAX_PATH] = {}; // ������ �̸��̶� Ȯ���ڸ� ��������

				_char		szFBXName[MAX_PATH] = {};


				_splitpath_s(pModelFilePath, szDriveName, MAX_PATH, szDirName, MAX_PATH, szFBXName, MAX_PATH, nullptr, 0);
				_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

				path relPath = strTexturePath.data;
				path fbxTexturePath = (path)szDriveName / szDirName;
				fbxTexturePath /= relPath;
				fbxTexturePath = fbxTexturePath.lexically_normal();

				materialList.push_back(fbxTexturePath); // ���߿� ������ �� ����ٿ��ֱ� ���� �ؽ��ĵ� ��� ��Ƶ��� 

				strcat_s(szFullPath, szFileName);
				strcat_s(szFullPath, szExt);


				tMat.strTexturePath = szFullPath;					// �ؽ��� �̸��̶� Ȯ���ڸ� ����
				tMat.eTexType = static_cast<aiTextureType>(j);		// �ؽ���Ÿ�� �������� �����Ͻð�
				tMaterialData.push_back(tMat);

			}
		}
		_uint numSRVs = tMaterialData.size();
		ofs.write(reinterpret_cast<const char*>(&numSRVs), sizeof(_uint));						// ���׸��� �ȿ� srv�� � �ִ��� �����ؿ�
		for (auto matdata : tMaterialData)													
		{
			_uint texType = static_cast<_uint>(matdata.eTexType);								// �ؽ��� Ÿ�� uint�� �ٲ����
			ofs.write(reinterpret_cast<const char*>(&texType), sizeof(_uint));					// �����ؿ�
			_uint pathLength = static_cast<_uint>(matdata.strTexturePath.size());				// ���ڿ� ���� ���ؿ�
			ofs.write(reinterpret_cast<const char*>(&pathLength), sizeof(_uint));				// ���ڿ� ���� �����ؿ�
			if (pathLength > 0)
				ofs.write(matdata.strTexturePath.data(), pathLength);							// ���ڿ� �����ؿ�
		}
	}

	return S_OK;
}

HRESULT CConvertTool::Copy_MaterialTextures()
{
	for (auto srcPath : materialList)
	{
		try
		{
			_char       szFileName[MAX_PATH] = {};
			_char       szExt[MAX_PATH] = {}; // ������ �̸��̶� Ȯ���ڸ� ��������
		
			_splitpath_s(srcPath.string().c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			string dstPath = savePath + "\\" + szFileName + szExt;

			if (false == copy_file(srcPath, dstPath, copy_options::overwrite_existing))
				continue;
		}	
		catch (const filesystem_error& e)
		{
			// 1) ���� ������ �ϳ��� std::string���� ����
			std::string msg = "���� ���� ����:\n";
			msg += e.what();
			msg += "\n\n�ڵ� ��: " + std::to_string(e.code().value());
			msg += "\n���� �޽���: " + e.code().message();
			msg += "\n���� ���: " + e.path1().string();
			msg += "\n��� ���: " + e.path2().string();

			// 2) MessageBoxA�� ���
			MessageBoxA( nullptr, msg.c_str(), "���� ���� ����", MB_OK | MB_ICONERROR );
			continue;
		}          
	}
	materialList.clear();
	return S_OK;
}

HRESULT CConvertTool::Ready_FBXData(const _char* pModelFilePath, ofstream& ofs)
{
	return E_NOTIMPL;
}


HRESULT CConvertTool::Ready_FBXData(const _char* pModelFilePath, FBXDATA& m_pFBXData)
{
	int		iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast | aiProcess_PreTransformVertices;


	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);

	if (nullptr == m_pAIScene)
	{
		MSG_BOX("������� �������� !!! �������̾� !!!");
		return E_FAIL;
	}
	Ready_FBX_BoneData(m_pFBXData);
	Ready_FBX_MeshData(m_pFBXData);
	Ready_FBX_MaterialData(pModelFilePath, m_pFBXData);
	Ready_FBX_AnimationData(m_pFBXData);
	return S_OK;
}

HRESULT CConvertTool::Write_BoneData(const aiNode* pAINode, _int iParentBoneIndex, vector<string>& m_BoneNames, ostream& ofs)
{
	{ // �� Create
		ofs.write(reinterpret_cast<const char*>(&pAINode->mName.length), sizeof(_uint));	// �� �̸� ���� �����ؿ�
		ofs.write(reinterpret_cast<const char*>(&pAINode->mName.data), pAINode->mName.length);	// �� �̸� �����ؿ�
		m_BoneNames.push_back(pAINode->mName.data);

		_float4x4 TransformationMatrix = {};
		memcpy(&TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));
		XMStoreFloat4x4(&TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&TransformationMatrix)));
		ofs.write(reinterpret_cast<const char*>(&TransformationMatrix), sizeof(_float4x4));	// ��� �����ؿ�
		ofs.write(reinterpret_cast<const char*>(&iParentBoneIndex), sizeof(_int));			// �θ� �� �ε��� �����ؿ�
	}

	++m_iCurNumBones;
	_int		iParentIndex = m_iCurNumBones - 1;

	for (size_t i = 0; i < pAINode->mNumChildren; i++)
	{
		Write_BoneData(pAINode->mChildren[i], iParentIndex, m_BoneNames, ofs);
	}
	return S_OK;
}

HRESULT CConvertTool::Ready_FBX_MeshData(FBXDATA& m_pFBXData)
{
	m_pFBXData.iNumMeshes = m_pAIScene->mNumMeshes;						// �޽����� ä����
	m_pFBXData.vecMeshes.reserve(m_pFBXData.iNumMeshes);
	for (size_t i = 0; i < m_pFBXData.iNumMeshes; i++)
	{
		FBX_MESHDATA tMesh = {};

		tMesh.iMaterialIndex = m_pAIScene->mMeshes[i]->mMaterialIndex;	// ����ϴ� ���׸��� �ε��� ��ȣ
		tMesh.iNumVertices = m_pAIScene->mMeshes[i]->mNumVertices;		// ���ؽ� ����
		tMesh.iNumIndices = m_pAIScene->mMeshes[i]->mNumFaces * 3;		// �ε��� ����
		tMesh.vecVertices.reserve(tMesh.iNumVertices);					// ���� ������ �����
		tMesh.vecIndices.reserve(tMesh.iNumIndices);

		for (size_t j = 0; j < tMesh.iNumVertices; j++)					// ���� ����ü ä����
		{
			VTXMESH tVtxMesh;
			memcpy(&tVtxMesh.vPosition, &m_pAIScene->mMeshes[i]->mVertices[j], sizeof(_float3));
			memcpy(&tVtxMesh.vNormal, &m_pAIScene->mMeshes[i]->mNormals[j], sizeof(_float3));
			memcpy(&tVtxMesh.vTangent, &m_pAIScene->mMeshes[i]->mTangents[j], sizeof(_float3));
			memcpy(&tVtxMesh.vTexcoord, &m_pAIScene->mMeshes[i]->mTextureCoords[0][j], sizeof(_float2));
			tMesh.vecVertices.push_back(tVtxMesh);
		}

		for (size_t j = 0; j < m_pAIScene->mMeshes[i]->mNumFaces; j++)	// �ε��� ä����
		{
			tMesh.vecIndices.push_back(m_pAIScene->mMeshes[i]->mFaces[j].mIndices[0]);
			tMesh.vecIndices.push_back(m_pAIScene->mMeshes[i]->mFaces[j].mIndices[1]);
			tMesh.vecIndices.push_back(m_pAIScene->mMeshes[i]->mFaces[j].mIndices[2]);
		}

		m_pFBXData.vecMeshes.push_back(tMesh);							// ��Ծ�
	}
	return S_OK;
}

HRESULT CConvertTool::Ready_FBX_MaterialData(const _char* pModelFilePath, FBXDATA& m_pFBXData)
{

	m_pFBXData.iNumMaterials = m_pAIScene->mNumMaterials;				// ���׸��� ä����
	for (size_t i = 0; i < m_pFBXData.iNumMaterials; i++)
	{
		vector<FBX_MATDATA> tMaterialData;
		for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)				// �������Ŀ�
		{
			_uint iNumSRVs = m_pAIScene->mMaterials[i]->GetTextureCount(static_cast<aiTextureType>(j));
			for (size_t k = 0; k < iNumSRVs; k++)
			{
				FBX_MATDATA tMat = {};
				aiString     strTexturePath;
				if (FAILED(m_pAIScene->mMaterials[i]->GetTexture(static_cast<aiTextureType>(j), k, &strTexturePath)))
					return E_FAIL;

				_char       szFullPath[MAX_PATH] = {};
				_char       szFileName[MAX_PATH] = {};
				_char       szDriveName[MAX_PATH] = {};
				_char       szDirName[MAX_PATH] = {};
				_char       szExt[MAX_PATH] = {}; // ������ �̸��̶� Ȯ���ڸ� ��������

				_char		szFBXName[MAX_PATH] = {};


				_splitpath_s(pModelFilePath, szDriveName, MAX_PATH, szDirName, MAX_PATH, szFBXName, MAX_PATH, nullptr, 0);
				m_pFBXData.strFBXName = szFBXName; // ������ ���� �̸� �����ϰ� fbx���� �� �����ؿ�

				_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

				path relPath = strTexturePath.data;
				path fbxTexturePath = (path)szDriveName / szDirName;
				fbxTexturePath /= relPath;	
				fbxTexturePath = fbxTexturePath.lexically_normal();
				//path cleaned;
				//for (auto& part : fbxTexturePath) {
				//	if (part == "." || part == "..")
				//		continue;
				//	cleaned /= part;
				//}
				//
				//cleaned.lexically_normal();
				//path basePath = R"(C:\Users\CMAP\Documents\Dx11_Personal_Projects\3d\Borderlands2 Exports\Borderlands2_ALL\)";
				//path absPath = basePath / cleaned;
				//absPath = absPath.lexically_normal();

				materialList.push_back(fbxTexturePath); // ���߿� ������ �� ����ٿ��ֱ� ���� �ؽ��ĵ� ��� ��Ƶ��� 

				strcat_s(szFullPath, szFileName);
				strcat_s(szFullPath, szExt);

				tMat.strTexturePath = szFullPath;					// �ؽ��� �̸��̶� Ȯ���ڸ� ����
				tMat.eTexType = static_cast<aiTextureType>(j);		// �ؽ���Ÿ�� �������� �����Ͻð�
				tMaterialData.push_back(tMat);
			}
		}
		m_pFBXData.vecMaterials.push_back(tMaterialData);
	}
	return S_OK;
}

HRESULT CConvertTool::Ready_FBX_BoneData(FBXDATA& m_pFBXData)
{
	return S_OK;
}

HRESULT CConvertTool::Ready_FBX_AnimationData(FBXDATA& m_pFBXData)
{
	return S_OK;
}



CConvertTool* CConvertTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CConvertTool* pInstance = new CConvertTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CConvertTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CConvertTool::Clone(void* pArg)
{
	CConvertTool* pInstance = new CConvertTool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CConvertTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CConvertTool::Free()
{
	__super::Free();

}
