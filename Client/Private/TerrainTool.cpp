#include "TerrainTool.h"
#include <Terrain.h>

CTerrainTool::CTerrainTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImGuiTool{ pDevice, pContext }
{
	
}

CTerrainTool::CTerrainTool(const CTerrainTool& Prototype)
	: CImGuiTool( Prototype )
{

}


HRESULT CTerrainTool::Initialize(void* pArg)
{
	return S_OK;
}

void CTerrainTool::Priority_Update(_float fTimeDelta)
{

}

void CTerrainTool::Update(_float fTimeDelta)
{
}

void CTerrainTool::Late_Update(_float fTimeDelta)
{
}

HRESULT CTerrainTool::Render()
{
	return S_OK;
}

HRESULT CTerrainTool::Main_Tool()
{
    ImGui::SetNextWindowSize(ImVec2(600, 400));
    ImGui::Begin("Terrain Tools", p_open, ImGuiWindowFlags_MenuBar);

    if (ImGui::Button("Create Terrain (Default)"))
    {
        CTerrain::DESC desc{};
        desc.eLevelID = LEVEL::MAPTOOL;
        if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Terrain"),
            ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Terrain"), &desc)))
        {
            ImGui::End();
            return E_FAIL;
        }

    }
    Separator();
    static _int iVerticesX = 0.f;
    static _int iVerticesZ = 0.f;
    SetNextItemWidth(130);
    InputInt("Vertices X", &iVerticesX);
    SetNextItemWidth(130);
    InputInt("Vertices Z", &iVerticesZ);

    Separator();



    ImGui::End();
    return S_OK;

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

void CTerrainTool::Free()
{
	__super::Free();

}
