#pragma once

#include "Transform.h"

NS_BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct tagGameObjectDesc : public CTransform::TRANSFORM_DESC
	{
		_uint		iLevelID = 0;
		_wstring	szName;
		_wstring	strVIBufferTag;
		CGameObject* pParentObject = { nullptr };
		const _float4x4* pParentMatrix = { nullptr };
		_bool				bActive = { true };
	}DESC;

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	CComponent* Get_Component(const _wstring& strComponentTag);
	CTransform* Get_Transform() { return m_pTransformCom; }
	const _wstring& Get_Name() { return m_szName; }
	const _wstring&	Get_VIBufferTag() const { return m_strVIBufferTag; }
	void Set_Dead() { m_bDead = true; }
	virtual void Set_State_Dead() { m_bDead = true; }
	void Set_Active(_bool bActive) { m_isActive = bActive; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void On_Collision(_uint iMyColID, _uint iHitColID, CCollider* pHitCol) {};

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

protected:
	_wstring									m_szName = {};
	map<const _wstring, class CComponent*>		m_Components;
	class CTransform*							m_pTransformCom = { nullptr };
	
	// �����̳�-��Ʈ ������ ���ӿ�����Ʈ �ϳ��� ������ ���� ���� �غ���
	map<const _wstring, CGameObject*>			m_PartObjects;  
	CGameObject*								m_pParentObject = { nullptr };
	const _float4x4*							m_pParentMatrix = { nullptr };
	_float4x4									m_CombinedWorldMatrix{};
	// ��Ʈ ������Ʈ �θ� ������ ��: �θ� �����Ͱ� �ִ� ���� ������ �Ǵ� ����ó��?

protected:
	_wstring			m_strVIBufferTag;
	//m_bDead�� ���̾�
	_bool				m_bDead = { false };
	_uint				m_iLevelID = UINT_MAX;
	_bool				m_isActive = { true };

protected:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);

	// ���������� �����ؼ� �ʾ��ݴϴ�.
	HRESULT Add_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPartObjKey, const _wstring& strPrototypeTag, void* pArg);
	// ���� �� �� ���� �ֽ��ϴ�.
	HRESULT Add_PartObject(const _wstring& strPartObjKey, CGameObject* pPartObject);


	// Replace���� ������Ʈ ������ �����ִϱ� ���� ������ ��!!!!!!!!!!!!!!! �� �׻� �˾Ƽ� �����ֱ� �ٶ� !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	// ���������� �����ؼ� �ְ� ��ȯ������ �̹� �־��� ���� �̾� �ݴϴ�.
	CGameObject* Replace_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPartObjKey, const _wstring& strPrototypeTag, void* pArg);
	// ���� �� �� ���� �ְ� ��ȯ������ �̹� �־��� ���� �̾� �ݴϴ�.
	CGameObject* Replace_PartObject(const _wstring& strPartObjKey, CGameObject* pReplaceObject);

public:
	CGameObject* Find_PartObject(const _wstring& strPartObjKey);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END


