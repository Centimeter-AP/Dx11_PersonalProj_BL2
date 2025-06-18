#include "Level_GamePlay.h"
#include "GameInstance.h"
#include "Terrain.h"
#include "Player.h"
#include "Rakk.h"
#include "Skag.h"
#include "SpiderAnt.h"
#include "Camera.h"

#define CAM_FREE 0
#define CAM_FPS 1


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

	//ShowCursor(FALSE);

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	Key_Input();

	Intersect();
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
	desc.bHasPreset = false;
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
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Sky"),
		ENUM_CLASS(LEVEL::STATIC), strLayerTag)))
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
	CRakk::DESC RakkDesc;
	RakkDesc.bActive = true;
	RakkDesc.fRotationPerSec = XMConvertToRadians(180.f);
	RakkDesc.fSpeedPerSec = 9.f;
	RakkDesc.iLevelID = ENUM_CLASS(LEVEL::GAMEPLAY);
	RakkDesc.strVIBufferTag = TEXT("Prototype_Component_Model_Rakk");
	RakkDesc.bHasPreset = true;
	XMStoreFloat4x4(&RakkDesc.PresetMatrix, XMMatrixTranslation(rand()%10 + 40.f, rand()%3 + 13.f, rand()%10 + 40.f));
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Rakk"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &RakkDesc)))
		return E_FAIL;

	CSkag::DESC	SkagDesc;
	SkagDesc.bActive = true;
	SkagDesc.fRotationPerSec = XMConvertToRadians(180.f);
	SkagDesc.fSpeedPerSec = 8.f;
	SkagDesc.iLevelID = ENUM_CLASS(LEVEL::GAMEPLAY);
	SkagDesc.strVIBufferTag = TEXT("Prototype_Component_Model_Skag");
	SkagDesc.bHasPreset = true;
	XMStoreFloat4x4(&SkagDesc.PresetMatrix, XMMatrixTranslation(50.f, 2.f, 40.f));
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Skag"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SkagDesc)))
		return E_FAIL;

	//for (size_t i = 0; i < 5; i++)
	//{
	//	XMStoreFloat4x4(&SkagDesc.PresetMatrix, XMMatrixTranslation(rand()%100, 2.f, rand()%100));
	//	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Skag"),
	//		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SkagDesc)))
	//		return E_FAIL;
	//}

	CSpiderAnt::DESC	SpiderAntDesc;
	SpiderAntDesc.bActive = true;
	SpiderAntDesc.fRotationPerSec = XMConvertToRadians(180.f);
	SpiderAntDesc.fSpeedPerSec = 7.f;
	SpiderAntDesc.iLevelID = ENUM_CLASS(LEVEL::GAMEPLAY);
	SpiderAntDesc.strVIBufferTag = TEXT("Prototype_Component_Model_SpiderAnt");
	SpiderAntDesc.bHasPreset = true;
	XMStoreFloat4x4(&SpiderAntDesc.PresetMatrix, XMMatrixTranslation(60.f, 2.f, 50.f));
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_SpiderAnt"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &SpiderAntDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _wstring strLayerTag)
{
	CPlayer::DESC PlayerDesc;
	PlayerDesc.fRotationPerSec = XMConvertToRadians(180.f);
	PlayerDesc.fSpeedPerSec = 10.f;
	PlayerDesc.iLevelID = ENUM_CLASS(LEVEL::STATIC);
	PlayerDesc.bHasPreset = true;
	XMStoreFloat4x4(&PlayerDesc.PresetMatrix, XMMatrixTranslation(39.f, 2.f, 33.26f));
	PlayerDesc.szName = TEXT("Player");
	PlayerDesc.strVIBufferTag = TEXT("Prototype_Component_Model_Siren_Hand");
	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Player"),
		ENUM_CLASS(LEVEL::STATIC), strLayerTag, &PlayerDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_MapObject(const _wstring strLayerTag)
{
	json jLoad;

	ifstream ifs(R"(../Bin/Resources/Map/GamePlay.json)");

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
		mDesc.bHasPreset = true;        
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

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Intersect()
{
	//m_pGameInstance->Intersect_Group(ENUM_CLASS(COL_GROUP::PLAYER), ENUM_CLASS(COL_GROUP::MONSTER)); 
	m_pGameInstance->Intersect_Group(ENUM_CLASS(COL_GROUP::MONSTER), ENUM_CLASS(COL_GROUP::MONSTER));
	//m_pGameInstance->Intersect_Group(ENUM_CLASS(COL_GROUP::PLAYER), ENUM_CLASS(COL_GROUP::MON_BULLET)); 
}

void CLevel_GamePlay::Key_Input()
{
	if (KEY_DOWN(DIK_RETURN))
	{
		Ready_Layer_Monster(TEXT("Layer_Monster"));
	}

	if (KEY_DOWN(DIK_BACKSLASH))
		CCollider::bColliderDraw = !CCollider::bColliderDraw;

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
}
