#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
NS_END

NS_BEGIN(Client)

class CBullet abstract : public CGameObject
{
public:
	typedef struct tagBulletDesc : public CGameObject::DESC
	{
		COL_ID eCollider_ID = { COL_ID::END };
		_float3 vTargetPos = {};
		_uint	iDamage = {1};
	}DESC;

protected:
	CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet(const CBullet& Prototype);
	virtual ~CBullet()
	{
		int a = 0;

	}

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	const _uint Get_Damage() { return m_iDamage; }

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

	_float3	m_vTargetPos = {};

	_uint	m_iDamage = {};

protected:
	HRESULT Ready_Components(void* pArg);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

NS_END