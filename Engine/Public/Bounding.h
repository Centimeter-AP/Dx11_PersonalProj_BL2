#pragma once

#include "Base.h"

#ifdef _DEBUG
#include "DebugDraw.h"
#endif

NS_BEGIN(Engine)

class CBounding abstract : public CBase
{
public:
	typedef struct tagBoundingDesc
	{
		class CGameObject* pOwner = { nullptr };
		_uint				iColliderID = {};
		_uint				iColliderGroup = {};
		COLLIDER	eType;
		_float3		vCenter;
	}BOUNDING_DESC;

protected:
	CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding() = default;

public:
	COLLIDER Get_Type() const {
		return m_eType;
	}


public:
	HRESULT Initialize(const CBounding::BOUNDING_DESC* pDesc);
	virtual void Update(_fmatrix WorldMatrix) = 0;
	virtual _bool Intersect(CBounding* pTarget) = 0;
	// fRayDist�� ������ ���̰� �ƴ϶� �浹���� �� �浹ü���� ���� �������������� ���� ª�� �Ÿ��� ��ȯ���ִ� ��ȯ������!!
	virtual _bool Raycast(_fvector vRayOrigin, _fvector vRayDir, _float& fRayDist) = 0;
	// �׷� ID��ȯ�� �ʿ������? ��,, 
	_float3 Get_Penetrated() { return m_vPenetratedVector; }

#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) { return S_OK; }
#endif
	
protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	COLLIDER					m_eType = { };
	
protected:
	_float3						m_vPenetratedVector = {};

public:
	virtual void Free() override;
};

NS_END