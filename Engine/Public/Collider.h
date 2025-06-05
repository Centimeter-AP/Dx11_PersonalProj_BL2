#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{	
private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(COLLIDER eType);
	virtual HRESULT Initialize(void* pArg) override;
	void Update(_fmatrix WorldMatrix);

	_bool Intersect(CCollider* pTargetCollider);
	_bool Raycast(_fvector vRayOrigin, _fvector vRayDir, const _float fRayDist);
	class CGameObject* Get_Owner() { return m_pOwner; }
	_uint Get_ColliderID() { return m_iColliderID; }

private:
	COLLIDER			m_eType = { COLLIDER::END };
	class CBounding*	m_pBounding = { nullptr };

	class CGameObject*	m_pOwner = { nullptr };
	_uint				m_iColliderID = {};

	_bool				m_isColl = { false };

#ifdef _DEBUG
private:
	PrimitiveBatch<VertexPositionColor>* m_pBatch = { nullptr };
	BasicEffect*						 m_pEffect = { nullptr };
	ID3D11InputLayout*					 m_pInputLayout = { nullptr };
	_vector								 m_vRenderColor = {XMVectorSet(0.f, 1.f, 0.f, 1.f)};

public:
	void Set_ColliderColor(_float4 vColor) { m_vRenderColor = XMVectorSet(vColor.x, vColor.y, vColor.z, vColor.w); }
	HRESULT Render();
#endif

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDER eType);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

NS_END