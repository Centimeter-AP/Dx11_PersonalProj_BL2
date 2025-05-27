#include "GameInstance.h"
#include "ObjectTool.h"
#include "Terrain.h"
#include "Monster.h"
#include <sstream>
#include "Layer.h"
#include "Camera_Tool.h"
#include "Camera_FPS.h"
//ImGuiFileDialog g_ImGuiFileDialog;
//ImGuiFileDialog::Instance() 이래 싱글톤으로 쓰라고 신이 말하고 감


CObjectTool::CObjectTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImGuiTool{ pDevice, pContext }
{

}

CObjectTool::CObjectTool(const CObjectTool& Prototype)
	: CImGuiTool(Prototype)
{
}

HRESULT CObjectTool::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CObjectTool::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CObjectTool::Priority_Update(_float fTimeDelta)
{

}

EVENT CObjectTool::Update(_float fTimeDelta)
{
	Key_Input();
	return EVN_NONE;
}

void CObjectTool::Late_Update(_float fTimeDelta)
{
}

HRESULT CObjectTool::Render()
{
	if (m_pWindowData->ShowObjectMenu)
	{
		if (FAILED(Render_ObjectTool()))
			return E_FAIL;
	}

	return S_OK;
}

void CObjectTool::Key_Input()
{
	if (KEY_DOWN(DIK_F1))
	{
		CCamera_Tool* pCamera = static_cast<CCamera_Tool*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::MAPTOOL), L"Layer_Camera", 0));
		pCamera->Set_Using(true); // 이거 다 카메라 매니저에 옮겨두쇼 제발 
		if (m_isPlayerExists)
		{
			CCamera_FPS* pPrevCamera = static_cast<CCamera_FPS*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::MAPTOOL), L"Layer_Camera", 1));
			pPrevCamera->Set_Using(false);
		}
	}
	if (KEY_DOWN(DIK_F2))
	{
		if (m_isPlayerExists)
		{
			CCamera_FPS* pCamera = static_cast<CCamera_FPS*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::MAPTOOL), L"Layer_Camera", 1));
			pCamera->Set_Using(true); // 이거 다 카메라 매니저에 옮겨두쇼 제발 
			CCamera_Tool* pPrevCamera = static_cast<CCamera_Tool*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::MAPTOOL), L"Layer_Camera", 0));
			pPrevCamera->Set_Using(false);
		}
	}

	if (KEY_PRESSING(DIK_LCONTROL))
	{
		if (m_isGizmoEnable)
		{
			if (KEY_DOWN(DIK_W))
			{
				m_eOperation = ImGuizmo::TRANSLATE;
			}
			else if (KEY_DOWN(DIK_E))
			{
				m_eOperation = ImGuizmo::ROTATE;
			}
			else if (KEY_DOWN(DIK_R))
			{
				m_eOperation = ImGuizmo::SCALE;
			}
		}

		if (KEY_DOWN(DIK_Q))
		{
			m_isGizmoEnable = !m_isGizmoEnable;
		}

		if (KEY_DOWN(DIK_S))
		{
			m_pWindowData->ShowSaveMenu = true;
		}

		if (KEY_DOWN(DIK_S))
		{
			m_pWindowData->ShowLoadMenu = true;
		}
	}
	if (m_isGizmoEnable)
	{
		if (MOUSE_DOWN(DIM::LBUTTON))
		{
			CGameObject* pPickedObj = m_pGameInstance->Pick_Object_In_Layer(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_MapObject"), m_fMeshPickedPosition);
			if (nullptr == pPickedObj)
				return ;
			else
				m_pSelectedObj = pPickedObj;
		}
	}
}

HRESULT CObjectTool::Render_ObjectTool()
{
	ImGui::SetNextWindowSize(ImVec2(200, 300));
	ImGui::Begin("Object Tools", &m_pWindowData->ShowObjectMenu, NULL);

	if (Button("Make Player"))
	{
		if (FAILED(Make_Player()))
		{
			ImGui::End();
			return E_FAIL;
		}
	}
	ImGui::SeparatorText("Find Object");

	if (Button("Open Directory"))
	{
		IGFD::FileDialogConfig config;
		if (m_isAnim)
			config.path = R"(..\Bin\Resources\Models\Bin_Anim)";
		else
			config.path = R"(..\Bin\Resources\Models\Bin_NonAnim)";
		//config.path = R"(C:\Users\CMAP\Documents\Dx11_Personal_Projects\3d\testing)";
		IFILEDIALOG->OpenDialog("BINDialog", "Select BIN Files", nullptr, config);
	}
	SameLine();
	Checkbox("Animation", &m_isAnim);
	if (m_isAnim == true)
		m_eModelType = MODEL::ANIM;
	else
		m_eModelType = MODEL::NONANIM;
	if (IFILEDIALOG->Display("BINDialog"))
	{
		if (IFILEDIALOG->IsOk())
		{
			path FDCurPath = IFILEDIALOG->GetCurrentPath();
			if (FAILED(Open_ModelDirectory(FDCurPath)))
				return E_FAIL;
		}
		IFILEDIALOG->Close();
	}

	static int item_selected_idx = 0; // Here we store our selected data as an index.
	
	int item_highlighted_idx = -1; // Here we store our highlighted data as an index.

	if (ImGui::BeginListBox("##listbox 1", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int n = 0; n < m_ModelNames.size(); n++)
		{
			//const char* szModelName = WStringToString(m_ModelNames[n]).c_str();
			const bool is_selected = (item_selected_idx == n);
			if (ImGui::Selectable(WStringToString(m_ModelNames[n]).c_str(), is_selected))
				item_selected_idx = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	CMonster::DESC mDesc;
	if (Button("Make Object"))
	{
		wstring ObjectTag;
		if (m_isAnim)
			ObjectTag = L"Monster";
		else
			ObjectTag = L"MapObject";
		mDesc.strVIBufferTag = m_ModelNames[item_selected_idx];
		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), _wstring(L"Prototype_GameObject_") + ObjectTag,
			ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_MapObject"), &mDesc)))
			return E_FAIL;
	}


	ImGui::Checkbox("Enable Gizmo", &m_isGizmoEnable);
	ImGui::Checkbox("Snap to Terrain?", &m_isObjectSnapToTerrain);

	ImGui::SetNextItemWidth(80);
	ImGui::InputFloat("Snap Offset", &m_fSnapOffset, 0.1f, 1.f, "%.1f");
	

	static _float test[9] = {};
	if (m_pSelectedObj != nullptr)
	{
		_matrix matObjWorld = m_pSelectedObj->Get_Transform()->Get_WorldMatrix();
		_vector  vecObjScale, vecObjRot, vecObjPos = {};
		XMMatrixDecompose(&vecObjScale, &vecObjRot, &vecObjPos, matObjWorld);
		_float4 ObjScale, ObjRot, ObjPos = {};
		XMStoreFloat4(&ObjScale, vecObjScale);
		XMStoreFloat4(&ObjRot, vecObjRot);
		XMStoreFloat4(&ObjPos, vecObjPos);
		ImGui::SeparatorText("Manipulate Object");
		ImGui::DragFloat("Pos X", &ObjPos.x, 0.05f);
		ImGui::DragFloat("Pos Y", &ObjPos.y, 0.05f);
		ImGui::DragFloat("Pos Z", &ObjPos.z, 0.05f);
		ImGui::Separator();
		ImGui::DragFloat("Rot X", &ObjRot.x, 1.f);
		ImGui::DragFloat("Rot Y", &ObjRot.y, 1.f);
		ImGui::DragFloat("Rot Z", &ObjRot.z, 1.f);
		ImGui::Separator();
		ImGui::DragFloat("Scale X", &ObjScale.x, 0.05f);
		ImGui::DragFloat("Scale Y", &ObjScale.y, 0.05f);
		ImGui::DragFloat("Scale Z", &ObjScale.z, 0.05f);
		
		//먼가 이상한디?
		m_pSelectedObj->Get_Transform()->Set_Matrix(XMMatrixAffineTransformation(XMLoadFloat4(&ObjScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&ObjRot), XMLoadFloat4(&ObjPos)));
	}


	ImGui::End();

	Guizmo_Tool();
	

	return S_OK;
}

HRESULT CObjectTool::Guizmo_Tool()
{
	if (m_isGizmoEnable && m_pSelectedObj != nullptr)
	{
		// ImGui 프레임 내부에서 호출
		ImGuizmo::BeginFrame(); // ← 한 번만 호출

		ImGuizmo::SetOrthographic(false); // 투영 방식 설정 (false = Perspective)
		ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList()); // 기본 ImGui drawlist 사용

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGuizmo::SetRect(viewport->Pos.x, viewport->Pos.y, viewport->Size.x, viewport->Size.y);

		//auto selectedObject = m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Monster"), m_iSelectObjectIndex);

		auto pObjTransformCom = m_pSelectedObj->Get_Transform();

		_float4x4 matView = *m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW);
		_float4x4 matProj = *m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ);

		/* 스냅 오프셋 조절, 회전은 15도 기준*/
		_float snapoffset[3] = {};
		if (KEY_PRESSING(DIK_LSHIFT))
		{
			if (m_eOperation == ImGuizmo::ROTATE)
				snapoffset[0] = 15.f;
			else
				snapoffset[0] = snapoffset[1] = snapoffset[2] = 1.f;
		}

		/* 실제 조작하는 부분 */
		ImGuizmo::Manipulate(
			reinterpret_cast<const _float*>(&matView),
			reinterpret_cast<const _float*>(&matProj),
			m_eOperation, m_eMode,
			reinterpret_cast<float*>(&pObjTransformCom->Get_WorldMatrix4x4Ref()),
			nullptr, snapoffset);

		if (m_isObjectSnapToTerrain)
		{
			auto pTerrain = dynamic_cast<CTerrain*>(m_pGameInstance->Find_Object(
				ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Terrain"), 0));

			auto pVIBuffer = dynamic_cast<CVIBuffer_Terrain*>(pTerrain->Get_Component(TEXT("Com_VIBuffer")));
			_float x = pObjTransformCom->Get_WorldMatrix4x4Ref()._41;
			_float z = pObjTransformCom->Get_WorldMatrix4x4Ref()._43;
			_float y = pVIBuffer->Get_Height(x, z);

			_float yOffset = 0.1f;
			XMVECTOR fixedPos = XMVectorSet(x, y + yOffset, z, 1.0f);
			m_pSelectedObj->Get_Transform()->Set_State(STATE::POSITION, fixedPos);
		}
	}
	return S_OK;
}

HRESULT CObjectTool::Window_ObjectList()
{
	ImGui::Begin("Object Lists");
	if (ImGui::TreeNode("Basic trees"))
	{

	}
	//if (ImGui::BeginListBox("##Current Object List", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
	//{
	//	for (int n = 0; n < m_ModelNames.size(); n++)
	//	{
	//		//const char* szModelName = WStringToString(m_ModelNames[n]).c_str();
	//		const bool is_selected = (item_selected_idx == n);
	//		if (ImGui::Selectable(WStringToString(m_ModelNames[n]).c_str(), is_selected))
	//			item_selected_idx = n;

	//		// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
	//		if (is_selected)
	//			ImGui::SetItemDefaultFocus();
	//	}
	//	ImGui::EndListBox();
	//}

	ImGui::End();
	return S_OK;
}

HRESULT CObjectTool::Make_Player()
{
	if (m_isPlayerExists)
	{
		MSG_BOX("이미 플레이어가 있어용");
		return S_OK;
	}
	else
	{
		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Player"), ENUM_CLASS(LEVEL::STATIC), L"Layer_Player")))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Camera_FPS"), ENUM_CLASS(LEVEL::MAPTOOL), L"Layer_Camera")))
			return E_FAIL;
		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_AssaultRifle"), ENUM_CLASS(LEVEL::STATIC), L"Layer_Weapon")))
			return E_FAIL;
		CCamera* pPrevCamera = static_cast<CCamera*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::MAPTOOL), L"Layer_Camera"));
		pPrevCamera->Set_Using(false);
		m_isPlayerExists = true;
	}
	return S_OK;
}


HRESULT CObjectTool::Open_ModelDirectory(path& CurPath)
{
	if (!exists(CurPath))
	{
		MSG_BOX("경로 잘못 들어오기도 쉽지 않은데");
		return E_FAIL;
	}

	_matrix PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	_matrix PreTransMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(-90.f);
	for (const auto& entry : directory_iterator(CurPath))
	{
		if (entry.is_regular_file() && entry.path().extension() == L".bin")
		{
			string filePath = entry.path().string();
			_wstring stemName = entry.path().stem().wstring();
			_wstring prototypeTag = L"Prototype_Component_Model_" + stemName;
			if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::MAPTOOL), prototypeTag,
				CModel::Create(m_pDevice, m_pContext, m_eModelType,
					filePath.c_str(), PreTransMatrix))))
			{
				std::string msg = "파일 오픈 실패:\n대상 경로: ";
				msg += filePath;

				// 2) MessageBoxA로 출력
				MessageBoxA(nullptr, msg.c_str(), "오류", MB_OK | MB_ICONERROR);
				continue;
			}

			string strRelativePath = filesystem::relative(entry.path(), filesystem::current_path()).string();
			//m_DataPaths[entry.path().stem().string()] = strRelativePath; // map으로 파일 위치 상대경로로 변환해서 저장? 왜했지
			m_ModelNames.push_back(stemName);
		}
	}

	return S_OK;
}

HRESULT CObjectTool::Open_SaveFile()
{
	path savePath = R"(..\Bin\Resources\Map\)";

	IGFD::FileDialogConfig config;
	config.path = R"(..\Bin\Resources\Map\)";
	config.flags = ImGuiFileDialogFlags_ConfirmOverwrite | (ImGuiFileDialogFlags_ReadOnlyFileNameField == false);

	IFILEDIALOG->OpenDialog("SaveMapDialog", "Choose directory to save", ".map", config);

	if (IFILEDIALOG->Display("SaveMapDialog"))
	{
		if (IFILEDIALOG->IsOk())
		{
			path savePath = IFILEDIALOG->GetFilePathName();

			// 확장자가 없으면 .map 붙이기
			if (savePath.extension().string() != ".map")
				savePath += ".map";

			Save_Objects(savePath); // 직접 작성한 저장 함수
		}

		IFILEDIALOG->Close();
	}

	return S_OK;
}

HRESULT CObjectTool::Autosave()
{
	path savePath = R"(..\Bin\Resources\Map\Autosave\)";
	auto now = chrono::system_clock::now();
	time_t t = chrono::system_clock::to_time_t(now);
	tm local_tm;
	localtime_s(&local_tm, &t);  // thread-safe 함수

	ostringstream oss; // 문자열에 출력하는 스트림 변수...
	oss << savePath
		<< std::put_time(&local_tm, "%Y-%m-%d-%H-%M")
		<< ".map";

	Save_Objects(savePath);
	return S_OK;
}

HRESULT CObjectTool::Save_Objects(path SavePath)
{
	ofstream ofs(SavePath, ios::binary);

	auto pLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::MAPTOOL), L"Layer_MapObject");

	auto& Objects = pLayer->Get_LayerObjectLists();
	
	size_t iObjectSize = { Objects.size() };
	ofs.write(reinterpret_cast<const char*>(&iObjectSize), sizeof(size_t)); // 한 레이어에 오브젝트 몇 갠지 저장
	for (auto Object : Objects)
	{
		_tchar* ObjectName = Object->Get_Name();
		//_uint ObjectNameSize = ;
		//ofs.write(reinterpret_cast<const char*>(&ObjectNameSize), sizeof(_uint)); // 한 레이어에 오브젝트 몇 갠지 저장
		
		//ofs.write(reinterpret_cast<const char*>(ObjectName.data()), sizeof(_tchar) * 10); // 한 레이어에 오브젝트 몇 갠지 저장
		/*
			오브젝트 이름(프로토타입_게임오브젝트_이름<-이거)
			모델 이름(VIBufferTag)
			모델 파일 경로? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
			Transform->WorldMatrix
			Anim/Nonanim(이거저장해야함?)해야할거같은데용
			생성할 레벨
			pretransformmatrix?

		*/
	}

	//ofs.write(reinterpret_cast<const char*>(&a), sizeof(_uint));

	return S_OK;
}

CObjectTool* CObjectTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CObjectTool* pInstance = new CObjectTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CObjectTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CObjectTool::Clone(void* pArg)
{
	CObjectTool* pInstance = new CObjectTool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CObjectTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CObjectTool::Free()
{
	__super::Free();

}
