#include "Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UIObject.h"
#include "BackGround.h"
#include "Layer.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_StaticUI(TEXT("Layer_Static_UI"))))
		return E_FAIL;


	return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_F1))
	{
		if (FAILED(m_pGameInstance->Change_Level(static_cast<_uint>(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::GAMEPLAY))))
			return;
	}
	else if (KEY_DOWN(DIK_F2))
	{
		if (FAILED(m_pGameInstance->Change_Level(static_cast<_uint>(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::MAPTOOL))))
			return;
	}
	else if (KEY_DOWN(DIK_F3))
	{
		if (FAILED(m_pGameInstance->Change_Level(static_cast<_uint>(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::BOSS))))
			return;
	}
}

HRESULT CLevel_Logo::Render()
{
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _wstring strLayerTag)
{
	CBackGround::BACKGROUND_DESC				BackGroundDesc{};

	BackGroundDesc.fX = g_iWinSizeX * 0.5f;
	BackGroundDesc.fY = g_iWinSizeY * 0.5f;
	BackGroundDesc.fSizeX = 200.0f;
	BackGroundDesc.fSizeY = 200.0f;	

	//BackGroundDesc.fSizeX = g_iWinSizeX;
	//BackGroundDesc.fSizeY = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Add_GameObject(static_cast<_uint>(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
		static_cast<_uint>(LEVEL::LOGO), strLayerTag, &BackGroundDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_StaticUI(const _wstring strLayerTag)
{
	CUIObject::UIOBJECT_DESC			UIDesc;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	UIDesc.fSizeX = 50.0f;
	UIDesc.fSizeY = 50.0f;
	if (FAILED(m_pGameInstance->Add_GameObject(static_cast<_uint>(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_Aim"),
		static_cast<_uint>(LEVEL::STATIC), strLayerTag, &UIDesc)))
		return E_FAIL;


	UIDesc.fX = 180.f;
	UIDesc.fY = 650.f;
	UIDesc.fSizeX = 214.f;
	UIDesc.fSizeY = 36.0f;
	if (FAILED(m_pGameInstance->Add_GameObject(static_cast<_uint>(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_HP"),
		static_cast<_uint>(LEVEL::STATIC), strLayerTag, &UIDesc)))
		return E_FAIL;
	

	UIDesc.fX = g_iWinSizeX - 180.f;
	UIDesc.fY = 650.f;
	UIDesc.fSizeX = 214.f;
	UIDesc.fSizeY = 36.0f;
	if (FAILED(m_pGameInstance->Add_GameObject(static_cast<_uint>(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_Ammo"),
		static_cast<_uint>(LEVEL::STATIC), strLayerTag, &UIDesc)))
		return E_FAIL;
	

	UIDesc.fX = 160.f;
	UIDesc.fY = 620.f;
	UIDesc.fSizeX = 168.f;
	UIDesc.fSizeY = 31.0f;
	if (FAILED(m_pGameInstance->Add_GameObject(static_cast<_uint>(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_Shield"),
		static_cast<_uint>(LEVEL::STATIC), strLayerTag, &UIDesc)))
		return E_FAIL;
	

	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = 560.f;
	UIDesc.fSizeX = 439.f;
	UIDesc.fSizeY = 125.f;
	if (FAILED(m_pGameInstance->Add_GameObject(static_cast<_uint>(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_Phaselock"),
		static_cast<_uint>(LEVEL::STATIC), strLayerTag, &UIDesc)))
		return E_FAIL;


	UIDesc.fX = g_iWinSizeX * 0.5f;// 640 640
	UIDesc.fY = 640.f;
	UIDesc.fSizeX = 400.f;
	UIDesc.fSizeY = 29.f;
	if (FAILED(m_pGameInstance->Add_GameObject(static_cast<_uint>(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_Exp"),
		static_cast<_uint>(LEVEL::STATIC), strLayerTag, &UIDesc)))
		return E_FAIL;


	//m_pGameInstance->Active_Layer(static_cast<_uint>(LEVEL::STATIC), strLayerTag, false);

	return S_OK;
}


CLevel_Logo* CLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Logo* pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Logo::Free()
{
	__super::Free();

}
