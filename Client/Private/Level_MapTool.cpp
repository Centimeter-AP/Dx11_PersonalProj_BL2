#include "Level_MapTool.h"
#include "Level_Loading.h"
#include "GameInstance.h"

#include "Terrain.h"

#include "TerrainTool.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "ImGuizmo.h"

#pragma push_macro("new")
#undef new
#include "imgui.h"
#include "imgui_internal.h"
#pragma pop_macro("new")

CLevel_MapTool::CLevel_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_MapTool::Initialize()
{
    //if (FAILED(Ready_Lights()))
        //return E_FAIL;

    if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
        return E_FAIL;



    if (FAILED(Ready_ImGui()))
        return E_FAIL;

    //if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
    //    return E_FAIL;


    return S_OK;
}

void CLevel_MapTool::Update(_float fTimeDelta)
{

}

HRESULT CLevel_MapTool::Render()
{
#ifdef _DEBUG
    SetWindowText(g_hWnd, TEXT("맵툴 레벨입니다."));
#endif
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui_Render();
    //렌더링 어쩌구저쩌구들
    ImGui::ShowDemoWindow(); // Show demo window! :)

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    //ImGui::UpdatePlatformWindows();
    //ImGui::RenderPlatformWindowsDefault();
    return S_OK;
}

HRESULT CLevel_MapTool::Ready_ImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // multi-viewport?
     
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Consola.ttf", 14.0f);

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX11_Init(m_pDevice, m_pContext);

    return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_Camera(const _wstring strLayerTag)
{
    if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Camera_Tool"),
        ENUM_CLASS(LEVEL::MAPTOOL), strLayerTag)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_MapTool::ImGui_Render()
{
    //ImGui::DockSpaceOverViewport();  똥

    /* 도킹 세팅 + 툴바 세팅 */
    if (FAILED(ImGui_Docking_Settings()))
        return E_FAIL;
    /* 툴 바 등에서 호출한 개별 창들 생성 */
    if (FAILED(Show_ExternWindows()))
        return E_FAIL;

    /* 기본 윈도우들 */
    if (FAILED(Window_ObjectList()))
        return E_FAIL;
    if (FAILED(Window_ObjectInspector()))
        return E_FAIL;
    if (FAILED(Window_ResourceList()))
        return E_FAIL;


    return S_OK;
}

HRESULT CLevel_MapTool::Window_ObjectList()
{
    ImGui::Begin("Object Lists");



    ImGui::End();
    return S_OK;
}

HRESULT CLevel_MapTool::Window_ObjectInspector()
{
    ImGui::Begin("Inspector");

    Text("Mouse Pos X : %.2f", 0.f);
    Text("Mouse Pos Y : %.2f", 0.f);
    Separator();
    Separator();
    Text("Mouse Pos on Terrain");
    Text("(%.2f, %.2f, %.2f)", 0.f, 0.f, 0.f);
    



    ImGui::End();
    return S_OK;
}

HRESULT CLevel_MapTool::Window_ResourceList()
{
    ImGui::Begin("Resources");

    ImGui::End();
    return S_OK;
}

HRESULT CLevel_MapTool::ImGui_Docking_Settings()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_MenuBar;


    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    Toolbar_Menus();

    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::End();

    return S_OK;
}

HRESULT CLevel_MapTool::Toolbar_Menus()
{
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open", "Ctrl+O")) { /* 파일 열기 로직 */ }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { /* 파일 저장 로직 */ }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* 실행 취소 로직 */ }
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* 다시 실행 로직 */ }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tool")) {
            if (ImGui::MenuItem("Anim Model")) {   }
            if (ImGui::MenuItem("NonAnim Model")) {}
            ImGui::MenuItem("Terrain", NULL, &m_tWindowData.ShowTerrainMenu);
            if (ImGui::BeginMenu("Components")) {
                ImGui::MenuItem("Debug Log", NULL, false, true);
                if (ImGui::MenuItem("Collider")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    return S_OK;
}

HRESULT CLevel_MapTool::Terrain_Tools(_bool* p_open)
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
}

HRESULT CLevel_MapTool::Show_ExternWindows()
{
    if (m_tWindowData.ShowTerrainMenu) { Terrain_Tools(&m_tWindowData.ShowTerrainMenu); }
    return S_OK;
}

HRESULT CLevel_MapTool::Ready_Lights()
{
    return S_OK;
}

HRESULT CLevel_MapTool::Ready_ImGuiTools()
{
	for (_uint i = 0; i < ENUM_CLASS(IMGUITOOL::END); ++i)
	{
        switch (static_cast<IMGUITOOL>(i))
        {
        case IMGUITOOL::TERRAIN:
            m_ImGuiTools[i] = CTerrainTool::Create(m_pDevice, m_pContext);
            if (nullptr == m_ImGuiTools[i])
                return E_FAIL;
            break;
        case IMGUITOOL::OBJECT:
            break;
        case IMGUITOOL::CAMERA:
            break;
        case IMGUITOOL::END:
            break;
        }
		
	}
    return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_Player(const _wstring& strLayerTag)
{

    return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_Monster(const _wstring& strLayerTag)
{
    return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
    //if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Terrain"),
    //    ENUM_CLASS(LEVEL::MAPTOOL), strLayerTag)))
    //    return E_FAIL;

    //if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"),
    //    LEVEL_GAMEPLAY, strLayerTag)))
    //    return E_FAIL;


    return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_Effect(const _wstring& strLayerTag)
{


    return S_OK;
}

CLevel_MapTool* CLevel_MapTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLevel_MapTool* pGameInstance = new CLevel_MapTool(pDevice, pContext);

    if (FAILED(pGameInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CLevel_MapTool");
        Safe_Release(pGameInstance);
    }

    return pGameInstance;
}


void CLevel_MapTool::Free()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    //ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable; // 멀티 뷰포트 비활성화
    //ImGui::GetIO().Fonts->Clear(); // 폰트 캐시 정리
    ImGui::DestroyContext();

	for (_uint i = 0; i < ENUM_CLASS(IMGUITOOL::END); ++i)
	{
		Safe_Release(m_ImGuiTools[i]);
	}

    __super::Free();

}
