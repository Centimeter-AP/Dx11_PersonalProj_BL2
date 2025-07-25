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
		config.path = "../Bin/Resources/Textures_BL2/";
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

		}
		IFILEDIALOG->Close();
		
	}

	SeparatorText("Find Object");
	End();
	return S_OK;
}

HRESULT CConvertTool::Convert_NonAnimFBX(const _char* pModelFilePath)
{
	//int		iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast | aiProcess_PreTransformVertices;


	//m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);

	//if (nullptr == m_pAIScene)
	//{
	//	MSG_BOX("어심프가 쓰러졌다 !!! 무슨일이야 !!!");
	//	return E_FAIL;
	//}

	


	////if (FAILED(Ready_Meshes()))
	//m_iNumMeshes = m_pAIScene->mNumMeshes;

	//for (size_t i = 0; i < m_iNumMeshes; i++)
	//{
	//	CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, m_pAIScene->mMeshes[i], XMLoadFloat4x4(&m_PreTransformMatrix));
	//	if (nullptr == pMesh)
	//		return E_FAIL;

	//	m_Meshes.push_back(pMesh);
	//}



	////if (FAILED(Ready_Materials(pModelFilePath)))
	//m_iNumMaterials = m_pAIScene->mNumMaterials;

	//for (size_t i = 0; i < m_iNumMaterials; i++)
	//{
	//	CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pContext, pModelFilePath, m_pAIScene->mMaterials[i]);
	//	if (nullptr == pMaterial)
	//		return E_FAIL;

	//	m_Materials.push_back(pMaterial);
	//}


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
