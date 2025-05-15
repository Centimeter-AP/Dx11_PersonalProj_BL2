#include "GameInstance.h"
#include "ObjectTool.h"
#include <Terrain.h>

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

	if (Button("Make Mushroom"))
	{
		if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_GameObject_Monster"),
			ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Monster"))))
			return E_FAIL;
	}

	Checkbox("Enable Gizmo", &m_isGizmoEnable);
	Checkbox("Snap to Terrain?", &m_isObjectSnapToTerrain);

	SetNextItemWidth(80);
	InputFloat("Snap Offset", &m_fSnapOffset, 0.1f, 1.f, "%.1f");

	ImGui::End();

	if (m_isGizmoEnable)
	{
		// ImGui 프레임 내부에서 호출
		ImGuizmo::BeginFrame(); // ← 한 번만 호출

		ImGuizmo::SetOrthographic(false); // 투영 방식 설정 (false = Perspective)
		ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList()); // 기본 ImGui drawlist 사용

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGuizmo::SetRect(viewport->Pos.x, viewport->Pos.y, viewport->Size.x, viewport->Size.y);

		auto selectedObject = m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Monster"), 0);
		auto pObjTransformCom = selectedObject->Get_Transform();

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
			selectedObject->Get_Transform()->Set_State(STATE::POSITION, fixedPos);
		}
	}

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
