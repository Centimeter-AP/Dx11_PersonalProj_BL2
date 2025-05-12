#include "TerrainTool.h"
#include "Terrain.h"
#include "GameInstance.h"

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

void CTerrainTool::Update(_float fTimeDelta)
{
	//if (m_bBrushEnable && m_pGameInstance->Get_DIMouseState(DIMK::LB) & 0x80)
	//{
	//	_float3 vPickedPos;
	//	if (m_pGameInstance->Get_PickingManager()->PickTerrain(vPickedPos)) // 피킹 성공 시
	//	{
	//		CTerrain* pTerrain = dynamic_cast<CTerrain*>(m_pGameInstance->Find_GameObject(TEXT("Layer_Terrain")));
	//		if (pTerrain)
	//		{
	//			pTerrain->ApplyBrush(vPickedPos, fTmpRadius, fTmpPower, fDeltaTime);
	//		}
	//	}
	//}

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
			if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_Component_VIBuffer_Terrain_Toolver"),
				CVIBuffer_Terrain::Create(m_pDevice, m_pContext, m_iVerticesX, m_iVerticesZ))))
				return E_FAIL;
			CTerrain::DESC desc{};
			desc.eLevelID = LEVEL::MAPTOOL;
			desc.isToolver = true;
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

	static _float fTmpRadius = 0.0f;
	static _float fTmpPower = 0.0f;
	SliderFloat(" ", &fTmpRadius, 1.0f, 50.0f);
	Text("Brush Radius");
	SetNextItemWidth(200);
	SliderFloat("Brush Power", &fTmpPower, -10.0f, 10.0f);
	Checkbox("Enable Brush", &m_bBrushEnable);

	if (m_bBrushEnable && m_pGameInstance->Get_DIKeyState(DIK_LCONTROL) & 0x80)
	{
		_long		MouseWheel = {};
		if (MouseWheel = m_pGameInstance->Get_DIMouseMove(DIMM::WHEEL))
		{
			if (MouseWheel > 0)
			{
				fTmpRadius += MouseWheel * 0.01f;
			}
			else
			{
				fTmpRadius += MouseWheel * 0.01f;
			}
		}
		fTmpRadius = CLAMP(fTmpRadius, 1.0f, 50.0f);
	}




	End();
	return S_OK;
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
