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
	BackGroundDesc.fSizeX = g_iWinSizeX;
	BackGroundDesc.fSizeY = g_iWinSizeY; 

	if (FAILED(m_pGameInstance->Add_GameObject(static_cast<_uint>(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
		static_cast<_uint>(LEVEL::LOGO), strLayerTag, &BackGroundDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_StaticUI(const _wstring strLayerTag)
{

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
