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
#define IFILEDIALOG ImGuiFileDialog::Instance()

	SetNextWindowSize(ImVec2(200, 300));
	Begin("Convert Tools", &m_pWindowData->ShowConvertMenu, NULL);
	
	if (Button("Load File"))
	{
		IGFD::FileDialogConfig config;
		config.path = R"(C:\Users\CMAP\Documents\Dx11_Personal_Projects\3d\Borderlands2 Exports\Frost_StaticMesh)";
		config.countSelectionMax = 0; // ������

		IFILEDIALOG->OpenDialog("FBXDialog", "Select FBX Files", ".fbx", config);
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
					Convert_NonAnimFBX(FilePath.second.data());
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
	FBXDATA m_pFBXData = { };
	if (FAILED(Ready_FBXData(pModelFilePath, m_pFBXData)))
		return E_FAIL;
	string saveFileName = savePath + "\\" + m_pFBXData.strFBXName + ".bin";
	ofstream ofs(saveFileName, ios::binary);
	if (!ofs.is_open())
	{
		MSG_BOX("��Ѿ��");
		return E_FAIL;
	}

//	�߰��ٸ� �ؿ�
//****�޽�
	ofs.write(reinterpret_cast<const char*>(&m_pFBXData.iNumMeshes),sizeof(_uint)); // �޽� �?
	for (auto meshdata : m_pFBXData.vecMeshes)										// �޽� ��ü �� ���ƿ�
	{
		ofs.write(reinterpret_cast<const char*>(&meshdata.iMaterialIndex), sizeof(_uint));	// ���׸��� �ε��� �����ؿ�
		ofs.write(reinterpret_cast<const char*>(&meshdata.iNumVertices), sizeof(_uint));	// ���ؽ� ��� �����ؿ�
		ofs.write(reinterpret_cast<const char*>(&meshdata.iNumIndices), sizeof(_uint));		// �ε��� ��� �����ؿ�
		ofs.write(reinterpret_cast<const char*>(meshdata.vecIndices.data()), meshdata.iNumIndices * sizeof(_uint));		// �ε��� ���� �迭�� ��¥�����ؿ�
		ofs.write(reinterpret_cast<const char*>(meshdata.vecVertices.data()), meshdata.iNumVertices * sizeof(VTXMESH)); // ���ؽ� ���� ����ü �迭�� ��¥�����ؿ�~����
	}
//****���׸���
	ofs.write(reinterpret_cast<const char*>(&m_pFBXData.iNumMaterials),sizeof(_uint));	// ���׸��� �?
	for (auto material : m_pFBXData.vecMaterials)										// ���׸��� ��ü �� ���ƿ�
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
//****������¼����¼���� �̷��� ���� �����ٰ� ���� �ȴ�


}
HRESULT CConvertTool::Copy_MaterialTextures()
{
	try {
		for (auto srcPath : materialList)
		{

			_char       szFileName[MAX_PATH] = {};
			_char       szExt[MAX_PATH] = {}; // ������ �̸��̶� Ȯ���ڸ� ��������
		

			_splitpath_s(srcPath.string().c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			string dstPath = savePath + "\\" + szFileName + szExt;


			if (false == copy_file(srcPath, dstPath, copy_options::overwrite_existing))
				return E_FAIL;
		}
	}
	catch (const filesystem_error& e) {
		// 1) ���� ������ �ϳ��� std::string���� ����
		std::string msg = "���� ���� ����:\n";
		msg += e.what();
		msg += "\n\n�ڵ� ��: " + std::to_string(e.code().value());
		msg += "\n���� �޽���: " + e.code().message();
		msg += "\n���� ���: " + e.path1().string();
		msg += "\n��� ���: " + e.path2().string();

		// 2) MessageBoxA�� ���
		MessageBoxA(
			nullptr,
			msg.c_str(),
			"���� ���� ����",
			MB_OK | MB_ICONERROR
		);
		return false;
	}
	materialList.clear();
	return S_OK;
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


	//if (FAILED(Ready_Meshes()))
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
				_char       szExt[MAX_PATH] = {}; // ������ �̸��̶� Ȯ���ڸ� ��������

				_char		szFBXName[MAX_PATH] = {};

				_splitpath_s(pModelFilePath, nullptr, 0, nullptr, 0, szFBXName, MAX_PATH, nullptr, 0);
				m_pFBXData.strFBXName = szFBXName; // ������ ���� �̸� �����ϰ� fbx���� �� �����ؿ�

				_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

				path relPath = strTexturePath.data;
				path basePath = R"(C:\Users\CMAP\Documents\Dx11_Personal_Projects\3d\Borderlands2 Exports\Borderlands2_ALL\)";
				path absPath = basePath / relPath;
				absPath = absPath.lexically_normal();

				materialList.push_back(absPath); // ���߿� ������ �� ����ٿ��ֱ� ���� �ؽ��ĵ� ��� ��Ƶ��� 

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
