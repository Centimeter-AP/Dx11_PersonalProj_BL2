#pragma once

#include "Bullet.h"

NS_BEGIN(Engine)
class CTexture;
NS_END

NS_BEGIN(Client)

class CLevi_Bullet final : public CBullet
{
public:
	typedef struct tagLeviBulletDesc : public CBullet::DESC
	{
		_bool bLaunch = { true };
		_float4x4* pSocketMatrix = { nullptr };
	}DESC;

private:
	CLevi_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLevi_Bullet(const CLevi_Bullet& Prototype);
	virtual ~CLevi_Bullet() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	void Launch_Projectile(const _float3& targetPos, _float fSpeed);

private:
	HRESULT Ready_Components(void* pArg);

	void Update_Projectile(_float fTimeDelta);

private:
	CTexture* m_pTextureCom = nullptr;
	CTexture* m_pTextureEmissiveCom = nullptr;
	_vector m_vVelocity = XMVectorZero();
	_vector m_vGravity = XMVectorSet(0.f, -65.f, 0.f, 0.f);
	_bool   m_bIsProjectile = false;

	_float	m_fLifeTime = {};
	_bool	m_bLaunched = { false };
	_float4x4* m_pSocketMatrix = { nullptr };

public:
	static CLevi_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

NS_END