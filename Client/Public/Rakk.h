#pragma once

#include "Client_Defines.h"
#include "Monster.h"


NS_BEGIN(Client)

class CRakk final : public CMonster
{
public:
	typedef struct tagMonsterDesc : public CGameObject::DESC
	{

	}DESC;

private:
	CRakk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRakk(const CRakk& Prototype);
	virtual ~CRakk() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader*			m_pShaderCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };

private:
	HRESULT Ready_Components(void* pArg);

public:
	static CRakk* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END