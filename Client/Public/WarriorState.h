//#pragma once
//
//#include "Rakk.h"
//#include "GameInstance.h"
//
//NS_BEGIN(Client)
//
//class CRakkState abstract : public CBase
//{
//public:
//	CRakkState(class CRakk* pOwner)
//		: m_pOwner(pOwner),
//		m_pGameInstance(CGameInstance::Get_Instance())
//	{
//		Safe_AddRef(m_pGameInstance);
//	}
//	virtual ~CRakkState() = default;
//
//public:
//	virtual void Enter() PURE;
//	virtual void Execute(_float fTimeDelta) PURE;
//	virtual void Exit() PURE;
//	virtual void Free() override { __super::Free(); Safe_Release(m_pGameInstance); }
//
//protected:
//	CRakk*			m_pOwner;
//	CGameObject*	m_pTarget = { nullptr };
//
//	_float			m_fElapsedTime = {};
//	CGameInstance*	m_pGameInstance = { nullptr };
//};
//
//class CRakkState_Idle final : public CRakkState
//{
//public:
//	CRakkState_Idle(class CRakk* pOwner)
//		: CRakkState(pOwner) {}
//	virtual ~CRakkState_Idle() = default;
//
//public:
//	virtual void Enter() override
//	{
//
//	}
//	virtual void Execute(_float fTimeDelta) override
//	{
//
//	}
//	virtual void Exit() override
//	{
//
//	}
//	virtual void Free() override { __super::Free(); }
//};
