#include "VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype)
    : CVIBuffer( Prototype )
	, m_iNumVerticesX { Prototype.m_iNumVerticesX }
	, m_iNumVerticesZ { Prototype.m_iNumVerticesZ }
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

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _wstring& BinaryFilePath, _bool isParsing)
{
	ifstream ifs(BinaryFilePath, ios::binary);

	if (!ifs.is_open())
		return E_FAIL;

	ifs.read(reinterpret_cast<char*>(&m_iNumVerticesX), sizeof(_uint));
	ifs.read(reinterpret_cast<char*>(&m_iNumVerticesZ), sizeof(_uint));


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



	ifs.read(reinterpret_cast<char*>(m_pVertexPositions), sizeof(_float3) * m_iNumVertices);


	for (size_t i = 0; i < m_iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX; j++)
		{
			size_t		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = m_pVertexPositions[iIndex];
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}

	VBInitialData.pSysMem = pVertices;



	D3D11_BUFFER_DESC			IBBufferDesc{};
	IBBufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	IBBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IBBufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE*/0;
	IBBufferDesc.StructureByteStride = m_iIndexStride;
	IBBufferDesc.MiscFlags = 0;

	m_pIndices = new _uint[m_iNumIndices];
	ZeroMemory(m_pIndices, sizeof(_uint) * m_iNumIndices);

	ifs.read(reinterpret_cast<char*>(m_pIndices), sizeof(_uint) * m_iNumIndices);
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

			_vector		vSourDir, vDestDir, vNormal;

			m_pIndices[iNumIndices++] = iIndices[0];
			m_pIndices[iNumIndices++] = iIndices[1];
			m_pIndices[iNumIndices++] = iIndices[2];

			vSourDir = XMLoadFloat3(&m_pVertexPositions[iIndices[1]]) - XMLoadFloat3(&m_pVertexPositions[iIndices[0]]);
			vDestDir = XMLoadFloat3(&m_pVertexPositions[iIndices[2]]) - XMLoadFloat3(&m_pVertexPositions[iIndices[1]]);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal
				, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[1]].vNormal
				, XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal
				, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);

			m_pIndices[iNumIndices++] = iIndices[0];
			m_pIndices[iNumIndices++] = iIndices[2];
			m_pIndices[iNumIndices++] = iIndices[3];

			vSourDir = XMLoadFloat3(&m_pVertexPositions[iIndices[2]]) - XMLoadFloat3(&m_pVertexPositions[iIndices[0]]);
			vDestDir = XMLoadFloat3(&m_pVertexPositions[iIndices[3]]) - XMLoadFloat3(&m_pVertexPositions[iIndices[2]]);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal
				, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal
				, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[3]].vNormal
				, XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal);
		}
	}

	D3D11_SUBRESOURCE_DATA		IBInitialData{};
	IBInitialData.pSysMem = m_pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&VBBufferDesc, &VBInitialData, &m_pVB)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&IBBufferDesc, &IBInitialData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	ifs.close();
	
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
	if (FAILED(m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource)))
		return E_FAIL;

	VTXNORTEX* pVertices = static_cast<VTXNORTEX*>(SubResource.pData);

	// 1. 정점 위치 업데이트
	//for (_uint i = 0; i < m_iNumVertices; ++i)
	//{
	//	pVertices[i].vPosition = m_pVertexPositions[i];
	//}

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint index = i * m_iNumVerticesX + j;
			pVertices[index].vPosition = m_pVertexPositions[index];
			pVertices[index].vTexcoord = _float2(
				j / (_float)(m_iNumVerticesX - 1),
				i / (_float)(m_iNumVerticesZ - 1)
			);
		}
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

	if (FAILED(Update_VertexBuffer_PositionAnd_Normal()))
	{
		MSG_BOX("난진짜개멍청이다");
		return;
	}
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CVIBuffer_Terrain::Save_Terrain(const _wstring& filePath)
{
	ofstream ofs(filePath, ios::binary);
	if (!ofs.is_open())
	{
		MSG_BOX("파일 저장 실패");
		return E_FAIL;
	}

	ofs.write(reinterpret_cast<const char*>(&m_iNumVerticesX), sizeof(_uint));
	ofs.write(reinterpret_cast<const char*>(&m_iNumVerticesZ), sizeof(_uint));
	ofs.write(reinterpret_cast<const char*>(m_pVertexPositions), sizeof(_float3) * m_iNumVertices);
	ofs.write(reinterpret_cast<const char*>(m_pIndices), sizeof(_uint) * m_iNumIndices);

	ofs.close();
	return S_OK;
}


_float CVIBuffer_Terrain::Get_Height(_float x, _float z)
{
	_int iX = static_cast<_int>(x);
	_int iZ = static_cast<_int>(z);

	if (iX < 0 || iX >= m_iNumVerticesX - 1 || iZ < 0 || iZ >= m_iNumVerticesZ - 1)
		return 0.f;

	// 정점 4개
	_int iIndex = iZ * m_iNumVerticesX + iX;

	_float3 v0 = m_pVertexPositions[iIndex];                       // (iX, iZ)
	_float3 v1 = m_pVertexPositions[iIndex + 1];                   // (iX+1, iZ)
	_float3 v2 = m_pVertexPositions[iIndex + m_iNumVerticesX];     // (iX, iZ+1)
	_float3 v3 = m_pVertexPositions[iIndex + m_iNumVerticesX + 1]; // (iX+1, iZ+1)

	_float fU = x - iX;
	_float fV = z - iZ;

	_float y;
	if (fU + fV <= 1.0f)
	{
		// 삼각형 v0, v1, v2
		y = v0.y + (v1.y - v0.y) * fU + (v2.y - v0.y) * fV;
	}
	else
	{
		// 삼각형 v3, v2, v1
		y = v3.y + (v2.y - v3.y) * (1.0f - fU) + (v1.y - v3.y) * (1.0f - fV);
	}

	return y;
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

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& BinaryFilePath, _bool isParsing)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(BinaryFilePath, isParsing)))
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
	if (!m_isCloned){
		Safe_Delete_Array(m_pIndices);
	}

}
