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
	const _uint Get_Damage() { return m_iDamage; }
	const _bool Is_FirstHit() { return m_isFirstHit; }
	void Set_FirstHit_False() { m_isFirstHit = false; }

protected: // 재공격가능인지는 몬스터만 제어할 수 있게 막읍시다 
	void Set_FirstHit_True() { m_isFirstHit = true; }

protected:
	CShader*			m_pShaderCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };
	CGravity*			m_pGravityCom = { nullptr };
	CNavigation*		m_pNavigationCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };
	CCollider*			m_pColliderHeadCom = { nullptr }; // at least

protected:
	_int				m_iLevel = {};
	_int				m_iHP = {};
	_int				m_iMaxHP = {};
	_uint				m_iDamage = {};
	_int				m_iDefense = {};
	_float				m_fSpeed = {};
	_float				m_fDeathDelay = {};
	const _float		m_fForceDeathDelay = { 0.f };
	_float				m_fBase_HP = {};
	_float				m_fBase_ATK = {};
	_float				m_fGrowthRate = { 1.13f }; // 13% 성장

	CGameObject*		m_pTarget = { nullptr };
	_float				m_fAttackableDistance = {};
	_float				m_fDetectiveDistance = {};
	_bool				m_isFirstHit = { true }; // 공격 상태 마다 1회씩만 타격 되도록 체크하는 변수

	CCollider*			m_LastCollidedCollider = { nullptr };
	_uint				m_iHeadBoneIdx = {};

	_int				m_iNavIndex = {};


protected:
	HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();
	virtual void Initialize_BasicStatus(_int iLevel);

	EVENT Death_Delay(_float fTimeDelta);

	void Ride_Terrain(); //임시지형타기..

	virtual void On_Collision(_uint iMyColID, _uint iHitColID, CCollider* pHitCol);

public:
	virtual void OnHit_Phaselock() {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

NS_END