#pragma once

#include "Base.h"

/* 길을 구성하는 삼각형 하나. */

NS_BEGIN(Engine)

class ENGINE_DLL CCell final : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum ATTRIBUTE { ATT_NORMAL, ATT_JUMPONLY, ATT_END };

private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCell() = default;

public:
	_vector Get_Point(POINT ePoint) {
		return XMLoadFloat3(&m_vPoints[ePoint]);
	}
	ATTRIBUTE Get_Attribute() const {
		return m_eAttribute;
	}
	ATTRIBUTE* Get_Attribute_Ptr() {
		return &m_eAttribute;
	}

public:
	void Set_Neighbor(LINE eLine, CCell* pCell) {
		m_iNeighborIndices[eLine] = pCell->m_iIndex;
	}
	void Set_Attribute(ATTRIBUTE eAttribute) {
		m_eAttribute = eAttribute;
	}
	const _int Get_MyIndex() const { return m_iIndex; }

public:
	HRESULT Initialize(const _float3* pPoints, _int iIndex, ATTRIBUTE eAttribute);
	_bool isIn(_fvector vLocalPos, _int* pNeighborIndex);
	_bool Compare(_fvector vSour, _fvector vDest);
	_float Compute_Height(_fvector vLocalPos);
	_vector Get_CenterPos();

#ifdef _DEBUG
public:
	HRESULT Render();

#endif

private:	
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	_float3			m_vPoints[POINT_END] = {};
	_float3			m_vNormals[LINE_END] = {};
	_int			m_iNeighborIndices[LINE_END] = { -1, -1, -1 };
	_int			m_iIndex = {};
	ATTRIBUTE		m_eAttribute = {};

#ifdef _DEBUG
	class CVIBuffer_Cell* m_pVIBuffer = { nullptr };
#endif

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex, ATTRIBUTE eAttribute = ATT_NORMAL);
	virtual void Free() override;

};

NS_END