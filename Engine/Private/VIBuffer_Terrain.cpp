#include "VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype)
    : CVIBuffer( Prototype )
	, m_iNumVerticesX { Prototype.m_iNumVerticesX }
	, m_iNumVerticesZ { Prototype.m_iNumVerticesZ }
	, m_pIndices { Prototype. m_pIndices}
{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumVertexBuffers = 1;
	m_iVertexStride = sizeof(VTXNORTEX);
	m_iNumIndices = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2 * 3;
	m_iIndexStride = sizeof(_uint);
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	D3D11_BUFFER_DESC			VBBufferDesc{};
	VBBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	VBBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VBBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VBBufferDesc.StructureByteStride = m_iVertexStride;
	VBBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA		VBInitialData{};

	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			size_t		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3(static_cast<_float>(j), 0.f, static_cast<_float>(i));
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}


	for (_uint i = 0; i < m_iNumVertices; ++i)
		m_pVertexPositions[i] = pVertices[i].vPosition;

	VBInitialData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBBufferDesc, &VBInitialData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	D3D11_BUFFER_DESC			IBBufferDesc{};
	IBBufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	IBBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IBBufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE*/0;
	IBBufferDesc.StructureByteStride = m_iIndexStride;
	IBBufferDesc.MiscFlags = 0;

	m_pIndices = new _uint[m_iNumIndices];
	ZeroMemory(m_pIndices, sizeof(_uint) * m_iNumIndices);

	_uint	iNumIndices = { 0 };

	for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			m_pIndices[iNumIndices++] = iIndices[0];
			m_pIndices[iNumIndices++] = iIndices[1];
			m_pIndices[iNumIndices++] = iIndices[2];

			m_pIndices[iNumIndices++] = iIndices[0];
			m_pIndices[iNumIndices++] = iIndices[2];
			m_pIndices[iNumIndices++] = iIndices[3];
		}
	}

	D3D11_SUBRESOURCE_DATA		IBInitialData{};
	IBInitialData.pSysMem = m_pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBBufferDesc, &IBInitialData, &m_pIB)))
		return E_FAIL;


	return S_OK;

}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath)
{	
	HANDLE			hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_ulong			dwByte = {};

	BITMAPFILEHEADER			fh{};
	BITMAPINFOHEADER			ih{};

	auto bRes = ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);
	bRes = ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);

	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	_uint* pPixels = new _uint[m_iNumVertices];	
	bRes = ReadFile(hFile, pPixels, sizeof(_uint) * m_iNumVertices, &dwByte, nullptr);

	
	m_iNumVertexBuffers = 1;
	m_iVertexStride = sizeof(VTXNORTEX);
	m_iNumIndices = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2 * 3;
	m_iIndexStride = sizeof(_uint);
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	D3D11_BUFFER_DESC			VBBufferDesc{};
	VBBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	VBBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VBBufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE*/0;
	VBBufferDesc.StructureByteStride = m_iVertexStride;
	VBBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA		VBInitialData{};

	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			size_t		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3(static_cast<_float>(j), (pPixels[iIndex] & 0x000000ff) / 10.0f, static_cast<_float>(i));
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesX - 1.f));
		}
	}


	for (_uint i = 0; i < m_iNumVertices; ++i)
		m_pVertexPositions[i] = pVertices[i].vPosition;

	VBInitialData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBBufferDesc, &VBInitialData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	D3D11_BUFFER_DESC			IBBufferDesc{};
	IBBufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	IBBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IBBufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE*/0;
	IBBufferDesc.StructureByteStride = m_iIndexStride;
	IBBufferDesc.MiscFlags = 0;

	m_pIndices = new _uint[m_iNumIndices];
	ZeroMemory(m_pIndices, sizeof(_uint) * m_iNumIndices);

	_uint	iNumIndices = { 0 };

	for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			m_pIndices[iNumIndices++] = iIndices[0];
			m_pIndices[iNumIndices++] = iIndices[1];
			m_pIndices[iNumIndices++] = iIndices[2];

			m_pIndices[iNumIndices++] = iIndices[0];
			m_pIndices[iNumIndices++] = iIndices[2];
			m_pIndices[iNumIndices++] = iIndices[3];
		}
	}

	D3D11_SUBRESOURCE_DATA		IBInitialData{};
	IBInitialData.pSysMem = m_pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBBufferDesc, &IBInitialData, &m_pIB)))
		return E_FAIL;



	CloseHandle(hFile);
	Safe_Delete_Array(pPixels);

    return S_OK;
}

void CVIBuffer_Terrain::Get_Triangle(int iIndex, _float3& outA, _float3& outB, _float3& outC)
{
	// ex) 2차원 평면상 4x4 타일 기준 (한 타일 당 두 삼각형)
	int row = iIndex / ((m_iNumVerticesX - 1) * 2);
	int col = (iIndex % ((m_iNumVerticesZ - 1) * 2)) / 2;

	int v0 = row * m_iNumVerticesX + col;
	int v1 = v0 + 1;
	int v2 = v0 + m_iNumVerticesX;
	int v3 = v2 + 1;

	if (iIndex % 2 == 0) // 삼각형1: v0, v1, v2
	{
		outA = m_pVertexPositions[v0];
		outB = m_pVertexPositions[v1];
		outC = m_pVertexPositions[v2];
	}
	else // 삼각형2: v1, v3, v2
	{
		outA = m_pVertexPositions[v1];
		outB = m_pVertexPositions[v3];
		outC = m_pVertexPositions[v2];
	}
}

void CVIBuffer_Terrain::Update_VertexBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, m_pVertexPositions, sizeof(VTXNORTEX) * m_iNumVertices);
	m_pContext->Unmap(m_pVB, 0);
}

HRESULT CVIBuffer_Terrain::Update_VertexBuffer_PositionAnd_Normal()
{
	D3D11_MAPPED_SUBRESOURCE SubResource;
	if (FAILED(m_pContext->Map(m_pVB, 0, D3D11_MAP_READ_WRITE, 0, &SubResource)))
		return E_FAIL;

	VTXNORTEX* pVertices = static_cast<VTXNORTEX*>(SubResource.pData);

	// 1. 정점 위치 업데이트
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		// 위치를 업데이트 할 수 있는 로직
		// 예: pVertices[i].vPosition = new_position;
	}

	// 2. 노멀 재계산을 위한 벡터 초기화
	std::vector<XMVECTOR> vecNormals(m_iNumVertices, XMVectorZero());

	// 3. 각 삼각형에 대해 면 노멀 계산 후 각 정점 노멀에 누적
	for (_uint i = 0; i < m_iNumIndices; i += 3)
	{
		_uint i0 = m_pIndices[i];
		_uint i1 = m_pIndices[i + 1];
		_uint i2 = m_pIndices[i + 2];

		XMVECTOR v0 = XMLoadFloat3(&pVertices[i0].vPosition);
		XMVECTOR v1 = XMLoadFloat3(&pVertices[i1].vPosition);
		XMVECTOR v2 = XMLoadFloat3(&pVertices[i2].vPosition);

		XMVECTOR edge1 = v1 - v0;
		XMVECTOR edge2 = v2 - v0;

		XMVECTOR faceNormal = XMVector3Cross(edge1, edge2);
		faceNormal = XMVector3Normalize(faceNormal);

		vecNormals[i0] += faceNormal;
		vecNormals[i1] += faceNormal;
		vecNormals[i2] += faceNormal;
	}

	// 4. 계산된 노멀을 정규화하여 버퍼에 저장
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		XMVECTOR normal = XMVector3Normalize(vecNormals[i]);
		XMStoreFloat3(&pVertices[i].vNormal, normal);
	}

	m_pContext->Unmap(m_pVB, 0);

	return S_OK;
}


void CVIBuffer_Terrain::Apply_Brush(const _float3& vPickedPos, float fRadius, float fPower, float fDeltaTime)
{
	int centerX = static_cast<int>(vPickedPos.x + 0.5f);
	int centerZ = static_cast<int>(vPickedPos.z + 0.5f);

	int brushCellRadius = static_cast<int>(fRadius) + 1;

	for (int dz = -brushCellRadius; dz <= brushCellRadius; ++dz)
	{
		for (int dx = -brushCellRadius; dx <= brushCellRadius; ++dx)
		{
			int curX = centerX + dx;
			int curZ = centerZ + dz;

			if (curX < 0 || curX >= static_cast<int>(m_iNumVerticesX) ||
				curZ < 0 || curZ >= static_cast<int>(m_iNumVerticesZ))
				continue;

			int index = curZ * m_iNumVerticesX + curX;

			_float3& vtxPos = m_pVertexPositions[index];

			float dist = XMVectorGetX(
				XMVector3Length(XMLoadFloat3(&vtxPos) - XMLoadFloat3(&vPickedPos)));

			if (dist <= fRadius)
			{
				float fWeight = 1.0f - (dist / fRadius);
				vtxPos.y += fPower * fWeight * fDeltaTime;
			}
		}
	}

	Update_VertexBuffer_PositionAnd_Normal();
}




HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
    return S_OK;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(iNumVerticesX, iNumVerticesZ)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pHeightMapFilePath)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
    __super::Free();
	Safe_Delete_Array(m_pIndices);


}
