#include "NavigationTool.h"
#include "Terrain.h"
#include "VIBuffer_Terrain.h"
#include "Cell.h"
#include "Layer.h"

#include "GameInstance.h"


CNavigationTool::CNavigationTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImGuiTool{ pDevice, pContext }
{

}

CNavigationTool::CNavigationTool(const CNavigationTool& Prototype)
	: CImGuiTool(Prototype)
{
}

HRESULT CNavigationTool::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNavigationTool::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CNavigationTool::Priority_Update(_float fTimeDelta)
{

}

EVENT CNavigationTool::Update(_float fTimeDelta)
{

	return EVN_NONE;
}

void CNavigationTool::Late_Update(_float fTimeDelta)
{
}

HRESULT CNavigationTool::Render()
{
	if (m_pWindowData->ShowTerrainMenu)
	{
		if (FAILED(Render_NavimeshTool()))
			return E_FAIL;
		for (auto& pCell : m_Cells)
		{
			pCell->Render();
		}
	}

	return S_OK;
}

HRESULT CNavigationTool::Render_NavimeshTool()
{
	ImGui::Begin("NaviMesh Tools", &m_pWindowData->ShowNavigationMenu, NULL);

	ImGui::SeparatorText("Navigation Cells");

	Text("!!Check your other picking variables!!");
	Text("Should be false before making navigation cells\n");
	Text("Before Starting, Load all Terrain and Objects");

	Checkbox("Start Picking", &m_bEnablePicking);
	if (m_bEnablePicking && m_pCurTerrain == nullptr && m_pObjectList == nullptr)
	{
		m_pCurTerrain = dynamic_cast<CTerrain*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Terrain"), 0));
		if (nullptr == m_pCurTerrain)
		{
			m_bEnablePicking = false;
			MSG_BOX("땅이 없는데 어떻게 찍지용");
			return E_FAIL;
		}

		m_pCurTerrainBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pCurTerrain->Get_Component(TEXT("Com_VIBuffer")));
		if (nullptr == m_pCurTerrainBuffer)
		{
			m_bEnablePicking = false;
			m_pCurTerrain = nullptr;
			MSG_BOX("터레인은 있는데 버퍼를 못가져왔다고?");
			return E_FAIL;
		}

		CLayer* ObjectLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_MapObject"));
		if (nullptr == ObjectLayer)
		{
			m_bEnablePicking = false;
			m_pCurTerrain = nullptr;
			MSG_BOX("님아 오브젝트도 불러주세용");
			return E_FAIL;

		}
		m_pObjectList = ObjectLayer->Get_LayerObjectListsPtr();
		if (nullptr == m_pObjectList)
		{
			m_bEnablePicking = false;
			m_pCurTerrain = nullptr;
			MSG_BOX("레이어가 잇는데 리스트를 못가져왔다고??");
			return E_FAIL;
		}
	}

	if (ImGui::RadioButton("Pick Meshes", m_isTerrainPicking == false)) {
		m_isTerrainPicking = false;
	}
	if (ImGui::RadioButton("Pick Terrain", m_isTerrainPicking)) {
		m_isTerrainPicking = true;
	}

	if (m_isTerrainPicking)
	{
		Pick_Terrain();
	}
	else
	{
		Pick_Objects();
	}

	End();
	return S_OK;
}

HRESULT CNavigationTool::Pick_Terrain()
{
	if (m_bEnablePicking)
	{
		// 마우스 좌클릭 상태 확인
		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureMouse)
		{
			if (MOUSE_DOWN(DIM::LBUTTON))
			{
				// Terrain 찾기
				if (nullptr == m_pCurTerrain)
					return E_FAIL;

				// 버텍스와 인덱스 정보 가져오기
				if (nullptr == m_pCurTerrainBuffer)
					return E_FAIL;

				const _float3* pVertices = m_pCurTerrainBuffer->Get_VertexPositions();
				const _uint* pIndices = m_pCurTerrainBuffer->Get_Indices();
				_uint iNumIndices = m_pCurTerrainBuffer->Get_NumIndices();

				// 월드 행렬 가져오기
				_matrix WorldMatrix = m_pCurTerrain->Get_Transform()->Get_WorldMatrix();

				_float3 fPick = {};

				// 피킹된 위치 저장
				if (m_pGameInstance->Pick_Mesh(WorldMatrix, pVertices, pIndices, iNumIndices, fPick))
				{
					m_fPickedPos[m_iCurIndex] = { fPick.x, fPick.y + 0.2f, fPick.z };
					m_iCurIndex++;
					if (m_iCurIndex > 2)
					{
						Make_Cell();
					}
				}
			}
		}
	}
	return S_OK;
}

HRESULT CNavigationTool::Pick_Objects()
{
	if (m_bEnablePicking)
	{
		for (auto& pObject : *m_pObjectList)
		{

		}
	}

	return S_OK;
}

HRESULT CNavigationTool::Make_Cell()
{
	CCell* pCell = CCell::Create(m_pDevice, m_pContext, m_fPickedPos, m_Cells.size());
	if (nullptr == pCell)
		return E_FAIL;

	m_Cells.push_back(pCell);

	m_iCurIndex = 0;
	for (auto& vPos : m_fPickedPos)
		vPos = { 0.f, 0.f, 0.f };

	return S_OK;
}

HRESULT CNavigationTool::Save_Navigation()
{
	return S_OK;
}

HRESULT CNavigationTool::Load_Navigation()
{
	return S_OK;
}

CNavigationTool* CNavigationTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
	CNavigationTool* pInstance = new CNavigationTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CNavigationTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNavigationTool::Clone(void* pArg)
{
	CNavigationTool* pInstance = new CNavigationTool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNavigationTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CNavigationTool::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
}
