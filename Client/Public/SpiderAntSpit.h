#pragma once

#include "Bullet.h"

NS_BEGIN(Engine)
class CTexture;
NS_END


NS_BEGIN(Client)
class CSpiderAntSpit final : public CBullet
{

public:
	typedef struct tagSpiderAntSpitDesc : public CBullet::DESC
	{

	}DESC;

private:
	CSpiderAntSpit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpiderAntSpit(const CSpiderAntSpit& Prototype);
	virtual ~CSpiderAntSpit() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components(void* pArg);

	void Launch_Projectile(const _float3& targetPos, _float fSpeed);
	void Update_Projectile(_float fTimeDelta);

private:
	CTexture* m_pTextureCom = { nullptr };
	_vector m_vVelocity = XMVectorZero();
	_vector m_vGravity = XMVectorSet(0.f, -55.f, 0.f, 0.f);
	_bool   m_bIsProjectile = false;

	_float	m_fLifeTime = {};

public:
	static CSpiderAntSpit* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

NS_END