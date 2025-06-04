#pragma once

#include "Base.h"

NS_BEGIN(Engine)

/* 그냥 은성햄 코드 그대로 갈까요.. */
class CColliderManager : public CBase
{
private:
	CColliderManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CColliderManager() = default;

public:
	HRESULT Initialize();
	HRESULT Add_Collider(/*콜라이더레이어,*/ class CCollider* pCollider);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	void Intersect(class CCollider* pDst, class CCollider* pSrc);
	void Intersect_Group(class CCollider* pDst, class CCollider* pSrc);

public:
	static CColliderManager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

NS_END