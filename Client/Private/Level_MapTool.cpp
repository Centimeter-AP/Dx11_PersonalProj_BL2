#include "Level_MapTool.h"
#include "GameInstance.h"

#include "Level_Loading.h"
//#include "Camera_Free.h"

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

    if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
        return E_FAIL;


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

    return S_OK;
}

HRESULT CLevel_MapTool::Ready_Lights()
{
    //LIGHT_DESC          LightDesc{};

    //LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
    //LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
    //LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
    //LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
    //LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

    //if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
    //    return E_FAIL;

    //LightDesc.eType = LIGHT_DESC::TYPE_POINT;
    //// LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
    //LightDesc.vPosition = _float4(20.f, 5.f, 20.f, 1.f);
    //LightDesc.fRange = 20.f;
    //LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
    //LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
    //LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

    //if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
    //    return E_FAIL;

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
    __super::Free();

}
