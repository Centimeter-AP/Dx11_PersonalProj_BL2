#include "TerrainTool.h"
#include "Terrain.h"
#include "GameInstance.h"

//ImGuiFileDialog g_ImGuiFileDialog;
//ImGuiFileDialog::Instance() 이래 싱글톤으로 쓰라고 신이 말하고 감


CTerrainTool::CTerrainTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImGuiTool{ pDevice, pContext }
{

}

CTerrainTool::CTerrainTool(const CTerrainTool& Prototype)
	: CImGuiTool(Prototype)
{
}

HRESULT CTerrainTool::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrainTool::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTerrainTool::Priority_Update(_float fTimeDelta)
{

}

EVENT CTerrainTool::Update(_float fTimeDelta)
{
	Height_Update(fTimeDelta);
	return EVN_NONE;
}

void CTerrainTool::Late_Update(_float fTimeDelta)
{
}

HRESULT CTerrainTool::Render()
{
	if (m_pWindowData->ShowTerrainMenu)
	{
		if (FAILED(Render_TerrainTool()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CTerrainTool::Render_TerrainTool()
{
	ImGui::SetNextWindowSize(ImVec2(600, 400));
	ImGui::Begin("Terrain Tools", &m_pWindowData->ShowTerrainMenu, NULL);

	ImGui::SeparatorText("Terrain Create");


	SetNextItemWidth(130);
	InputInt("Vertices X", &m_iVerticesX);
	SetNextItemWidth(130);
	InputInt("Vertices Z", &m_iVerticesZ);

	if (ImGui::Button("Create Terrain (Default)"))
	{
		if (m_iVerticesX == 0 || m_iVerticesZ == 0)
		{
			MSG_BOX("Vertices X or Z is 0");
		}
		else
		{
			m_pGameInstance->Clear_Layer(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Terrain"));
			/* 프로토타입 */
			if (FAILED(m_pGameInstance->Replace_Prototype(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_Component_VIBuffer_Terrain_Toolver"),
				CVIBuffer_Terrain::Create(m_pDevice, m_pContext, m_iVerticesX, m_iVerticesZ))))
			{
				ImGui::End();
				return E_FAIL;
			}
			CTerrain::DESC desc{};
			desc.iLevelID = ENUM_CLASS(LEVEL::MAPTOOL);
			desc.isToolver = true;
			/* 생성 */
			if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Terrain"),
				ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Terrain"), &desc)))
			{
				ImGui::End();
				return E_FAIL;
			}
		}
	}

	/************************Terrain Height Edit************************/
	SeparatorText("Terrain Height Edit");
	Terrain_HeightEditor();

	/************************Terrain Save************************/
	SeparatorText("Save Terrain");
	if (Button("Save Terrain"))
		m_bSave = true;
	if (Button("Load Terrain"))
		m_bLoad = true;
	if (m_bSave)
		Terrain_Save();
	if (m_bLoad)
		Terrain_Load();

	End();
	return S_OK;
}

void CTerrainTool::Terrain_HeightEditor()
{
	Checkbox("Enable_Picking", &m_bEnablePicking);
	Text("Mouse Pos on Terrain");
	Text("(%.2f, %.2f, %.2f)", m_fPickedPos.x, m_fPickedPos.y, m_fPickedPos.z);
	Separator();
	Text("Brush Setting");
	SliderFloat("Radius", &m_fBrushRadius, 1.0f, 50.0f);
	SliderFloat("Power", &m_fBrushPower, -20.0f, 20.0f);
	Checkbox("Enable Brush", &m_bBrushEnable);

	if (m_bBrushEnable && m_pGameInstance->Get_DIKeyState(DIK_LCONTROL) & 0x80)
	{
		_long		MouseWheel = {};
		if (MouseWheel = m_pGameInstance->Get_DIMouseMove(DIMM::WHEEL))
		{
			if (MouseWheel > 0)
			{
				m_fBrushRadius += MouseWheel * 0.01f;
			}
			else
			{
				m_fBrushRadius += MouseWheel * 0.01f;
			}
		}
		m_fBrushRadius = CLAMP(m_fBrushRadius, 1.0f, 50.0f);
	}
}

void CTerrainTool::Terrain_Save()
{
	path savePath = R"(..\Bin\Resources\Map\)";

	IGFD::FileDialogConfig config;
	config.path = R"(..\Bin\Resources\Map\)";
	config.flags = ImGuiFileDialogFlags_ConfirmOverwrite;

	IFILEDIALOG->OpenDialog("SaveMapDialog", "Choose directory to save", ".ter", config);

	if (IFILEDIALOG->Display("SaveMapDialog"))
	{
		if (IFILEDIALOG->IsOk())
		{
			// Terrain 찾기
			auto pTerrain = dynamic_cast<CTerrain*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Terrain"), 0));
			if (nullptr == pTerrain)
				return;

			// 버텍스와 인덱스 정보 가져오기
			auto pVIBuffer = dynamic_cast<CVIBuffer_Terrain*>(pTerrain->Get_Component(TEXT("Com_VIBuffer")));
			if (nullptr == pVIBuffer)
				return;

			path savePath = IFILEDIALOG->GetFilePathName();

			// 확장자가 없으면 .map 붙이기
			if (savePath.extension().string() != ".ter")
				savePath += ".ter";


			pVIBuffer->Save_Terrain(_wstring(savePath)); // 직접 작성한 저장 함수
		}
		m_bSave = false;
		IFILEDIALOG->Close();
	}

	return ;
}

void CTerrainTool::Terrain_Load()
{
	path savePath = R"(..\Bin\Resources\Map\)";

	IGFD::FileDialogConfig config;
	config.path = R"(..\Bin\Resources\Map\)";

	IFILEDIALOG->OpenDialog("SaveMapDialog", "Choose File to Load", ".ter", config);

	if (IFILEDIALOG->Display("SaveMapDialog"))
	{
		if (IFILEDIALOG->IsOk())
		{
			path savePath = IFILEDIALOG->GetFilePathName();

			// 확장자가 없으면 .map 붙이기
			if (savePath.extension().string() != ".ter")
				savePath += ".ter";

			/* 프로토타입 */
			if (FAILED(m_pGameInstance->Replace_Prototype(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_Component_VIBuffer_Terrain_Toolver"),
				CVIBuffer_Terrain::Create(m_pDevice, m_pContext, savePath, true))))
			{
				ImGui::End();
				return ;
			}
			m_pGameInstance->Clear_Layer(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Terrain"));
			CTerrain::DESC desc{};
			desc.iLevelID = ENUM_CLASS(LEVEL::MAPTOOL);
			desc.isToolver = true;
			/* 생성 */
			if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Terrain"),
				ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Terrain"), &desc)))
			{
				ImGui::End();
				return ;
			}
		}
		m_bLoad = false;
		IFILEDIALOG->Close();
	}

	return;
}


void CTerrainTool::Height_Update(_float fTimeDelta)
{
	if (m_bEnablePicking)
	{
		// 마우스 좌클릭 상태 확인
		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureMouse)
		{
			// Terrain 찾기
			auto pTerrain = dynamic_cast<CTerrain*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Terrain"), 0));
			if (nullptr == pTerrain)
				return;

			// 버텍스와 인덱스 정보 가져오기
			auto pVIBuffer = dynamic_cast<CVIBuffer_Terrain*>(pTerrain->Get_Component(TEXT("Com_VIBuffer")));
			if (nullptr == pVIBuffer)
				return;

			const _float3* pVertices = pVIBuffer->Get_VertexPositions();
			const _uint* pIndices = pVIBuffer->Get_Indices();
			_uint iNumIndices = pVIBuffer->Get_NumIndices();

			// 월드 행렬 가져오기
			_matrix WorldMatrix = pTerrain->Get_Transform()->Get_WorldMatrix();

			// 피킹된 위치 저장
			if (m_pGameInstance->Pick_Mesh(WorldMatrix, pVertices, pIndices, iNumIndices, m_fPickedPos))
			{
				// 브러시 적용
				if (m_pGameInstance->Get_DIMouseState(DIM::LBUTTON) & 0x80 && m_bBrushEnable)
				{
					pVIBuffer->Apply_Brush(m_fPickedPos, m_fBrushRadius, m_fBrushPower, fTimeDelta);
				}
			}
		}
	}
	
}

CTerrainTool* CTerrainTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CTerrainTool* pInstance = new CTerrainTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CTerrainTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrainTool::Clone(void* pArg)
{
	CTerrainTool* pInstance = new CTerrainTool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrainTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CTerrainTool::Free()
{
	__super::Free();

}
