#include "Level_MapTool.h"
#include "GameInstance.h"

#include "Level_Loading.h"
//#include "Camera_Free.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "ImGuizmo.h"

CLevel_MapTool::CLevel_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_MapTool::Initialize()
{
    //if (FAILED(Ready_Lights()))
        //return E_FAIL;

    //if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
    //    return E_FAIL;

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
    //ImGui::ShowDemoWindow(); // Show demo window! :)

    ImGui_Render();
    //렌더링 어쩌구저쩌구들

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
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX11_Init(m_pDevice, m_pContext);

    return S_OK;
}

HRESULT CLevel_MapTool::ImGui_Render()
{
    ImGui::DockSpaceOverViewport();

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
    ImGui::Begin("Hierachy");

    ImGui::End();
    return S_OK;
}

HRESULT CLevel_MapTool::Window_ObjectInspector()
{
    ImGui::Begin("Inspector");

    ImGui::End();
    return S_OK;
}

HRESULT CLevel_MapTool::Window_ResourceList()
{
    ImGui::Begin("Resources");

    ImGui::End();
    return S_OK;
}

HRESULT CLevel_MapTool::Ready_Lights()
{
    return S_OK;
}

HRESULT CLevel_MapTool::Ready_Layer_Camera(const _wstring& strLayerTag)
{
    //CCamera_Free::CAMERA_FREE_DESC            Desc{};

    //Desc.pGameObjectTag = TEXT("GameObject_Camera_Free");
    //Desc.vEye = _float3(0.f, 20.f, -15.f);
    //Desc.vAt = _float3(0.f, 0.f, 0.f);
    //Desc.fFov = XMConvertToRadians(60.f);
    //Desc.fNear = 0.01f;
    //Desc.fFar = 500.f;
    //Desc.fMouseSensor = 0.07f;
    //Desc.fSpeedPerSec = 10.f;
    //Desc.fRotationPerSec = XMConvertToRadians(180.f);

    //if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"),
    //    LEVEL_GAMEPLAY, strLayerTag, &Desc)))
    //    return E_FAIL;


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
    if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
        ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
        return E_FAIL;

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

    __super::Free();

}
