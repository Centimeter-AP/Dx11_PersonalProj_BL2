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
	//상태 진입 시 한 번 호출
	virtual void Enter(_float fTimeDelta) PURE;
	//해당 상태일 때 계속 호출
	virtual void Execute(_float fTimeDelta) PURE;
	//상태 퇴장 시 한 번 호출
	virtual void Exit() PURE;
	virtual void Free() override { __super::Free(); }

protected:
	CPlayer*			m_pOwner;
	_float				m_fElapsedTime = {};
};

NS_END;