#pragma once

#include "Base.h"

NS_BEGIN(Engine)

/* �׳� ������ �ڵ� �״�� �����.. */
class CColliderManager : public CBase
{
private:
	CColliderManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CColliderManager() = default;

public:
	HRESULT Initialize();
	HRESULT Add_Collider(/*�ݶ��̴����̾�,*/ class CCollider* pCollider);

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