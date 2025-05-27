#pragma once

#include "Transform.h"

NS_BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct tagGameObjectDesc : public CTransform::TRANSFORM_DESC
	{
		_tchar		szName[MAX_PATH];
		_wstring	strVIBufferTag;

	}GAMEOBJECT_DESC;

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	CComponent* Get_Component(const _wstring& strComponentTag);
	CTransform* Get_Transform() { return m_pTransformCom; }
	_tchar*		Get_Name() { return m_szName; }
	const _wstring&	Get_VIBufferTag() const { return m_strVIBufferTag; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

	

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

protected:
	_tchar										m_szName[MAX_PATH] = {};
	map<const _wstring, class CComponent*>		m_Components;
	class CTransform*							m_pTransformCom = { nullptr };
	
	// �����̳�-��Ʈ ������ ���ӿ�����Ʈ �ϳ��� ������ ���� ���� �غ���
	map<const _wstring, CGameObject*>			m_PartObjects;
	CGameObject*								m_pParentObject = { nullptr };
	// ��Ʈ ������Ʈ �θ� ������ ��: �θ� �����Ͱ� �ִ� ���� ������ �Ǵ� ����ó�� 

protected:
	_wstring			m_strVIBufferTag;

protected:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END


