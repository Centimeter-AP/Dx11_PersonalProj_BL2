#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CMapObject final : public CGameObject
{
public:
	typedef struct tagMapObjectDesc : public CGameObject::DESC
	{
		//LEVEL		eLevelID = { LEVEL::END };
	}DESC;

private:
	CMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMapObject(const CMapObject& Prototype);
	virtual ~CMapObject() = default;

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
	//LEVEL				m_eLevelID = { LEVEL::END };

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CMapObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END