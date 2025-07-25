#include "Level_Loading.h"


#include "Level_GamePlay.h"
#include "Level_Logo.h"
#include "Level_MapTool.h"
#include "Level_Boss.h"

#include "Loader.h"

#include "GameInstance.h"
#include <BackGround.h>
#include "LoadingUI.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	/* 로딩레벨 자체에 필요한 객체를 생성한다. */
	/* 배경, 로딩바, 버튼, font */

	/* 로딩의 역할(다음레벨에 필요한 자원(Resource)(텍스쳐, 모델, 사운드 등등등 )을 생성하는)을 
	수행할 로더객체를 생성한다. */
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;

	if (m_eNextLevelID != LEVEL::LOGO)
	{
		CLoadingUI::DESC				LoadingDesc{};

		LoadingDesc.fX = g_iWinSizeX * 0.5f;
		LoadingDesc.fY = g_iWinSizeY * 0.5f;
		LoadingDesc.fSizeX = g_iWinSizeX;
		LoadingDesc.fSizeY = g_iWinSizeY;
		if (m_eNextLevelID == LEVEL::GAMEPLAY)
			LoadingDesc.iLevelTextureIdx = 0;
		else if (m_eNextLevelID == LEVEL::BOSS)
		{
			LoadingDesc.iLevelTextureIdx = 1;
			GET_PLAYER->Set_Active(false);
		}

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_LoadingUI"),
			ENUM_CLASS(LEVEL::LOADING), TEXT("Layer_LoadingUI"), &LoadingDesc)))
			return E_FAIL;
		
	}
	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (true == m_pLoader->isFinished())
		{
			CLevel* pLevel = { nullptr };
			m_pGameInstance->Clear_Collider();
			m_pGameInstance->Clear_Lights();
			switch (m_eNextLevelID)
			{
			case LEVEL::LOGO:
				pLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;
			case LEVEL::GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				break;
			case LEVEL::MAPTOOL:
				pLevel = CLevel_MapTool::Create(m_pDevice, m_pContext);
				break;
			case LEVEL::BOSS:
				pLevel = CLevel_Boss::Create(m_pDevice, m_pContext);
				break;
			}

			if (nullptr == pLevel)
				return;

			if (FAILED(m_pGameInstance->Change_Level(static_cast<_uint>(m_eNextLevelID), pLevel)))
				return;
		}
	}	
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->Output_LoadingText();

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const _wstring strLayerTag)
{


	return S_OK;
}


CLevel_Loading* CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
