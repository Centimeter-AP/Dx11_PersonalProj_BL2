#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Client)

class CState abstract : public CBase
{
public:
	CState(class CPlayer* pOwner) : m_pOwner(pOwner) {}
	virtual ~CState() = default;

public:
	//���� ���� �� �� �� ȣ��
	virtual void Enter(_float fTimeDelta) PURE;
	//�ش� ������ �� ��� ȣ��
	virtual void Execute(_float fTimeDelta) PURE;
	//���� ���� �� �� �� ȣ��
	virtual void Exit() PURE;
	virtual void Free() override { __super::Free(); }

protected:
	CPlayer*			m_pOwner;
	_float				m_fElapsedTime = {};
};

NS_END;