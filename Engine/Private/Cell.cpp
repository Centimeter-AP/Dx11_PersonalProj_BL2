#include "Cell.h"
#include "GameInstance.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex, ATTRIBUTE eAttribute)
{
	m_iIndex = iIndex;

	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	_vector		vLine = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	vLine = XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]);
	m_vNormals[LINE_AB] = _float3(vLine.m128_f32[2] * -1.f, 0.f, vLine.m128_f32[0]);

	vLine = XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]);
	m_vNormals[LINE_BC] = _float3(vLine.m128_f32[2] * -1.f, 0.f, vLine.m128_f32[0]);

	vLine = XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]);
	m_vNormals[LINE_CA] = _float3(vLine.m128_f32[2] * -1.f, 0.f, vLine.m128_f32[0]);

	m_eAttribute = eAttribute;

#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, pPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif


	return S_OK;
}

_bool CCell::isIn(_fvector vLocalPos, _int* pNeighborIndex)
{
	for (size_t i = 0; i < LINE_END; i++) // 세 변에 대해서 전부 검사를 수행 
	{
		_vector		vDir = vLocalPos - XMLoadFloat3(&m_vPoints[i]);

		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMVector3Normalize(XMLoadFloat3(&m_vNormals[i])))))
		{ // 검사를 통과하지 못했을 경우 (셀 안에 있지 않을 경우)
			*pNeighborIndex = m_iNeighborIndices[i]; // 그럼 그 선분에 이웃한 인덱스의 셀로 갔음을 알려줌 (이웃도 없었을 시 -1 반환)
			return false;
		}
	}

	return true;
}

_bool CCell::Compare(_fvector vSour, _fvector vDest)
{
	/*XMVectorEqual(vSour, vDest);*/
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSour))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDest))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDest))
			return true;		
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSour))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDest))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDest))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSour))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDest))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDest))
			return true;
	}

	return false;
}

_float CCell::Compute_Height(_fvector vLocalPos)
{
	_vector		vPlane = XMPlaneFromPoints(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&m_vPoints[POINT_C]));

	return (-vPlane.m128_f32[0] * vLocalPos.m128_f32[0] - vPlane.m128_f32[2] * vLocalPos.m128_f32[2] - vPlane.m128_f32[3]) / vPlane.m128_f32[1];

	// y = (-ax - cz - d) / b
}

_vector CCell::Get_CenterPos()
{
	_vector vPointA = XMLoadFloat3(&m_vPoints[POINT_A]);
	_vector vPointB = XMLoadFloat3(&m_vPoints[POINT_B]);
	_vector vPointC = XMLoadFloat3(&m_vPoints[POINT_C]);


	return (vPointA + vPointB + vPointC) / 3.f;
}

#ifdef _DEBUG
HRESULT CCell::Render()
{
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	return S_OK;
}
#endif

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex, ATTRIBUTE eAttribute)
{
	CCell* pInstance = new CCell(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pPoints, iIndex, eAttribute)))
	{
		MSG_BOX("Failed to Created : CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CCell::Free()
{
	__super::Free();


	Safe_Release(m_pVIBuffer);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
