#include "VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& Prototype)
	: CComponent( Prototype )
	, m_pVB { Prototype.m_pVB }
	, m_pIB { Prototype.m_pIB }
	, m_iNumVertices { Prototype.m_iNumVertices }
	, m_iVertexStride { Prototype.m_iVertexStride }
	, m_iNumIndices { Prototype.m_iNumIndices }
	, m_iIndexStride { Prototype.m_iIndexStride }
	, m_pVertexPositions { Prototype.m_pVertexPositions }
	, m_iNumVertexBuffers { Prototype.m_iNumVertexBuffers }
	, m_eIndexFormat{ Prototype.m_eIndexFormat }
	, m_ePrimitiveTopology{ Prototype.m_ePrimitiveTopology }
	, m_isCloned{ true }
	, m_pIndices{ Prototype.m_pIndices }
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
	return S_OK;
}


HRESULT CVIBuffer::Bind_Buffers()
{
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB, 
		/*m_pVB1,
		m_pVB2,*/
	};

	_uint		iVertexStrides[] = {
		m_iVertexStride, 
	};

	_uint		iOffsets[] = {
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);	
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	return S_OK;
}

HRESULT CVIBuffer::Render()
{

	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}

void CVIBuffer::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pVertexPositions);
	if (!m_isCloned) {
		Safe_Delete_Array(m_pIndices);
	}
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
