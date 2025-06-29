#pragma once

#include "Base.h"

NS_BEGIN(Engine)

/* �׳� ������ �ڵ� �״�� �����.. */
class CCollider_Manager : public CBase
{
private:
	CCollider_Manager();
	virtual ~CCollider_Manager() = default;

public:
	HRESULT Initialize(_uint iNumGroups);
	HRESULT Add_Collider(_uint iGroupID, class CCollider* pCollider);
	void Delete_Collider(const class CGameObject* pOwner);
	void Clear();

private:
	_uint						m_iNumLevels = {};
	_uint						m_iNumGroups = {};
	list<class CCollider*>*		m_pColliders = { nullptr };

public:
	void Intersect(class CCollider* pDst, class CCollider* pSrc);
	void Intersect_Group(_uint iSrcGroupID, _uint iDstGroupID, _bool bMovePenetrate = true);
	_float3 Resolve_Slide_AABB(class CCollider* pMyCol, const _float3 vDesiredMove, _uint iGroupID);
	class CCollider* Raycast(_fvector vRayOrigin, _fvector vRayDir, _float fRayLength, _uint iColliderGroupID, _float& fRayDist);

public:
	static CCollider_Manager* Create(_uint iNumGroups);
	virtual void Free() override;

};

NS_END