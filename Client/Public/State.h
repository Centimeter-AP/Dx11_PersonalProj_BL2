#pragma once

#include "Client_Defines.h"
#include "GameInstance.h"
#include "Base.h"

NS_BEGIN(Client)

class CState abstract : public CBase
{
public:
	CState(class CPlayer* pOwner)
		: m_pOwner(pOwner),
		  m_pGameInstance(CGameInstance::Get_Instance())
	{ Safe_AddRef(m_pGameInstance); }
	virtual ~CState() = default;

public:
	//���� ���� �� �� �� ȣ��
	virtual void Enter() PURE;
	//�ش� ������ �� ��� ȣ��
	virtual void Execute(_float fTimeDelta) PURE;
	//���� ���� �� �� �� ȣ��
	virtual void Exit() PURE;
	virtual void Free() override { __super::Free(); Safe_Release(m_pGameInstance); }

protected:
	CPlayer*			m_pOwner;
	_float				m_fElapsedTime = {};
	CGameInstance*		m_pGameInstance = { nullptr };
};

NS_END;