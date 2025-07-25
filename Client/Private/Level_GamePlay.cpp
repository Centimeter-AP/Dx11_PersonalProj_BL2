#include "Level_GamePlay.h"
#include "Level_Loading.h"
#include "GameInstance.h"
#include "UIObject.h"
#include "Terrain.h"
#include "Player.h"
#include "Rakk.h"
#include "Skag.h"
#include "SpiderAnt.h"
#include "Camera_FPS.h"
#include "Sky.h"
#include "SpriteEffect.h"


CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Sky(TEXT("Layer_Sky"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_MapObject(TEXT("Layer_MapObject"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Sound()))
		return E_FAIL;

	ShowCursor(FALSE);

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	Intersect();

	Key_Input();
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _wstring strLayerTag)
{
	CTerrain::DESC desc;
	desc.iLevelID = ENUM_CLASS(LEVEL::GAMEPLAY);
	desc.isToolver = false;
	desc.fRotationPerSec = 0.f;
	desc.fSpeedPerSec = 0.f;
	desc.bHasTransformPreset = false;
	desc.pParentMatrix = nullptr;
	desc.pParentObject = nullptr;
	desc.strVIBufferTag = L"";
	desc.szName = L"";
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Sky(const _wstring strLayerTag)
{
	CSky::DESC skydesc = {};
	skydesc.iLevelID = ENUM_CLASS(LEVEL::STATIC);
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Sky"),
		ENUM_CLASS(LEVEL::STATIC), strLayerTag, &skydesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Camera_Free"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Camera_FPS"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	CCamera* pPrevCamera = static_cast<CCamera*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::GAMEPLAY), L"Layer_Camera", CAM_FREE));
	pPrevCamera->Set_Using(false);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _wstring strLayerTag)
{
	srand(_uint(time(NULL)));
	CRakk::DESC RakkDesc;
	RakkDesc.bActive = true;
	RakkDesc.szName = L"Rakk";
	RakkDesc.fRotationPerSec = XMConvertToRadians(180.f);
	RakkDesc.fSpeedPerSec = 10.f;
	RakkDesc.iLevelID = ENUM_CLASS(LEVEL::GAMEPLAY);
	RakkDesc.strVIBufferTag = TEXT("Prototype_Component_Model_Rakk");
	RakkDesc.bHasTransformPreset = true;
	XMStoreFloat4x4(&RakkDesc.PresetMatrix, XMMatrixTranslation(rand()%200, rand()%3 + 20.f, rand()%200));
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Rakk"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &RakkDesc)))
		return E_FAIL;
	XMStoreFloat4x4(&RakkDesc.PresetMatrix, XMMatrixTranslation(rand() % 200, rand() % 3 + 20.f, rand() % 200));
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Rakk"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &RakkDesc)))
		return E_FAIL;
	XMStoreFloat4x4(&RakkDesc.PresetMatrix, XMMatrixTranslation(rand() % 200, rand() % 3 + 20.f, rand() % 200));
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Rakk"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &RakkDesc)))
		return E_FAIL;
	XMStoreFloat4x4(&RakkDesc.PresetMatrix, XMMatrixTranslation(rand() % 200, rand() % 3 + 20.f, rand() % 200));
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Rakk"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &RakkDesc)))
		return E_FAIL;
	XMStoreFloat4x4(&RakkDesc.PresetMatrix, XMMatrixTranslation(rand() % 200, rand() % 3 + 20.f, rand() % 200));
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Rakk"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &RakkDesc)))
		return E_FAIL;
	XMStoreFloat4x4(&RakkDesc.PresetMatrix, XMMatrixTranslation(rand() % 200, rand() % 3 + 20.f, rand() % 200));
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Rakk"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &RakkDesc)))
		return E_FAIL;
	XMStoreFloat4x4(&RakkDesc.PresetMatrix, XMMatrixTranslation(rand() % 200, rand() % 3 + 20.f, rand() % 200));
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Rakk"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &RakkDesc)))
		return E_FAIL;
	XMStoreFloat4x4(&RakkDesc.PresetMatrix, XMMatrixTranslation(rand() % 200, rand() % 3 + 20.f, rand() % 200));
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Rakk"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &RakkDesc)))
		return E_FAIL;
	XMStoreFloat4x4(&RakkDesc.PresetMatrix, XMMatrixTranslation(rand() % 200, rand() % 3 + 20.f, rand() % 200));
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Rakk"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &RakkDesc)))
		return E_FAIL;
	//XMStoreFloat4x4(&RakkDesc.PresetMatrix, XMMatrixTranslation(rand()%10 + 40.f, rand()%3 + 20.f, rand()%10 + 40.f));
	//if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Rakk"),
	//	ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &RakkDesc)))
	//	return E_FAIL;
	//XMStoreFloat4x4(&RakkDesc.PresetMatrix, XMMatrixTranslation(rand()%10 + 40.f, rand()%3 + 20.f, rand()%10 + 40.f));
	//if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Rakk"),
	//	ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &RakkDesc)))
	//	return E_FAIL;
	//XMStoreFloat4x4(&RakkDesc.PresetMatrix, XMMatrixTranslation(rand()%10 + 40.f, rand()%3 + 20.f, rand()%10 + 40.f));
	//if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Rakk"),
	//	ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &RakkDesc)))
	//	return E_FAIL;

	CSkag::DESC	SkagDesc;
	SkagDesc.bActive = true;
	SkagDesc.szName = L"Skag";
	SkagDesc.fRotationPerSec = XMConvertToRadians(180.f);
	SkagDesc.fSpeedPerSec = 13.f;
	SkagDesc.iLevelID = ENUM_CLASS(LEVEL::GAMEPLAY);
	SkagDesc.strVIBufferTag = TEXT("Prototype_Component_Model_Skag");
	//SkagDesc.bHasTransformPreset = true;
	//XMStoreFloat4x4(&SkagDesc.PresetMatrix, XMMatrixTranslation(50.f, 2.f, 40.f));
	SkagDesc.iNavigationIndex = 717;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Skag"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SkagDesc)))
		return E_FAIL;
	SkagDesc.iNavigationIndex = 668;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Skag"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SkagDesc)))
		return E_FAIL;
	SkagDesc.iNavigationIndex = 618;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Skag"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SkagDesc)))
		return E_FAIL;
	SkagDesc.iNavigationIndex = 718;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Skag"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SkagDesc)))
		return E_FAIL;
	SkagDesc.iNavigationIndex = 966;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Skag"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SkagDesc)))
		return E_FAIL;
	SkagDesc.iNavigationIndex = 966;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Skag"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SkagDesc)))
		return E_FAIL;
	SkagDesc.iNavigationIndex = 967;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Skag"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SkagDesc)))
		return E_FAIL;
	SkagDesc.iNavigationIndex = 852;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Skag"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SkagDesc)))
		return E_FAIL;
	SkagDesc.iNavigationIndex = 809;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Skag"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SkagDesc)))
		return E_FAIL;
	SkagDesc.iNavigationIndex = 767;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Skag"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SkagDesc)))
		return E_FAIL;
	SkagDesc.iNavigationIndex = 768;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Skag"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SkagDesc)))
		return E_FAIL;
	

	CSpiderAnt::DESC	SpiderAntDesc;
	SpiderAntDesc.bActive = true;
	SpiderAntDesc.szName = L"SpiderAnt";
	SpiderAntDesc.fRotationPerSec = XMConvertToRadians(180.f);
	SpiderAntDesc.fSpeedPerSec = 10.f;
	SpiderAntDesc.iLevelID = ENUM_CLASS(LEVEL::GAMEPLAY);
	SpiderAntDesc.strVIBufferTag = TEXT("Prototype_Component_Model_SpiderAnt");
	SpiderAntDesc.iNavigationIndex = 1011;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SpiderAnt"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SpiderAntDesc)))
		return E_FAIL;
	SpiderAntDesc.iNavigationIndex = 1012;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SpiderAnt"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SpiderAntDesc)))
		return E_FAIL;
	SpiderAntDesc.iNavigationIndex = 1325;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SpiderAnt"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SpiderAntDesc)))
		return E_FAIL;
	SpiderAntDesc.iNavigationIndex = 1294;
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SpiderAnt"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SpiderAntDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _wstring strLayerTag)
{
	auto pPlayer = static_cast<CPlayer*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::STATIC), TEXT("Layer_Player")));

	if (pPlayer != nullptr)
	{
		if (FAILED(pPlayer->Change_Level(ENUM_CLASS(LEVEL::BOSS))))
			return E_FAIL;

		return S_OK;
	}
	CPlayer::DESC PlayerDesc;
	PlayerDesc.fRotationPerSec = XMConvertToRadians(180.f);
	PlayerDesc.fSpeedPerSec = 13.f;
	PlayerDesc.iLevelID = ENUM_CLASS(LEVEL::GAMEPLAY);
	PlayerDesc.bHasTransformPreset = true;
	XMStoreFloat4x4(&PlayerDesc.PresetMatrix, XMMatrixScaling(1.5f, 1.5f, 1.5f));
	//PlayerDesc.iNavigationIndex = 32;
	PlayerDesc.iNavigationIndex = 1056;
	PlayerDesc.szName = TEXT("Player");
	PlayerDesc.strVIBufferTag = TEXT("Prototype_Component_Model_Siren_Hand");
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Player"),
		ENUM_CLASS(LEVEL::STATIC), strLayerTag, &PlayerDesc)))
		return E_FAIL;

	//CCamera_FPS* pFPSCamera = static_cast<CCamera_FPS*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::GAMEPLAY), L"Layer_Camera", CAM_FPS));
	//pFPSCamera->Initialize_Player();
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_MapObject(const _wstring strLayerTag)
{
	json jLoad;

	ifstream ifs(R"(../Bin/Resources/Map/backup/1/GamePlay.json)");
	//ifstream ifs(R"(../Bin/Resources/Map/GamePlay.json)");

	if (!ifs.is_open())
	{
		MSG_BOX("안열렸떠염");
		return E_FAIL;
	}

	ifs >> jLoad;

	// "MapObject" 키가 존재하고 배열인지 확인
	if (!jLoad.contains("MapObject") || !jLoad["MapObject"].is_array())
	{
		return E_FAIL;
	}

	m_pGameInstance->Clear_Layer(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_MapObject"));

	for (const auto& Object : jLoad["MapObject"])
	{
		// 오브젝트 이름
		_wstring ObjectName = L"";
		if (Object.contains("Name") && Object["Name"].is_string())
		{
			string tempName = Object["Name"];
			ObjectName = _wstring(tempName.begin(), tempName.end());
		}

		// 모델 이름
		_wstring ObjectModelName = L"";
		if (Object.contains("ModelName") && Object["ModelName"].is_string())
		{
			string tempModelName = Object["ModelName"];
			ObjectModelName = _wstring(tempModelName.begin(), tempModelName.end());
		}

		// Position 읽기
		_float4 vTranslation = { 0.f, 0.f, 0.f, 1.f };
		if (Object.contains("Position") && Object["Position"].is_array() && Object["Position"].size() >= 3)
		{
			vTranslation.x = Object["Position"][0];
			vTranslation.y = Object["Position"][1];
			vTranslation.z = Object["Position"][2];
		}

		// Rotation 읽기 (쿼터니언)
		_float4 vRotation = { 0.f, 0.f, 0.f, 1.f };
		if (Object.contains("Rotation") && Object["Rotation"].is_array() && Object["Rotation"].size() >= 4)
		{
			vRotation.x = Object["Rotation"][0];
			vRotation.y = Object["Rotation"][1];
			vRotation.z = Object["Rotation"][2];
			vRotation.w = Object["Rotation"][3];
		}

		// Scale 읽기
		_float4 vScale = { 1.f, 1.f, 1.f, 1.f };
		if (Object.contains("Scale") && Object["Scale"].is_array() && Object["Scale"].size() >= 3)
		{
			vScale.x = Object["Scale"][0];
			vScale.y = Object["Scale"][1];
			vScale.z = Object["Scale"][2];
		}

		// DirectX Math 벡터로 변환
		_vector objScale = XMLoadFloat4(&vScale);
		_vector objRotation = XMLoadFloat4(&vRotation);
		_vector objTranslation = XMLoadFloat4(&vTranslation);

		// 월드 매트릭스 재구성
		_matrix objWorldMat = XMMatrixScalingFromVector(objScale) *
			XMMatrixRotationQuaternion(objRotation) *
			XMMatrixTranslationFromVector(objTranslation);

		//if (ObjectModelName == L"MntRange")
		//	int a = 0;
		CGameObject::DESC mDesc;
		mDesc.fRotationPerSec = 0.f;
		mDesc.fSpeedPerSec = 0.f;
		mDesc.pParentMatrix = {};
		mDesc.pParentObject = nullptr;
		mDesc.szName = ObjectName.data();
		mDesc.strVIBufferTag = ObjectModelName;
		mDesc.bHasTransformPreset = true;        
		mDesc.iLevelID = ENUM_CLASS(LEVEL::GAMEPLAY);
		XMStoreFloat4x4(&mDesc.PresetMatrix, objWorldMat);
		_matrix PreTransMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(-90.f);
		_wstring wstrModelPath = L"../Bin/Resources/Models/Bin_NonAnim/" + ObjectModelName + L".bin";
		string strModelPath = WStringToString(wstrModelPath);

		if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), _wstring(L"Prototype_Component_Model_") + ObjectModelName,
			CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM,
				strModelPath.c_str(), PreTransMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), _wstring(L"Prototype_GameObject_") + ObjectName,
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_MapObject"), &mDesc)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _wstring strLayerTag)
{
	CGameObject::DESC desc = {};
	desc.iLevelID = ENUM_CLASS(LEVEL::GAMEPLAY);
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Snow"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &desc)))
		return E_FAIL;
	

	CUIObject::UIOBJECT_DESC UIdesc = {};

	UIdesc.iLevelID = ENUM_CLASS(LEVEL::GAMEPLAY);
	UIdesc.fSizeX = 64;
	UIdesc.fSizeY = 64;
	UIdesc.fX = g_iWinSizeX * 0.5f;
	UIdesc.fY = g_iWinSizeY * 0.5f;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ScreenSnow"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &UIdesc)))
		return E_FAIL;

	//for (size_t i = 0; i < 10; i++)
	//{
	//	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_TestSpirteEffect"),
	//		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
	//		return E_FAIL;
	//}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _wstring strLayerTag)
{
	//CGameObject::DESC desc = {};
	//desc.iLevelID = ENUM_CLASS(LEVEL::GAMEPLAY);
	//if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Snow"),
	//	ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &desc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vSpecular = _float4(0.4f, 0.4f, 0.4f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;


	CShadow::SHADOW_DESC		Desc{};
	Desc.vEye = _float4(26.f, 86.f, -28.f, 1.f);
	Desc.vAt = _float4(75.f, 4.f, 78.f, 1.f);
	Desc.fFovy = XMConvertToRadians(45.f);
	Desc.fNear = 0.1f;
	Desc.fFar = 1000.f;

	if (FAILED(m_pGameInstance->Ready_Light_For_Shadow(Desc)))
		return E_FAIL;

	//LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	//LightDesc.vPosition = _float4(30.f, 10.0f, 30.f, 1.f);
	//LightDesc.fRange = 20.f;
	//LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	//LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 0.4f);
	//LightDesc.vSpecular = _float4(1.f, 0.f, 0.f, 1.f);

	//if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Sound()
{
	m_pBGM = m_pGameInstance->Get_Single_Sound("Audio_Streaming_141");
	m_pBGM->Set_Volume(0.6f);
	m_pBGM->Play();
	m_pEnv[0] = m_pGameInstance->Get_Single_Sound("Audio_Streaming_40");
	m_pEnv[0]->Set_Volume(0.1f);
	m_pEnv[0]->Play();
	m_pEnv[1] = m_pGameInstance->Get_Single_Sound("Audio_Streaming_481");
	m_pEnv[1]->Set_Volume(0.2f);
	m_pEnv[1]->Play();

	return S_OK;
}

void CLevel_GamePlay::Intersect()
{
	m_pGameInstance->Intersect_Group(ENUM_CLASS(COL_GROUP::PLAYER), ENUM_CLASS(COL_GROUP::MONSTER), false); 
	m_pGameInstance->Intersect_Group(ENUM_CLASS(COL_GROUP::PLAYER), ENUM_CLASS(COL_GROUP::MON_ATTACK), false);
	m_pGameInstance->Intersect_Group(ENUM_CLASS(COL_GROUP::PLAYER), ENUM_CLASS(COL_GROUP::MON_BULLET), false);
	m_pGameInstance->Intersect_Group(ENUM_CLASS(COL_GROUP::MONSTER), ENUM_CLASS(COL_GROUP::MONSTER));
}

void CLevel_GamePlay::Key_Input()
{
	//if (KEY_DOWN(DIK_RETURN))
	//{
	//	Ready_Layer_Monster(TEXT("Layer_Monster"));
	//}

	if (KEY_DOWN(DIK_RETURN))
	{
		CGameObject::DESC desc = {};

		desc.bHasTransformPreset = true;
		XMStoreFloat4x4(&desc.PresetMatrix, XMMatrixTranslation(100.f, 20.f, 100.f));
		// 이거 되나용?
		desc.iLevelID = ENUM_CLASS(LEVEL::STATIC);
		if (FAILED(m_pGameInstance->Add_GameObject(desc.iLevelID, TEXT("Prototype_GameObject_WaterExplode"),
			desc.iLevelID, TEXT("Layer_Effect"), &desc)))
			return;
	}


	if (KEY_DOWN(DIK_BACKSLASH))
	{
		CCollider::bColliderDraw = !CCollider::bColliderDraw;
		CNavigation::bNavigationDraw = !CNavigation::bNavigationDraw;
	}

	if (KEY_DOWN(DIK_F1)) // using free
	{
		CCamera* pCamera = static_cast<CCamera*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::GAMEPLAY), L"Layer_Camera", CAM_FREE));
		pCamera->Set_Using(true); // 이거 다 카메라 매니저에 옮겨두쇼 제발 
		CCamera* pPrevCamera = static_cast<CCamera*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::GAMEPLAY), L"Layer_Camera", CAM_FPS));
		pPrevCamera->Set_Using(false);
		GET_PLAYER->Set_Active(false);
	}
	if (KEY_DOWN(DIK_F2)) // using fps
	{
		CCamera* pCamera = static_cast<CCamera*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::GAMEPLAY), L"Layer_Camera", CAM_FPS));
		pCamera->Set_Using(true); // 이거 다 카메라 매니저에 옮겨두쇼 제발 
		GET_PLAYER->Set_Active(true);
		CCamera* pPrevCamera = static_cast<CCamera*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::GAMEPLAY), L"Layer_Camera", CAM_FREE));
		pPrevCamera->Set_Using(false);
	}

	if (KEY_DOWN(DIK_F3))
	{
		if (FAILED(m_pGameInstance->Change_Level(static_cast<_uint>(LEVEL::LOADING),
			CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL::BOSS))))
			return;
	}


}

CLevel_GamePlay* CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_GamePlay::Free()
{
	__super::Free();
	m_pBGM->Stop();
	m_pEnv[0]->Stop();
	m_pEnv[1]->Stop();
	Safe_Release(m_pBGM);
	Safe_Release(m_pEnv[0]);
	Safe_Release(m_pEnv[1]);
}
