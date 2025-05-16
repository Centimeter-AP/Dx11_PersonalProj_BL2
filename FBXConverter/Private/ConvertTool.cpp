#include "GameInstance.h"
#include "ConvertTool.h"

//ImGuiFileDialog g_ImGuiFileDialog;
//ImGuiFileDialog::Instance() 이래 싱글톤으로 쓰라고 신이 말하고 감


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
		config.countSelectionMax = 0; // 무제한

		IFILEDIALOG->OpenDialog("FBXDialog", "Select FBX Files", ".fbx", config);
	}

	if (IFILEDIALOG->Display("FBXDialog"))
	{
		if (IFILEDIALOG->IsOk())
		{
			auto selections = IFILEDIALOG->GetSelection();
			// 처리
			// first: 파일명.확장자
			// second: 전체 경로 (파일명포함)
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
		MSG_BOX("숏됏어요");
		return E_FAIL;
	}

//	야가다를 해요
//****메쉬
	ofs.write(reinterpret_cast<const char*>(&m_pFBXData.iNumMeshes),sizeof(_uint)); // 메쉬 몇개?
	for (auto meshdata : m_pFBXData.vecMeshes)										// 메쉬 전체 다 돌아요
	{
		ofs.write(reinterpret_cast<const char*>(&meshdata.iMaterialIndex), sizeof(_uint));	// 머테리얼 인덱스 저장해요
		ofs.write(reinterpret_cast<const char*>(&meshdata.iNumVertices), sizeof(_uint));	// 버텍스 몇갠지 저장해요
		ofs.write(reinterpret_cast<const char*>(&meshdata.iNumIndices), sizeof(_uint));		// 인덱스 몇갠지 저장해요
		ofs.write(reinterpret_cast<const char*>(meshdata.vecIndices.data()), meshdata.iNumIndices * sizeof(_uint));		// 인덱스 벡터 배열로 통짜저장해요
		ofs.write(reinterpret_cast<const char*>(meshdata.vecVertices.data()), meshdata.iNumVertices * sizeof(VTXMESH)); // 버텍스 벡터 구조체 배열도 통짜저장해요~ㅋㅋ
	}
//****머테리얼
	ofs.write(reinterpret_cast<const char*>(&m_pFBXData.iNumMaterials),sizeof(_uint));	// 머테리얼 몇개?
	for (auto material : m_pFBXData.vecMaterials)										// 머테리얼 전체 다 돌아요
	{
		_uint numSRVs = material.size();
		ofs.write(reinterpret_cast<const char*>(&numSRVs), sizeof(_uint));						// 머테리얼 안에 srv가 몇개 있는지 저장해요(구조체에없으니까조심!!)			
		for (auto matdata : material)															// 저장해요	
		{
			_uint texType = static_cast<_uint>(matdata.eTexType);								// 텍스쳐 타입 uint로 바꺼줘요
			ofs.write(reinterpret_cast<const char*>(&texType), sizeof(_uint));					// 저장해요
			_uint pathLength = static_cast<_uint>(matdata.strTexturePath.size());				// 문자열 길이 구해요
			ofs.write(reinterpret_cast<const char*>(&pathLength), sizeof(_uint));				// 문자열 길이 저장해요
			if (pathLength > 0)
				ofs.write(matdata.strTexturePath.data(), pathLength);							// 문자열 저장해요
		}							
	}
//****본과어쩌구저쩌구는 미래의 내가 뒤졌다고 보면 된다


}
HRESULT CConvertTool::Copy_MaterialTextures()
{
	try {
		for (auto srcPath : materialList)
		{

			_char       szFileName[MAX_PATH] = {};
			_char       szExt[MAX_PATH] = {}; // 저장은 이름이랑 확장자만 저장하자
		

			_splitpath_s(srcPath.string().c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			string dstPath = savePath + "\\" + szFileName + szExt;


			if (false == copy_file(srcPath, dstPath, copy_options::overwrite_existing))
				return E_FAIL;
		}
	}
	catch (const filesystem_error& e) {
		// 1) 에러 정보를 하나의 std::string으로 조합
		std::string msg = "파일 복사 실패:\n";
		msg += e.what();
		msg += "\n\n코드 값: " + std::to_string(e.code().value());
		msg += "\n에러 메시지: " + e.code().message();
		msg += "\n원본 경로: " + e.path1().string();
		msg += "\n대상 경로: " + e.path2().string();

		// 2) MessageBoxA로 출력
		MessageBoxA(
			nullptr,
			msg.c_str(),
			"파일 복사 오류",
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
		MSG_BOX("어심프가 쓰러졌다 !!! 무슨일이야 !!!");
		return E_FAIL;
	}


	//if (FAILED(Ready_Meshes()))
	m_pFBXData.iNumMeshes = m_pAIScene->mNumMeshes;						// 메쉬부터 채워요
	m_pFBXData.vecMeshes.reserve(m_pFBXData.iNumMeshes);
	for (size_t i = 0; i < m_pFBXData.iNumMeshes; i++)
	{
		FBX_MESHDATA tMesh = {};

		tMesh.iMaterialIndex = m_pAIScene->mMeshes[i]->mMaterialIndex;	// 사용하는 머테리얼 인덱스 번호
		tMesh.iNumVertices = m_pAIScene->mMeshes[i]->mNumVertices;		// 버텍스 갯수
		tMesh.iNumIndices = m_pAIScene->mMeshes[i]->mNumFaces * 3;		// 인덱스 갯수
		tMesh.vecVertices.reserve(tMesh.iNumVertices);					// 벡터 리저브 해줘요
		tMesh.vecIndices.reserve(tMesh.iNumIndices);

		for (size_t j = 0; j < tMesh.iNumVertices; j++)					// 정점 구조체 채워요
		{
			VTXMESH tVtxMesh;
			memcpy(&tVtxMesh.vPosition, &m_pAIScene->mMeshes[i]->mVertices[j], sizeof(_float3));
			memcpy(&tVtxMesh.vNormal, &m_pAIScene->mMeshes[i]->mNormals[j], sizeof(_float3));
			memcpy(&tVtxMesh.vTangent, &m_pAIScene->mMeshes[i]->mTangents[j], sizeof(_float3));
			memcpy(&tVtxMesh.vTexcoord, &m_pAIScene->mMeshes[i]->mTextureCoords[0][j], sizeof(_float2));
			tMesh.vecVertices.push_back(tVtxMesh);
		}

		for (size_t j = 0; j < m_pAIScene->mMeshes[i]->mNumFaces; j++)	// 인덱스 채워요
		{
			tMesh.vecIndices.push_back(m_pAIScene->mMeshes[i]->mFaces[j].mIndices[0]);
			tMesh.vecIndices.push_back(m_pAIScene->mMeshes[i]->mFaces[j].mIndices[1]);
			tMesh.vecIndices.push_back(m_pAIScene->mMeshes[i]->mFaces[j].mIndices[2]);
		}

		m_pFBXData.vecMeshes.push_back(tMesh);							// 밥먹어
	}

	m_pFBXData.iNumMaterials = m_pAIScene->mNumMaterials;				// 머테리얼 채워요
	for (size_t i = 0; i < m_pFBXData.iNumMaterials; i++)
	{
		vector<FBX_MATDATA> tMaterialData;
		for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)				// 죳뺑이쳐요
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
				_char       szExt[MAX_PATH] = {}; // 저장은 이름이랑 확장자만 저장하자

				_char		szFBXName[MAX_PATH] = {};

				_splitpath_s(pModelFilePath, nullptr, 0, nullptr, 0, szFBXName, MAX_PATH, nullptr, 0);
				m_pFBXData.strFBXName = szFBXName; // 내보낼 파일 이름 저장하게 fbx파일 명 저장해요

				_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

				path relPath = strTexturePath.data;
				path basePath = R"(C:\Users\CMAP\Documents\Dx11_Personal_Projects\3d\Borderlands2 Exports\Borderlands2_ALL\)";
				path absPath = basePath / relPath;
				absPath = absPath.lexically_normal();

				materialList.push_back(absPath); // 나중에 저장할 때 복사붙여넣기 해줄 텍스쳐들 경로 모아두자 

				strcat_s(szFullPath, szFileName);
				strcat_s(szFullPath, szExt);

				tMat.strTexturePath = szFullPath;					// 텍스쳐 이름이랑 확장자만 저장
				tMat.eTexType = static_cast<aiTextureType>(j);		// 텍스쳐타입 뭐였는지 저장하시고
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
