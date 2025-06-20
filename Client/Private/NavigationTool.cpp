#include "NavigationTool.h"
#include "Terrain.h"
#include "VIBuffer_Terrain.h"
#include "Cell.h"
#include "Layer.h"
#include "Shader.h"
#include "GameInstance.h"
#include <MapObject.h>


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

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	return S_OK;
}

void CNavigationTool::Priority_Update(_float fTimeDelta)
{

}

EVENT CNavigationTool::Update(_float fTimeDelta)
{
	Key_Input();
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

		if (FAILED(Render_Cells()))
			return E_FAIL;
		
	}

	return S_OK;
}

HRESULT CNavigationTool::Render_Cells()
{
	if (m_bEnablePicking)
	{
		_float4x4		WorldMatrix = m_pCurTerrain->Get_Transform()->Get_WorldMatrix4x4Ref();
		WorldMatrix.m[3][1] += 0.1f;
		if (!m_Cells.empty())
		{
			m_pShader->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW));
			m_pShader->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ));

			m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix);
			_float4 vColor = _float4(0.f, 1.f, 0.f, 1.f);

			m_pShader->Bind_RawValue("g_vColor", &vColor, sizeof(_float4));

			m_pShader->Begin(0);

			for (auto& pCell : m_Cells)
			{
				pCell->Render();
			}

			if (m_iSelectedCellIndex != -1)
			{
				//_float4x4		WorldMatrix = m_pCurTerrain->Get_Transform()->Get_WorldMatrix4x4Ref();
				WorldMatrix.m[3][1] += 0.1f;

				m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix);

				vColor = _float4(1.f, 0.f, 0.f, 1.f);

				m_pShader->Bind_RawValue("g_vColor", &vColor, sizeof(_float4));

				m_pShader->Begin(0);
				m_Cells[m_iSelectedCellIndex]->Render();
			}
		}
	}

	return S_OK;
}

HRESULT CNavigationTool::Render_NavimeshTool()
{
	ImGui::Begin("NaviMesh Tools", &m_pWindowData->ShowNavigationMenu, NULL);

	ImGui::Separator();

	Checkbox("Start Picking", &m_bEnablePicking);
	Text("!!Check your other picking variables!!");
	Text("Should be false before making navigation cells");
	Text("\nBefore Starting, Load all Terrain and Objects");

	if (m_bEnablePicking && m_pCurTerrain == nullptr && m_pObjectList == nullptr)
	{
		m_pCurTerrain = dynamic_cast<CTerrain*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_Terrain"), 0));
		if (nullptr == m_pCurTerrain)
		{
			m_bEnablePicking = false;
			MSG_BOX("땅이 없는데 어떻게 찍지용");
			End();
			return E_FAIL;
		}

		m_pCurTerrainBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pCurTerrain->Get_Component(TEXT("Com_VIBuffer")));
		if (nullptr == m_pCurTerrainBuffer)
		{
			m_bEnablePicking = false;
			m_pCurTerrain = nullptr;
			MSG_BOX("터레인은 있는데 버퍼를 못가져왔다고?");
			End();
			return E_FAIL;
		}

		CLayer* ObjectLayer = m_pGameInstance->Find_Layer(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Layer_MapObject"));
		if (nullptr == ObjectLayer)
		{
			m_bEnablePicking = false;
			m_pCurTerrain = nullptr;
			MSG_BOX("님아 오브젝트도 불러주세용");
			End();
			return E_FAIL;

		}
		m_pObjectList = ObjectLayer->Get_LayerObjectListsPtr();
		if (nullptr == m_pObjectList)
		{
			m_bEnablePicking = false;
			m_pCurTerrain = nullptr;
			MSG_BOX("레이어가 잇는데 리스트를 못가져왔다고??");
			End();
			return E_FAIL;
		}
	}
	Separator();
	Text("Shift + Q / W / E");
	if (ImGui::RadioButton("Pick Terrain", m_ePickingType == TYPE_TERRIAN)) {
		m_ePickingType = TYPE_TERRIAN;
	}
	if (ImGui::RadioButton("Pick Meshes", m_ePickingType == TYPE_MESH)) {
		m_ePickingType = TYPE_MESH;
	}
	if (ImGui::RadioButton("Pick Cell", m_ePickingType == TYPE_CELL)) {
		m_ePickingType = TYPE_CELL;
	}

	switch (m_ePickingType)
	{
	case Client::CNavigationTool::TYPE_TERRIAN:
		Pick_Terrain();
		break;
	case Client::CNavigationTool::TYPE_MESH:
		Pick_Objects();
		break;
	case Client::CNavigationTool::TYPE_CELL:
		Pick_Cells();
		break;
	default:
		break;
	}
	Separator();
	if (Button("Make Terrain buffer to NaviMesh"))
	{
		Make_Terrain_Navimesh();
	}
	//if (Button("SetUp_Neighbors"))
	//{
	//	SetUp_Neighbors();
	//}
	Separator();

	Text("Current Selected Cell Index");
	Text(to_string(m_iSelectedCellIndex).c_str());

	Separator();
	if (Button("Save Navi"))
	{
		Save_Navigation();
	}
	if (Button("Load Navi"))
	{
		Load_Navigation();
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
				const _float3* pVertices = m_pCurTerrainBuffer->Get_VertexPositions();
				const _uint* pIndices = m_pCurTerrainBuffer->Get_Indices();
				_uint iNumIndices = m_pCurTerrainBuffer->Get_NumIndices();

				// 월드 행렬 가져오기
				_matrix WorldMatrix = m_pCurTerrain->Get_Transform()->Get_WorldMatrix();

				_float3 fPick = {};
				_float fDist = {};
				// 피킹된 위치 저장
				if (m_pGameInstance->Pick_Mesh(WorldMatrix, pVertices, pIndices, iNumIndices, fPick, fDist))
				{
					fPick = Snap_PickedPos(fPick, 0.6f);
					m_fPickedPos[m_iCurCellIndex] = fPick;
					m_iCurCellIndex++;
					if (m_iCurCellIndex > 2)
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
		// 마우스 좌클릭 상태 확인
		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureMouse)
		{
			if (MOUSE_DOWN(DIM::LBUTTON))
			{
				_float3 fClosestPos = {};
				_float fClosestDist = FLT_MAX;
				CGameObject* pClosestObject = { nullptr };
				for (auto& pObject : *m_pObjectList)
				{
					CModel* pModel = static_cast<CModel*>(static_cast<CMapObject*>(pObject)->Get_Component(TEXT("Com_Model")));
					
					auto pMeshes = pModel->Get_Meshes();

					for (auto& pMesh : *pMeshes)
					{
						const _float3* pVertices = pMesh->Get_VertexPositions();
						const _uint* pIndices = pMesh->Get_Indices();
						_uint iNumIndices = pMesh->Get_NumIndices();
						_matrix WorldMatrix = pObject->Get_Transform()->Get_WorldMatrix();
						// 월드 행렬 가져오기

						_float3 fPick = {};
						_float fDist = {};
						// 피킹된 위치 저장
						if (m_pGameInstance->Pick_Mesh(WorldMatrix, pVertices, pIndices, iNumIndices, fPick, fDist))
						{
							cout << "\npicked Object : " << WStringToString(pObject->Get_VIBufferTag()) << endl;
							cout << "Distance : " << fDist << endl;
							if (fDist < fClosestDist)
							{
								fClosestDist = fDist;
								fClosestPos = fPick;
								pClosestObject = pObject;
							}
						}
					}
				}
				if (fClosestDist < 0.f)
					return S_OK;

				/*가까운놈만 걸러야*/
				fClosestPos = Snap_PickedPos(fClosestPos, 0.5f);
				m_fPickedPos[m_iCurCellIndex] = fClosestPos;
				m_iCurCellIndex++;
				if (m_iCurCellIndex > 2)
				{
					Make_Cell();
				}
			}
		}
	}

	return S_OK;
}

HRESULT CNavigationTool::Pick_Cells()
{
	if (m_bEnablePicking)
	{			
		// 마우스 좌클릭 상태 확인
		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureMouse)
		{
			if (MOUSE_DOWN(DIM::LBUTTON))
			{
				const _float3* pVertices = m_pCurTerrainBuffer->Get_VertexPositions();
				const _uint* pIndices = m_pCurTerrainBuffer->Get_Indices();
				_uint iNumIndices = m_pCurTerrainBuffer->Get_NumIndices();

				// 월드 행렬 가져오기
				_matrix WorldMatrix = m_pCurTerrain->Get_Transform()->Get_WorldMatrix();

				_float3 fPick = {};
				_float fDist = {};

				// 피킹된 위치 저장
				if (m_pGameInstance->Pick_Mesh(WorldMatrix, pVertices, pIndices, iNumIndices, fPick, fDist))
				{
					// 로컬로 옮겨줌, 기준 월드 행렬은 터레인인거로
					_vector		vLocalPos = XMVector3TransformCoord(XMLoadFloat3(&fPick), XMMatrixInverse(nullptr, m_pCurTerrain->Get_Transform()->Get_WorldMatrix()));

					_int		iNeighborIndex = { -1 };
					for (_int i = 0; i < m_Cells.size(); i++)
					{
						if (true == m_Cells[i]->isIn(XMLoadFloat3(&fPick), &iNeighborIndex))
						{      
							m_iSelectedCellIndex = i;
							break; // 셀 안에 있으면 걍 트루
						}
					}
					/* 뭐 적으려 했는데 기억 안나서 비움 */
				}
			}
		}
	}
	return S_OK;
}


void CNavigationTool::EnsureClockwise(_float3& a, _float3& b, _float3& c)
{
	// 2D 외적: (bx - ax)*(cz - az) - (cx - ax)*(bz - az)
	_float ab_x = b.x - a.x;
	_float ab_z = b.z - a.z;
	_float ac_x = c.x - a.x;
	_float ac_z = c.z - a.z;

	_float cross = ab_x * ac_z - ac_x * ab_z;

	if (cross > 0.f)
	{
		swap(b, c);
	}
}

HRESULT CNavigationTool::Make_Cell()
{
	EnsureClockwise(m_fPickedPos[0], m_fPickedPos[1], m_fPickedPos[2]);

	CCell* pCell = CCell::Create(m_pDevice, m_pContext, m_fPickedPos, m_Cells.size());
	if (nullptr == pCell)
		return E_FAIL;

	m_Cells.push_back(pCell);

	m_iCurCellIndex = 0;
	for (auto& vPos : m_fPickedPos)
		vPos = { 0.f, 0.f, 0.f };

	return S_OK;
}

HRESULT CNavigationTool::Make_Terrain_Navimesh()
{
	const _float3* pVertices = m_pCurTerrainBuffer->Get_VertexPositions();
	const _uint* pIndices = m_pCurTerrainBuffer->Get_Indices();
	_uint iNumIndices = m_pCurTerrainBuffer->Get_NumIndices();

	//for (_uint i = 0; i < iNumIndices; i += 3)
	//{
	//	m_fPickedPos[0] = pVertices[pIndices[i]];
	//	m_fPickedPos[1] = pVertices[pIndices[i + 1]];
	//	m_fPickedPos[2] = pVertices[pIndices[i + 2]];

	//	Make_Cell();
	//}

	_uint	iIdx = { 0 };
	_uint	iMul = { 5 };

	for (_uint i = iMul * 4; i < m_pCurTerrainBuffer->Get_NumVerticesZ() - 1 - iMul * 6; i += iMul)
	{
		for (_uint j = iMul * 4; j < m_pCurTerrainBuffer->Get_NumVerticesX() - 1 - iMul * 6; j += iMul)
		{
			_uint		iIndex = i * m_pCurTerrainBuffer->Get_NumVerticesX() + j;

			_uint		iIndices[4] = {
				iIndex + m_pCurTerrainBuffer->Get_NumVerticesX() * iMul,
				iIndex + m_pCurTerrainBuffer->Get_NumVerticesX() * iMul + iMul,
				iIndex + iMul,
				iIndex
			};

			m_fPickedPos[0] = pVertices[iIndices[0]];
			m_fPickedPos[1] = pVertices[iIndices[1]];
			m_fPickedPos[2] = pVertices[iIndices[2]];
			Make_Cell();

			m_fPickedPos[0] = pVertices[iIndices[0]];
			m_fPickedPos[1] = pVertices[iIndices[2]];
			m_fPickedPos[2] = pVertices[iIndices[3]];
			Make_Cell();
		}
	}


	return S_OK;
}

void CNavigationTool::SetUp_Neighbors()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell);
			}

			if (true == pDestCell->Compare(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell);
			}

			if (true == pDestCell->Compare(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell);
			}
		}
	}
	return;
}

HRESULT CNavigationTool::Save_Navigation()
{
	ofstream ofs("../Bin/DataFiles/Navigation.dat", ios::binary);
	if (!ofs.is_open())
	{
		MSG_BOX("파일 저장 실패");
		return E_FAIL;
	}
	for (auto& pCell : m_Cells)
	{
		for (_uint i = 0; i < 3; i++)
		{
			_float3 vCellPoint = {};
			XMStoreFloat3(&vCellPoint, pCell->Get_Point(static_cast<CCell::POINT>(i)));

			ofs.write(reinterpret_cast<const char*>(&vCellPoint), sizeof(_float3));
		}
	}

	return S_OK;
}

HRESULT CNavigationTool::Load_Navigation()
{
	ifstream ifs("../Bin/DataFiles/Navigation.dat", ios::binary);

	if (!ifs.is_open())
		return E_FAIL;

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
	m_Cells.clear();
	m_Cells.shrink_to_fit();

	while (true)
	{
		_float3 vCellPoint[3] = {};

		if (!ifs.read(reinterpret_cast<char*>(&vCellPoint), sizeof(_float3) * 3))
			return S_OK;

		CCell* pCell = CCell::Create(m_pDevice, m_pContext, vCellPoint, m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	return S_OK;
}

_float3 CNavigationTool::Snap_PickedPos(const _float3& vPos, _float fSnapRadius)
{
	_float closestDistSq = fSnapRadius * fSnapRadius;
	const _float3* closestPoint = nullptr;

	for (auto& pCell : m_Cells)
	{
		for (_uint i = 0; i < 3; i++)
		{
			_float3 vCellPoint = {};
			XMStoreFloat3(&vCellPoint, pCell->Get_Point(static_cast<CCell::POINT>(i)));

			_float distSq =
				(vCellPoint.x - vPos.x) * (vCellPoint.x - vPos.x) +
				(vCellPoint.y - vPos.y) * (vCellPoint.y - vPos.y) +
				(vCellPoint.z - vPos.z) * (vCellPoint.z - vPos.z);

			if (distSq <= closestDistSq)
			{
				return vCellPoint;
				//closestDistSq = distSq;
				//closestPoint = &vCellPoint;
			}
		}
	}
	return vPos;
	//return closestPoint != nullptr ? *closestPoint : vPos;

}

void CNavigationTool::Key_Input()
{
	if (m_ePickingType == TYPE_CELL && m_iSelectedCellIndex != -1)
	{
		if (KEY_DOWN(DIK_DELETE))
		{
			auto iter = m_Cells.begin() + m_iSelectedCellIndex;
			Safe_Release(*iter);
			m_Cells.erase(iter);
			m_iSelectedCellIndex = -1;
		}
	}

	if (KEY_PRESSING(DIK_LCONTROL))
	{
		if (KEY_DOWN(DIK_1))
		{
			m_ePickingType = TYPE_TERRIAN;
		}
		if (KEY_DOWN(DIK_2))
		{
			m_ePickingType = TYPE_MESH;
		}
		if (KEY_DOWN(DIK_3))
		{
			m_ePickingType = TYPE_CELL;
		}
	}
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

	Safe_Release(m_pShader);
}
