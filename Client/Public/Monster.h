#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
class CNavigation;
class CGravity;
NS_END

NS_BEGIN(Client)

class CMonster abstract : public CGameObject
{
public:
	typedef struct tagMonsterDesc : public CGameObject::DESC
	{
		_bool				bActive = { true };
		_int				iNavigationIndex = { -1 };

	}DESC;

protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	CGameObject* Get_Target() { return m_pTarget; }

protected:
	CShader*			m_pShaderCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };
	CGravity*			m_pGravityCom = { nullptr };
	CNavigation*		m_pNavigationCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };
	CCollider*			m_pColliderHeadCom = { nullptr };

protected:
	_int				m_iHP = {};
	_int				m_iMaxHP = {};
	_uint				m_iAttackPower = {};
	_int				m_iDefense = {};
	_float				m_fSpeed = {};
	_float				m_fDeathDelay = {};
	const _float		m_fForceDeathDelay = { 0.f };

	_bool				m_bActive = { true };

	CGameObject*		m_pTarget = { nullptr };
	_float				m_fAttackableDistance = {};
	_float				m_fDetectiveDistance = {};

	CCollider*			m_LastCollidedCollider = { nullptr };
	_uint				m_iHeadBoneIdx = {};

	_int				m_iNavIndex = {};

protected:
	HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();
	virtual void Initialize_BasicStatus(_int iLevel);

	EVENT Death_Delay(_float fTimeDelta);

	void Ride_Terrain(); //임시지형타기..

	virtual void On_Collision(_uint iColID);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

NS_END