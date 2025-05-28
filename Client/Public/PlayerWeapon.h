#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CPlayerWeapon abstract : public CGameObject
{
public:
	typedef struct tagWeaponDesc : public CGameObject::DESC
	{

	}DESC;

protected:
	CPlayerWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerWeapon(const CPlayerWeapon& Prototype);
	virtual ~CPlayerWeapon() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	CShader*	m_pShaderCom = { nullptr };
	CModel*		m_pModelCom = { nullptr };

	CModel*		m_pPlayerModel = { nullptr };
	string		m_szPlayerCameraBoneName;

protected:
	HRESULT Ready_Components(void* pArg);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

NS_END