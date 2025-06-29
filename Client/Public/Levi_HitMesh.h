#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CLevi_HitMesh final : public CGameObject
{

public:
	typedef struct tagLeviHitMeshDesc : public CGameObject::DESC
	{
		string strSocketMatrixName;

	}DESC;

private:
	CLevi_HitMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLevi_HitMesh(const CLevi_HitMesh& Prototype);
	virtual ~CLevi_HitMesh() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();
	virtual void Initialize_BasicStatus(_int iLevel);

	virtual void On_Collision(_uint iMyColID, _uint iHitColID, CCollider* pHitCol);

private:
	CShader*			m_pShaderCom = { nullptr };
	CModel*				m_pModelCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };
private:
	string				strSocketBoneName;
	const _float4x4*	m_pSocketMatrix = { nullptr };
	_float4x4			m_CombinedWorldMatrix = {};

	_int				m_iHP = {};
	_int				m_iMaxHP = {};

public:
	static CLevi_HitMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

NS_END