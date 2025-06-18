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
	// fRayDist는 레이의 길이가 아니라 충돌했을 시 충돌체와의 광선 시작지점까지의 가장 짧은 거리를 반환해주는 반환값용임!!
	virtual _bool Raycast(_fvector vRayOrigin, _fvector vRayDir, _float& fRayDist) = 0;
	// 그럼 ID반환은 필요업나요? 음,, 
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