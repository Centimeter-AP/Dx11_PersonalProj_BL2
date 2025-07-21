#include "Level_Boss.h"
#include "GameInstance.h"
#include "Terrain.h"
#include "Player.h"
#include "Rakk.h"
#include "Skag.h"
#include "SpiderAnt.h"
#include "Leviathan.h"
#include "Camera.h"
#include "Sky.h"

#define CAM_FREE 0
#define CAM_FPS 1


CLevel_Boss::CLevel_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Boss::Initialize()
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

	if (FAILED(Ready_Layer_Boss(TEXT("Layer_Boss"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_MapObject(TEXT("Layer_MapObject"))))
		return E_FAIL;

	if (FAILED(Ready_Sound()))
		return E_FAIL;



	//ShowCursor(FALSE);

	return S_OK;
}

void CLevel_Boss::Update(_float fTimeDelta)
{
	Key_Input();

	Intersect();
}

HRESULT CLevel_Boss::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_BackGround(const _wstring strLayerTag)
{
	CTerrain::DESC desc;
	desc.iLevelID = ENUM_CLASS(LEVEL::BOSS);
	desc.isToolver = false;
	desc.fRotationPerSec = 0.f;
	desc.fSpeedPerSec = 0.f;
	desc.bHasTransformPreset = false;
	desc.pParentMatrix = nullptr;
	desc.pParentObject = nullptr;
	desc.strVIBufferTag = L"";
	desc.szName = L"";
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_Terrain"),
		ENUM_CLASS(LEVEL::BOSS), strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Sky(const _wstring strLayerTag)
{
	CSky::DESC skydesc = {};
	skydesc.iLevelID = ENUM_CLASS(LEVEL::STATIC);
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Sky"),
		ENUM_CLASS(LEVEL::STATIC), strLayerTag, &skydesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Camera(const _wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_Camera_Free"),
		ENUM_CLASS(LEVEL::BOSS), strLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_Camera_FPS"),
		ENUM_CLASS(LEVEL::BOSS), strLayerTag)))
		return E_FAIL;

	CCamera* pPrevCamera = static_cast<CCamera*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::BOSS), L"Layer_Camera", CAM_FREE));
	pPrevCamera->Set_Using(false);

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Monster(const _wstring strLayerTag)
{
	CRakk::DESC RakkDesc;
	RakkDesc.bActive = true;
	RakkDesc.fRotationPerSec = XMConvertToRadians(180.f);
	RakkDesc.fSpeedPerSec = 10.f;
	RakkDesc.iLevelID = ENUM_CLASS(LEVEL::BOSS);
	RakkDesc.strVIBufferTag = TEXT("Prototype_Component_Model_Rakk");
	RakkDesc.bHasTransformPreset = true;
	RakkDesc.iNavigationIndex = 50;
	XMStoreFloat4x4(&RakkDesc.PresetMatrix, XMMatrixTranslation(rand()%10 + 40.f, rand()%3 + 13.f, rand()%10 + 40.f));
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_Rakk"),
		ENUM_CLASS(LEVEL::BOSS), strLayerTag, &RakkDesc)))
		return E_FAIL;

	CSkag::DESC	SkagDesc;
	SkagDesc.bActive = true;
	SkagDesc.fRotationPerSec = XMConvertToRadians(180.f);
	SkagDesc.fSpeedPerSec = 13.f;
	SkagDesc.iLevelID = ENUM_CLASS(LEVEL::BOSS);
	SkagDesc.strVIBufferTag = TEXT("Prototype_Component_Model_Skag");
	SkagDesc.bHasTransformPreset = true;
	SkagDesc.iNavigationIndex = 49;
	XMStoreFloat4x4(&SkagDesc.PresetMatrix, XMMatrixTranslation(50.f, 2.f, 40.f));
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_Skag"),
		ENUM_CLASS(LEVEL::BOSS), strLayerTag, &SkagDesc)))
		return E_FAIL;

	//for (size_t i = 0; i < 5; i++)
	//{
	//	XMStoreFloat4x4(&SkagDesc.PresetMatrix, XMMatrixTranslation(rand()%100, 2.f, rand()%100));
	//	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_Skag"),
	//		ENUM_CLASS(LEVEL::BOSS), strLayerTag, &SkagDesc)))
	//		return E_FAIL;
	//}

	CSpiderAnt::DESC	SpiderAntDesc;
	SpiderAntDesc.bActive = true;
	SpiderAntDesc.fRotationPerSec = XMConvertToRadians(180.f);
	SpiderAntDesc.fSpeedPerSec = 10.f;
	SpiderAntDesc.iLevelID = ENUM_CLASS(LEVEL::BOSS);
	SpiderAntDesc.strVIBufferTag = TEXT("Prototype_Component_Model_SpiderAnt");
	SpiderAntDesc.bHasTransformPreset = true;
	SpiderAntDesc.iNavigationIndex = 51;
	XMStoreFloat4x4(&SpiderAntDesc.PresetMatrix, XMMatrixTranslation(60.f, 2.f, 50.f));
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_SpiderAnt"),
		ENUM_CLASS(LEVEL::BOSS), strLayerTag, &SpiderAntDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Boss(const _wstring strLayerTag)
{
	CLeviathan::DESC	LeviDesc;
	LeviDesc.bActive = true;
	LeviDesc.fRotationPerSec = XMConvertToRadians(180.f);
	LeviDesc.fSpeedPerSec = 10.f;
	LeviDesc.iLevelID = ENUM_CLASS(LEVEL::BOSS);
	LeviDesc.strVIBufferTag = TEXT("Prototype_Component_Model_Leviathan");
	LeviDesc.bHasTransformPreset = true;
	LeviDesc.iNavigationIndex = 51;
	XMStoreFloat4x4(&LeviDesc.PresetMatrix, XMMatrixScaling(2.f, 2.f, 2.f)*XMMatrixRotationRollPitchYaw(0.f, XMConvertToRadians(180.f), 0.f) * XMMatrixTranslation(150.f, 10.f, 280.f));
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::BOSS), TEXT("Prototype_GameObject_Leviathan"),
		ENUM_CLASS(LEVEL::BOSS), strLayerTag, &LeviDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_UI(const _wstring strLayerTag)
{

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Player(const _wstring strLayerTag)
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
	PlayerDesc.fSpeedPerSec = 10.f;
	PlayerDesc.iLevelID = ENUM_CLASS(LEVEL::BOSS);
	PlayerDesc.bHasTransformPreset = true;
	XMStoreFloat4x4(&PlayerDesc.PresetMatrix, XMMatrixScaling(1.5f, 1.5f, 1.5f));
	PlayerDesc.iNavigationIndex = 32;
	PlayerDesc.szName = TEXT("Player");
	PlayerDesc.strVIBufferTag = TEXT("Prototype_Component_Model_Siren_Hand");
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Player"),
		ENUM_CLASS(LEVEL::STATIC), strLayerTag, &PlayerDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_MapObject(const _wstring strLayerTag)
{
	json jLoad;

	ifstream ifs(R"(../Bin/Resources/Map/backup/2/GamePlay.json)");

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
		mDesc.iLevelID = ENUM_CLASS(LEVEL::BOSS);
		XMStoreFloat4x4(&mDesc.PresetMatrix, objWorldMat);
		_matrix PreTransMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(-90.f);
		_wstring wstrModelPath = L"../Bin/Resources/Models/Bin_NonAnim/" + ObjectModelName + L".bin";
		string strModelPath = WStringToString(wstrModelPath);

		if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::BOSS), _wstring(L"Prototype_Component_Model_") + ObjectModelName,
			CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM,
				strModelPath.c_str(), PreTransMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::BOSS), _wstring(L"Prototype_GameObject_") + ObjectName,
			ENUM_CLASS(LEVEL::BOSS), TEXT("Layer_MapObject"), &mDesc)))
			return E_FAIL;
	}
	return S_OK;
}	

HRESULT CLevel_Boss::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.5f, 0.5f, 0.5f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.3f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;


	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.fRange = 100.f;
	LightDesc.vAmbient = _float4(0.06f, 0.02f, 0.1f, 1.0f);
	LightDesc.vDiffuse = _float4(0.20f, 0.08f, 0.40f, 1.0f);
	//LightDesc.vSpecular = _float4(0.75f, 0.60f, 1.00f, 1.0f);
	LightDesc.vSpecular = _float4(0.10f, 0.04f, 0.20f, 1.0f);


#define ADD_PURPLE_POINTLIGHT(x, y, z)							\
	LightDesc.vPosition = _float4(x, y, z, 1.f);		\
	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))	\
		return E_FAIL;


	ADD_PURPLE_POINTLIGHT(169.153, 117.39, 122.182);
	ADD_PURPLE_POINTLIGHT(254.933, 135.458, -3.00446);
	ADD_PURPLE_POINTLIGHT(67.2147, 52.4714, 206.151);
	ADD_PURPLE_POINTLIGHT(43.101, 69.8857, 146.281);
	ADD_PURPLE_POINTLIGHT(59.199, 123.539, 103.168);
	ADD_PURPLE_POINTLIGHT(38.7896, 126.28, -32.413);
	ADD_PURPLE_POINTLIGHT(13.7532, 180.337, -12.3291);
	ADD_PURPLE_POINTLIGHT(34.9153, 163.112, -116.858);
	ADD_PURPLE_POINTLIGHT(-4.39817, 81.6552, 77.3488);
	ADD_PURPLE_POINTLIGHT(-8.47138, 76.2952, 146.465);
	ADD_PURPLE_POINTLIGHT(23.9756, 88.7342, 335.816);
	ADD_PURPLE_POINTLIGHT(-33.3746, 95.774, 291.532);
	ADD_PURPLE_POINTLIGHT(-61.3469, 151.309, 244.766);
	ADD_PURPLE_POINTLIGHT(-23.349, 206.952, 359.55);
	ADD_PURPLE_POINTLIGHT(-18.2155, 241.409, 309.554);
	ADD_PURPLE_POINTLIGHT(30.938, 285.092, 388.23);
	ADD_PURPLE_POINTLIGHT(79.733, 300.352, 394.54);
	ADD_PURPLE_POINTLIGHT(77.6912, 269.097, 372.781);
	ADD_PURPLE_POINTLIGHT(128.411, 286.409, 377.383);
	ADD_PURPLE_POINTLIGHT(272.035, 135.197, 338.068);
	ADD_PURPLE_POINTLIGHT(418.931, 136.11, 298.014);
	ADD_PURPLE_POINTLIGHT(372.732, 138.364, 399.353);
	ADD_PURPLE_POINTLIGHT(312.019, 72.8633, 80.8681);
	ADD_PURPLE_POINTLIGHT(327.616, 102.94, 83.7235);
	ADD_PURPLE_POINTLIGHT(377.857, 172.164, 178.702);
	ADD_PURPLE_POINTLIGHT(167.26, 149.359, 365.384);
	ADD_PURPLE_POINTLIGHT(209.859, 108.633, -48.6191);
	ADD_PURPLE_POINTLIGHT(268.129, 8.91216, 278.191);
	ADD_PURPLE_POINTLIGHT(251.793, 62.4851, 308.733);
	ADD_PURPLE_POINTLIGHT(150.316, 71.3566, 157.674);
	ADD_PURPLE_POINTLIGHT(196.579, 102.501, 104.988);
	ADD_PURPLE_POINTLIGHT(-86.8788, 193.923, -76.2842);
	ADD_PURPLE_POINTLIGHT(-46.8368, 206.643, -132.747);
	ADD_PURPLE_POINTLIGHT(-99.8456, 97.7251, 28.701);




	return S_OK;
}

HRESULT CLevel_Boss::Ready_Sound()
{
	m_pBGM = m_pGameInstance->Get_Single_Sound("Audio_Streaming_262");
	m_pBGM->Set_Volume(0.4f);
	m_pBGM->Play();
	m_pEnv[0] = m_pGameInstance->Get_Single_Sound("Audio_Streaming_179");
	m_pEnv[0]->Set_Volume(0.05f);
	m_pEnv[0]->Play();
	m_pEnv[1] = m_pGameInstance->Get_Single_Sound("Audio_Streaming_176");
	m_pEnv[1]->Set_Volume(0.2f);
	m_pEnv[1]->Play();
	m_pEnv[2] = m_pGameInstance->Get_Single_Sound("Audio_Streaming_255");
	m_pEnv[2]->Set_Volume(0.1f);
	m_pEnv[2]->Play();
	return S_OK;
}

void CLevel_Boss::Intersect()
{
	m_pGameInstance->Intersect_Group(ENUM_CLASS(COL_GROUP::PLAYER), ENUM_CLASS(COL_GROUP::MONSTER));
	m_pGameInstance->Intersect_Group(ENUM_CLASS(COL_GROUP::PLAYER), ENUM_CLASS(COL_GROUP::BOSS), false);
	m_pGameInstance->Intersect_Group(ENUM_CLASS(COL_GROUP::MONSTER), ENUM_CLASS(COL_GROUP::MONSTER));
	m_pGameInstance->Intersect_Group(ENUM_CLASS(COL_GROUP::PLAYER), ENUM_CLASS(COL_GROUP::MON_ATTACK), false);
	m_pGameInstance->Intersect_Group(ENUM_CLASS(COL_GROUP::PLAYER), ENUM_CLASS(COL_GROUP::MON_BULLET));
}
 
void CLevel_Boss::Key_Input()
{
	if (KEY_DOWN(DIK_RETURN))
	{
		CGameObject::DESC desc = {};

		desc.bHasTransformPreset = true;
		XMStoreFloat4x4(&desc.PresetMatrix, XMMatrixTranslation(110.f, 90.f, 110.f));
		// 이거 되나용?
		desc.iLevelID = ENUM_CLASS(LEVEL::BOSS);
  		if (FAILED(m_pGameInstance->Add_GameObject(desc.iLevelID, TEXT("Prototype_GameObject_WaterExplode"),
			desc.iLevelID, TEXT("Layer_Effect"), &desc)))
			return ;
	}

	if (KEY_DOWN(DIK_BACKSLASH))
	{
		CCollider::bColliderDraw = !CCollider::bColliderDraw;
		CNavigation::bNavigationDraw = !CNavigation::bNavigationDraw;
	}

	if (KEY_DOWN(DIK_F1)) // using free
	{
		CCamera* pCamera = static_cast<CCamera*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::BOSS), L"Layer_Camera", CAM_FREE));
		pCamera->Set_Using(true); // 이거 다 카메라 매니저에 옮겨두쇼 제발 
		CCamera* pPrevCamera = static_cast<CCamera*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::BOSS), L"Layer_Camera", CAM_FPS));
		pPrevCamera->Set_Using(false);
		GET_PLAYER->Set_Active(false);
	}
	if (KEY_DOWN(DIK_F2)) // using fps
	{
		CCamera* pCamera = static_cast<CCamera*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::BOSS), L"Layer_Camera", CAM_FPS));
		pCamera->Set_Using(true); // 이거 다 카메라 매니저에 옮겨두쇼 제발 
		GET_PLAYER->Set_Active(true);
		CCamera* pPrevCamera = static_cast<CCamera*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::BOSS), L"Layer_Camera", CAM_FREE));
		pPrevCamera->Set_Using(false);
	}

	//if (KEY_DOWN(DIK_SPACE))
	//{
	//	auto vCamPos = m_pGameInstance->Get_CamPosition();
	//	cout << vCamPos->x << ", " << vCamPos->y << ", " << vCamPos->z << endl;

	//	LIGHT_DESC			LightDesc{};

	//	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	//	LightDesc.vPosition = *vCamPos;
	//	LightDesc.fRange = 130.f;
	//	// Ambient : 아주 어두운 보라
	//	LightDesc.vAmbient = _float4(0.06f, 0.02f, 0.1f, 1.0f);
	//	// Diffuse : 어두운 자수정 컬러
	//	LightDesc.vDiffuse = _float4(0.20f, 0.08f, 0.40f, 1.0f);
	//	// Specular: 살짝 푸른 빛 도는 하이라이트, Power=28 (중앙이 또렷하게)
	//	LightDesc.vSpecular = _float4(0.75f, 0.60f, 1.00f, 1.0f);
	//	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
	//		return ;


	//}

}

CLevel_Boss* CLevel_Boss::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Boss* pInstance = new CLevel_Boss(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Boss");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Boss::Free()
{
	__super::Free();
	m_pBGM->Stop();
	m_pEnv[0]->Stop();
	m_pEnv[1]->Stop();
	Safe_Release(m_pBGM);
	Safe_Release(m_pEnv[0]);
	Safe_Release(m_pEnv[1]);
	Safe_Release(m_pEnv[2]);
}
