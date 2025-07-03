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
	
	// 컨테이너-파트 쓸건지 게임오브젝트 하나로 관리할 건지 생각 해보기
	map<const _wstring, CGameObject*>			m_PartObjects;  
	CGameObject*								m_pParentObject = { nullptr };
	const _float4x4*							m_pParentMatrix = { nullptr };
	_float4x4									m_CombinedWorldMatrix{};
	// 파트 오브젝트 부모가 관리할 때: 부모 포인터가 있는 경우는 파츠로 판단 예외처리?

protected:
	_wstring			m_strVIBufferTag;
	//m_bDead는 신이야
	_bool				m_bDead = { false };
	_uint				m_iLevelID = UINT_MAX;
	_bool				m_isActive = { true };

protected:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);

	// 내부적으로 생성해서 너어줍니다.
	HRESULT Add_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPartObjKey, const _wstring& strPrototypeTag, void* pArg);
	// 생성 된 걸 갖다 넣습니다.
	HRESULT Add_PartObject(const _wstring& strPartObjKey, CGameObject* pPartObject);


	// Replace들은 오브젝트 삭제를 안해주니까 누수 주의할 것!!!!!!!!!!!!!!! 꼭 항상 알아서 지워주기 바람 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	// 내부적으로 생성해서 넣고 반환값으로 이미 있었던 놈을 뽑아 줍니다.
	CGameObject* Replace_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPartObjKey, const _wstring& strPrototypeTag, void* pArg);
	// 생성 된 걸 갖다 넣고 반환값으로 이미 있었던 놈을 뽑아 줍니다.
	CGameObject* Replace_PartObject(const _wstring& strPartObjKey, CGameObject* pReplaceObject);

public:
	CGameObject* Find_PartObject(const _wstring& strPartObjKey);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END


