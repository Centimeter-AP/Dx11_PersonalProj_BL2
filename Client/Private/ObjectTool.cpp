#include "GameInstance.h"
#include "ObjectTool.h"
#include "Terrain.h"
#include "Monster.h"

//ImGuiFileDialog g_ImGuiFileDialog;
//ImGuiFileDialog::Instance() �̷� �̱������� ����� ���� ���ϰ� ��


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

void CObjectTool::Update(_float fTimeDelta)
{
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
	}
	if (m_isGizmoEnable)
	{
		if (MOUSE_DOWN(DIM::LBUTTON))
		{
			CGameObject* pPickedObj = m_pGameInstance->Pick_Object_In_Layer(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_MapObject"), m_fMeshPickedPosition);
			if (nullptr == pPickedObj)
				return;
			else
				m_pSelectedObj = pPickedObj;
		}
	}
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

HRESULT CObjectTool::Render_ObjectTool()
{
	ImGui::SetNextWindowSize(ImVec2(200, 300));
	ImGui::Begin("Object Tools", &m_pWindowData->ShowObjectMenu, NULL);

	ImGui::SeparatorText("Find Object");

	if (Button("Open Directory"))
	{
		IGFD::FileDialogConfig config;
		config.path = R"(C:\Users\CMAP\Documents\github\Dx11_PersonalProj_BL2\Client\Bin\Binary_Models)";
		//config.path = R"(C:\Users\CMAP\Documents\Dx11_Personal_Projects\3d\Borderlands2 Exports\Frost_StaticMesh)";
		IFILEDIALOG->OpenDialog("BINDialog", "Select BIN Files", nullptr, config);
	}
	if (IFILEDIALOG->Display("BINDialog"))
	{
		if (IFILEDIALOG->IsOk())
		{
			path FDCurPath = IFILEDIALOG->GetCurrentPath();
			if (FAILED(Open_FileDirectory(FDCurPath)))
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
		mDesc.strModelTag = m_ModelNames[item_selected_idx];
		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Monster"),
			ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_MapObject"), &mDesc)))
			return E_FAIL;
	}


	ImGui::Checkbox("Enable Gizmo", &m_isGizmoEnable);
	ImGui::Checkbox("Snap to Terrain?", &m_isObjectSnapToTerrain);

	ImGui::SetNextItemWidth(80);
	ImGui::InputFloat("Snap Offset", &m_fSnapOffset, 0.1f, 1.f, "%.1f");

	ImGui::End();

	Guizmo_Tool();
	

	return S_OK;
}

HRESULT CObjectTool::Guizmo_Tool()
{
	if (m_isGizmoEnable && m_pSelectedObj != nullptr)
	{
		// ImGui ������ ���ο��� ȣ��
		ImGuizmo::BeginFrame(); // �� �� ���� ȣ��

		ImGuizmo::SetOrthographic(false); // ���� ��� ���� (false = Perspective)
		ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList()); // �⺻ ImGui drawlist ���

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGuizmo::SetRect(viewport->Pos.x, viewport->Pos.y, viewport->Size.x, viewport->Size.y);

		//auto selectedObject = m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Monster"), m_iSelectObjectIndex);

		auto pObjTransformCom = m_pSelectedObj->Get_Transform();

		_float4x4 matView = *m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW);
		_float4x4 matProj = *m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ);

		/* ���� ������ ����, ȸ���� 15�� ����*/
		_float snapoffset[3] = {};
		if (KEY_PRESSING(DIK_LSHIFT))
		{
			if (m_eOperation == ImGuizmo::ROTATE)
				snapoffset[0] = 15.f;
			else
				snapoffset[0] = snapoffset[1] = snapoffset[2] = 1.f;
		}

		/* ���� �����ϴ� �κ� */
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


HRESULT CObjectTool::Open_FileDirectory(path& CurPath)
{
	if (!exists(CurPath))
	{
		MSG_BOX("��� �߸� �����⵵ ���� ������");
		return E_FAIL;
	}

	_matrix PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	_matrix PreTransMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	for (const auto& entry : directory_iterator(CurPath))
	{
		if (entry.is_regular_file() && entry.path().extension() == L".bin")
		{
			string filePath = entry.path().string();
			_wstring stemName = entry.path().stem().wstring();
			_wstring prototypeTag = L"Prototype_Component_Model_" + stemName;
			if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::MAPTOOL), prototypeTag,
				CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM,
					filePath.c_str(), PreTransMatrix))))
			{
				std::string msg = "���� ���� ����:\n��� ���: ";
				msg += filePath;

				// 2) MessageBoxA�� ���
				MessageBoxA(nullptr, msg.c_str(), "����", MB_OK | MB_ICONERROR);
				continue;
			}

			string strRelativePath = filesystem::relative(entry.path(), filesystem::current_path()).string();
			//m_DataPaths[entry.path().stem().string()] = strRelativePath; // map���� ���� ��ġ ����η� ��ȯ�ؼ� ����? ������
			m_ModelNames.push_back(stemName);
		}
	}

	return S_OK;
}

HRESULT CObjectTool::Ready_Prototype()
{
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
