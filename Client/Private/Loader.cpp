#include "Loader.h"

#include "GameInstance.h"

#include "Camera_Free.h"
#include "Camera_Tool.h"
#include "Camera_FPS.h"

#include "BackGround.h"

#include "Terrain.h"
#include "MapObject.h"

#include "Player.h"
#include "Pistol.h"
#include "AssaultRifle.h"

#include "Rakk.h"
#include "Skag.h"
#include "Warrior.h"
#include "SpiderAnt.h"
#include "SpiderAntSpit.h"
#include "Leviathan.h"
#include "Levi_HitMesh.h"
#include "Levi_Bullet.h"

/** UI **/ 
#include "UI_Aim.h"
#include "UI_Ammo.h"
#include "UI_Ammo_Bar.h"
#include "UI_HP.h"
#include "UI_HP_Bar.h"
#include "UI_Shield.h"
#include "UI_Shield_Bar.h"
#include "UI_Phaselock.h"
#include "UI_HPShieldPannel.h"
#include "UI_AmmoPannel.h"
#include "UI_Exp.h"
#include "UI_EnemyHP.h"

#include "TestSpirteEffect.h"
#include "Screen_Snow.h"
#include "Snow.h"
#include "MuzzleFlash.h"
#include "Sky.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

_uint APIENTRY LoadingMain(void* pArg)
{
	/* 자원로딩한다. */
	CLoader*		pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;	

	return 0;

}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	if (FAILED(CoInitializeEx(nullptr, 0)))
		return E_FAIL;

	EnterCriticalSection(&m_CriticalSection);

	HRESULT		hr = {};

	switch (m_eNextLevelID)
	{

	case LEVEL::LOGO:
		hr = Loading_For_Logo();
		break;

	case LEVEL::GAMEPLAY:
		hr = Loading_For_GamePlay();
		break;
		
	case LEVEL::MAPTOOL:
		hr = Loading_For_Maptool();
		break;

	case LEVEL::BOSS:
		hr = Loading_For_Boss();
		break;
	}

	LeaveCriticalSection(&m_CriticalSection);
	if (FAILED(hr))
		return E_FAIL;
	else
		return S_OK;
}

HRESULT CLoader::Loading_For_Loading()
{

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo()
{
	
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/* For.Prototype_Component_Texture_BackGround*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Logo/Logo.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_Logo_BackGround*/
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_Logo_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Logo/Logo_Background.dds"), 1))))
		return E_FAIL;

	/*************************** Loading for Static Objects *****************************/

#pragma region UI_TEXTURE

	/* For.Prototype_Component_Texture_UI_Aim */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Aim"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Tex_Dahl_Sight_Pistol.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Icon_HP */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Icon_HP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_Icon_HP.png"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_UI_HPBar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_HP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_Bar_HP.png"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_UI_Bar_Left */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Left"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_Bar_Left.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Bar_Ammo */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Ammo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_Bar_Ammo.png"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_UI_Bar_Right */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Right"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_Bar_Right.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Icon_Shield */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Icon_Shield"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_Icon_Shield.png"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_UI_Bar_Shield */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Shield"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_Bar_Shield.png"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_UI_Bar_ShieldBack */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_ShieldBack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_Bar_ShieldBack.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Bar_Phaselock_Back */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Phaselock_Back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_Bar_Phaselock_Back.png"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_UI_Bar_Phaselock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Phaselock"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_Bar_Phaselock.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Bar_Exp_Back */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Exp_Back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_Bar_Exp_Back.png"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_UI_Bar_Exp */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_Exp"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_Bar_Exp.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Bar_EnemyHP_Back */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_EnemyHP_Back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_EnemyHP.png"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_UI_Bar_EnemyHP */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_EnemyHP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_EnemyHP_Bar.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Bar_BossHP_Back */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_BossHP_Back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_Bar_Exp_Back.png"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_UI_Bar_BossHP */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_BossHP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/UI_Bar_Exp.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_MuzzleFlash */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_MuzzleFlash"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Tex_Assault_Muzzle_Flash_Front.dds")))))
		return E_FAIL;


#pragma endregion



	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("셰이더을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));
	

	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma region STATIC_UI

	/* For.Prototype_GameObject_UI_Aim */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_Aim"),
		CUI_Aim::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_HPShieldPannel */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_HPShieldPannel"),
		CUI_HPShieldPannel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_AmmoPannel */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_AmmoPannel"),
		CUI_AmmoPannel::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_UI_HP */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_HP"),
		CUI_HP::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_UI_HP_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_HP_Bar"),
		CUI_HP_Bar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_UI_Ammo */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_Ammo"),
		CUI_Ammo::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_UI_Ammo_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_Ammo_Bar"),
		CUI_Ammo_Bar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_UI_Shield */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_Shield"),
		CUI_Shield::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_UI_Shield */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_Shield_Bar"),
		CUI_Shield_Bar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_UI_Phaselock */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_Phaselock"),
		CUI_Phaselock::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_UI_Exp */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_Exp"),
		CUI_Exp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_EnemyHP */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI_EnemyHP"),
		CUI_EnemyHP::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_MuzzleFlash */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_MuzzleFlash"),
		CMuzzleFlash::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion


	
	
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Frost/DirtySnow_Dif.dds"), 1))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxRectInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxRectInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxRectInstance.hlsl"), VTXRECT_PARTICLE_INSTANCE::Elements, VTXRECT_PARTICLE_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxPosInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosInstance.hlsl"), VTXPOS_PARTICLE_INSTANCE::Elements, VTXPOS_PARTICLE_INSTANCE::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_Splash */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Splash"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Skag_Spit_Ball_Dif.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Snow */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Snow/Snow.dds")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_ScreenSnow */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ScreenSnow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Snow_FlakesScreen_Dif.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ScreenSnowMask */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ScreenSnowMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Snow_Dissipate_Mask.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_AR_Nor */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_AR_Nor"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Bin_Anim/Weap_AssaultRifle_Nrm.dds")))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_PST_Nor */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_PST_Nor"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Bin_Anim/Weap_Pistols_Nrm.dds")))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/backup/1/GamePlay.ter"), true))))
		//CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/backup/2/GamePlay.ter"), true))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Snow */
	CVIBuffer_Point_Instance::POINT_INSTANCE_DESC		SnowDesc{};
	SnowDesc.iNumInstance = 1000;
	SnowDesc.vCenter = _float3(100.f, 50.f, 100.0f);
	SnowDesc.vRange = _float3(200.f, 3.0f, 200.f);
	SnowDesc.vSize = _float2(0.1f, 0.4f);
	SnowDesc.vLifeTime = _float2(2.f, 5.f);
	SnowDesc.vSpeed = _float2(10.f, 20.f);
	SnowDesc.isLoop = true;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Snow"),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &SnowDesc))))
		return E_FAIL;



	///* For.Prototype_Component_VIBuffer_Cube */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
	//	CVIBuffer_Cube::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	// * XMMatrixTranslation(0.f, -0.5f, 0.f)
	_matrix PreTransMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(-90.f)) * XMMatrixTranslation(0.f, -1.5f, 0.f);

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Siren_Hand"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM,
			"../Bin/Resources/Models/Bin_Anim/Player_Hand.bin", PreTransMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Sky"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM,
			"../Bin/Resources/Models/Bin_NonAnim/Sky/Sky.bin", PreTransMatrix))))
		return E_FAIL;

	PreTransMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Rakk"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM,
			"../Bin/Resources/Models/Bin_Anim/Rakk/Rakk.bin", PreTransMatrix))))
		return E_FAIL;

	PreTransMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Skag"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM,
			"../Bin/Resources/Models/Bin_Anim/Skag/Skag.bin", PreTransMatrix))))
		return E_FAIL;

	PreTransMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_SpiderAnt"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM,
			"../Bin/Resources/Models/Bin_Anim/SpiderAnt/SpiderAnt.bin", PreTransMatrix))))
		return E_FAIL;

	PreTransMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(-90.f)) * XMMatrixRotationY(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_AR"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM,
			"../Bin/Resources/Models/Bin_Anim/AssaultRifle.bin", PreTransMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Pistol"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM,
			"../Bin/Resources/Models/Bin_Anim/Pistol.bin", PreTransMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("네비게이션을(를) 로딩중입니다."));

	/* Prototype_Component_Navigation */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/Navigation_1.dat")))))
		//CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/Navigation_2.dat")))))
		return E_FAIL;

	///* Prototype_Component_Gravity */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Gravity"),
	//	CGravity::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_FPS */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Camera_FPS"),
		CCamera_FPS::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_MapObject */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_MapObject"),
		CMapObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_AssaultRifle */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_AssaultRifle"),
		CAssaultRifle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Pistol */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Pistol"),
		CPistol::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rakk */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Rakk"),
		CRakk::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rakk */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Skag"),
		CSkag::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_SpiderAnt */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SpiderAnt"),
		CSpiderAnt::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_SpiderAnt */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SpiderAntSpit"),
		CSpiderAntSpit::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Effect"),
		CTestSpirteEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Snow */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Snow"),
		CSnow::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Snow */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ScreenSnow"),
		CScreen_Snow::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Maptool()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Frost/DirtySnow_Dif.dds"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));

	_matrix PreTransMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Siren_Hand"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM,
			"../Bin/Resources/Models/Bin_Anim/Player_Hand.bin", PreTransMatrix))))
		return E_FAIL;

	PreTransMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(-90.f)) * XMMatrixRotationY(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_AR"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM,
			"../Bin/Resources/Models/Bin_Anim/AssaultRifle.bin", PreTransMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Pistol"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM,
			"../Bin/Resources/Models/Bin_Anim/Pistol.bin", PreTransMatrix))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Tool */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Camera_Tool"),
		CCamera_Tool::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_Camera_FPS */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Camera_FPS"),
		CCamera_FPS::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_MapObject */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_MapObject"),
		CMapObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_AssaultRifle */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_AssaultRifle"),
		CAssaultRifle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Pistol */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Pistol"),
		CPistol::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_For_Boss()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/SlateGravel_Dif.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_AR_Nor */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_AR_Nor"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Bin_Anim/Weap_AssaultRifle_Nrm.dds")))))
		return E_FAIL;
	/* For.Prototype_Component_Texture_PST_Nor */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_PST_Nor"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Bin_Anim/Weap_Pistols_Nrm.dds")))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Map/backup/2/GamePlay.ter"), true))))
		return E_FAIL;

	// * XMMatrixTranslation(0.f, -0.5f, 0.f)
	_matrix PreTransMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(-90.f)) * XMMatrixTranslation(0.f, -1.5f, 0.f);

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Siren_Hand"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM,
			"../Bin/Resources/Models/Bin_Anim/Player_Hand.bin", PreTransMatrix))))
		return E_FAIL;

	PreTransMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(-90.f)) * XMMatrixRotationY(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_AR"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM,
			"../Bin/Resources/Models/Bin_Anim/AssaultRifle.bin", PreTransMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Pistol"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM,
			"../Bin/Resources/Models/Bin_Anim/Pistol.bin", PreTransMatrix))))
		return E_FAIL;

	PreTransMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Sky"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM,
			"../Bin/Resources/Models/Bin_NonAnim/Sky/Sky.bin", PreTransMatrix))))
		return E_FAIL;

	PreTransMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_Component_Model_Rakk"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM,
			"../Bin/Resources/Models/Bin_Anim/Rakk/Rakk.bin", PreTransMatrix))))
		return E_FAIL;

	PreTransMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_Component_Model_Skag"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM,
			"../Bin/Resources/Models/Bin_Anim/Skag/Skag.bin", PreTransMatrix))))
		return E_FAIL;

	PreTransMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_Component_Model_SpiderAnt"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM,
			"../Bin/Resources/Models/Bin_Anim/SpiderAnt/SpiderAnt.bin", PreTransMatrix))))
		return E_FAIL;

	PreTransMatrix = /*XMMatrixScaling(0.1f, 0.1f, 0.1f) * */XMMatrixRotationY(XMConvertToRadians(-90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_Component_Model_Leviathan"),
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM,
			"../Bin/Resources/Models/Bin_Anim/Boss_Leviathan/Leviathan.bin", PreTransMatrix))))
		return E_FAIL;

	PreTransMatrix =/* XMMatrixScaling(0.1f, 0.1f, 0.1f) **/ XMMatrixRotationY(XMConvertToRadians(-90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_Component_Model_Levi_HitMesh"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM,
			"../Bin/Resources/Models/Bin_NonAnim/Leviathan_HitMesh.bin", PreTransMatrix))))
		return E_FAIL;

	PreTransMatrix =/* XMMatrixScaling(0.1f, 0.1f, 0.1f) **/ XMMatrixRotationY(XMConvertToRadians(-90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_Component_Model_Levi_Boulder"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM,
			"../Bin/Resources/Models/Bin_NonAnim/BarrensRockBoulder.bin", PreTransMatrix))))
		return E_FAIL;

	PreTransMatrix =/* XMMatrixScaling(0.1f, 0.1f, 0.1f) **/ XMMatrixRotationY(XMConvertToRadians(-90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_Component_Model_Levi_Boulder_Small"),
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM,
			"../Bin/Resources/Models/Bin_NonAnim/BarrensRockBoulder_02.bin", PreTransMatrix))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("네비게이션을(를) 로딩중입니다."));

	/* Prototype_Component_Navigation */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/Navigation_2.dat")))))
		return E_FAIL;

	///* Prototype_Component_Gravity */
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Gravity"),
	//	CGravity::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_FPS */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_Camera_FPS"),
		CCamera_FPS::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_MapObject */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_MapObject"),
		CMapObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_AssaultRifle */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_AssaultRifle"),
		CAssaultRifle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Pistol */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Pistol"),
		CPistol::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rakk */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_Rakk"),
		CRakk::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Rakk */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_Skag"),
		CSkag::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_SpiderAnt */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_SpiderAnt"),
		CSpiderAnt::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Leviathan */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_Leviathan"),
		CLeviathan::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_Levi_HitMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_Levi_HitMesh"),
		CLevi_HitMesh::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Levi_Bullet */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_Levi_Bullet"),
		CLevi_Bullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_SpiderAntSpit */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_SpiderAntSpit"),
		CSpiderAntSpit::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

CLoader* CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	Safe_Release(m_pGameInstance);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	CoUninitialize();

	DeleteCriticalSection(&m_CriticalSection);
}
