#include "Navigation.h"

#include "Cell.h"
#include "GameInstance.h"

_float4x4		CNavigation::m_WorldMatrix = {};

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent { pDevice, pContext }		
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
}

CNavigation::CNavigation(const CNavigation& Prototype)
	: CComponent ( Prototype )
	, m_Cells { Prototype.m_Cells }
	, m_iIndex { Prototype.m_iIndex } 	
#ifdef _DEBUG
	, m_pShader { Prototype.m_pShader }
#endif
{	 

	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);

#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif

}


HRESULT CNavigation::Initialize_Prototype(const _tchar* pNavigationDataFile)
{
	ifstream ifs(pNavigationDataFile, ios::binary);

	if (!ifs.is_open())
		return E_FAIL;

	while (true)
	{
		_float3 vCellPoint[3] = {};
		CCell::ATTRIBUTE eAtt = {};

		if (!ifs.read(reinterpret_cast<char*>(&vCellPoint), sizeof(_float3) * 3))
			break;
		if (!ifs.read(reinterpret_cast<char*>(&eAtt), sizeof(CCell::ATTRIBUTE)))
			break;

		CCell* pCell = CCell::Create(m_pDevice, m_pContext, vCellPoint, m_Cells.size(), eAtt);
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	if (FAILED(SetUp_Neighbors()))
		return E_FAIL;


#ifdef _DEBUG
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

#endif

	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	NAVIGATION_DESC* pDesc = static_cast<NAVIGATION_DESC*>(pArg);

	m_iIndex = pDesc->iIndex;

	return S_OK;
}

void CNavigation::Update(_fmatrix WorldMatrix)
{
	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
}

_bool CNavigation::isMove(_fvector vWorldPos, _bool isGrounded)
{
	// 로컬로 옮겨줌, 기준 월드 행렬은 터레인인거로
	_vector		vLocalPos = XMVector3TransformCoord(vWorldPos, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

	_int		iNeighborIndex = { -1 };

	if (true == m_Cells[m_iIndex]->isIn(vLocalPos, &iNeighborIndex))
	{
		if (m_Cells[m_iIndex]->Get_Attribute() == CCell::ATT_JUMPONLY && isGrounded)
			return false; // 점프 안 하고 있는데 점프 전용 셀 들어가려 하면 거절

		return true; 
	}

	else
	{
		if(-1 == iNeighborIndex)
		/* 이웃이 없다면 */
			return false;

		else
		{
			while (true) // 진짜 셀 안에 있는 거 확인 될 때 까지 안 가고 버티겠다
			{
				_int curNeighborIndex = iNeighborIndex;
				if (true == m_Cells[iNeighborIndex]->isIn(vLocalPos, &iNeighborIndex)) // 존나검사해
				{
					if (m_Cells[curNeighborIndex]->Get_Attribute() == CCell::ATT_JUMPONLY && isGrounded)
						return false;

					break;
				}

				if (-1 == iNeighborIndex) // 없으면 걍 가지마 
					return false;
			}

			m_iIndex = iNeighborIndex; // 이웃잇으면 true반환하면서 내가 위치한 셀의 인덱스 바까줌 .

			/* 이웃이 있다면 */
			return true; 
		}
	}	
}

_vector CNavigation::SetUp_Height(_fvector vWorldPos, _float fOffset)
{
	_vector		vLocalPos = XMVector3TransformCoord(vWorldPos, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

	vLocalPos = XMVectorSetY(vLocalPos, m_Cells[m_iIndex]->Compute_Height(vLocalPos) + fOffset);

	return XMVector3TransformCoord(vLocalPos, XMLoadFloat4x4(&m_WorldMatrix));
}

_vector CNavigation::Get_CurCenterPoint()
{
	if (!m_Cells.empty() && m_iIndex != -1)
		return m_Cells[m_iIndex]->Get_CenterPos();
	else
		return XMVectorSet(0.f, 0.f, 0.f, 0.f);
}

#ifdef _DEBUG

_bool CNavigation::bNavigationDraw = false;

HRESULT CNavigation::Render()
{	
	if (bNavigationDraw)
	{
		m_pShader->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW));
		m_pShader->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ));

		_float4		vColor = {};

		if (-1 == m_iIndex)
		{
			m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);


			for (auto& pCell : m_Cells)
			{
				if (pCell->Get_Attribute() == CCell::ATT_NORMAL)
					vColor = _float4(0.f, 1.f, 0.f, 1.f);
				else if (pCell->Get_Attribute() == CCell::ATT_JUMPONLY)
					vColor = _float4(0.f, 0.f, 1.f, 1.f);

				m_pShader->Bind_RawValue("g_vColor", &vColor, sizeof(_float4));

				m_pShader->Begin(0);
				pCell->Render();

			}
		}
		else
		{
			_float4x4		WorldMatrix = m_WorldMatrix;
			WorldMatrix.m[3][1] += 0.1f;

			m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix);

			vColor = _float4(1.f, 0.f, 0.f, 1.f);

			m_pShader->Bind_RawValue("g_vColor", &vColor, sizeof(_float4));

			m_pShader->Begin(0);

			m_Cells[m_iIndex]->Render();
		}
	}

	return S_OK;
}
#endif


HRESULT CNavigation::SetUp_Neighbors()
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

	return S_OK;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFile)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pNavigationDataFile)))
	{
		MSG_BOX("Failed to Created : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif	
}
