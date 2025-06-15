#pragma once

#include "Base.h"

NS_BEGIN(Engine)

/* 그냥 은성햄 코드 그대로 갈까요.. */
class CCollider_Manager : public CBase
{
private:
	CCollider_Manager();
	virtual ~CCollider_Manager() = default;

public:
	HRESULT Initialize(_uint iNumGroups);
	HRESULT Add_Collider(_uint iGroupID, class CCollider* pCollider);
	void Delete_Collider(const class CGameObject* pOwner);

private:
	_uint						m_iNumGroups = {};
	list<class CCollider*>*		m_pColliders = { nullptr };

public:
	void Intersect(class CCollider* pDst, class CCollider* pSrc);
	void Intersect_Group(_uint iSrcGroupID, _uint iDstGroupID);
	class CCollider* Raycast(_fvector vRayOrigin, _fvector vRayDir, _float fRayLength, _uint iColliderGroupID, _float& fRayDist);

public:
	static CCollider_Manager* Create(_uint iNumGroups);
	virtual void Free() override;

};

NS_END