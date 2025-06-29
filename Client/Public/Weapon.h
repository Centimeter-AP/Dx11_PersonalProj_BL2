#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CWeapon abstract : public CGameObject
{
public:
	typedef struct tagWeaponDesc : public CGameObject::DESC
	{

	}DESC;

protected:
	CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon(const CWeapon& Prototype);
	virtual ~CWeapon() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

	class CTransform*		m_pPlayerTransform = { nullptr };
	CModel*					m_pPlayerModel = { nullptr };
	string					m_szPlayerCameraBoneName;
	const _float4x4*		m_pPlayerWorldMatrix = { nullptr };
	const _float4x4*		m_pSocketMatrix = { nullptr };

protected:
	_int					m_iDamage = {};
	_float					m_fAccuracy = {};
	_float					m_fFireRate = {};
	//_float					m_fReloadSpeed...는하지말죠?
	_int					m_iMagazineSize = {};
	_int					m_iCurMagazineLeft = {};
	// 총 탄약 수는 플레이어쪽에서 받는게 맞지 않으려나

public:
	const _int& Get_Damage() const{ return m_iDamage; }
	const _float& Get_Accuracy() const { return m_fAccuracy; }
	const _float& Get_Firerate() const { return m_fFireRate; }

protected:
	HRESULT Ready_Components(void* pArg);
	virtual void Initialize_BasicStatus() {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

NS_END