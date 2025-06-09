#pragma once

#include "Skag.h"
#include "GameInstance.h"

NS_BEGIN(Client)

class CSkagState abstract : public CBase
{
public:
	CSkagState(class CSkag* pOwner)
		: m_pOwner(pOwner),
		m_pGameInstance(CGameInstance::Get_Instance())
	{
		m_pTarget = m_pOwner->m_pTarget;
		Safe_AddRef(m_pGameInstance);
	}
	virtual ~CSkagState() = default;

public:
	virtual void Enter() PURE;
	virtual void Execute(_float fTimeDelta) PURE;
	virtual void Exit() PURE;
	virtual void Free() override { __super::Free(); Safe_Release(m_pGameInstance); }

	virtual _bool Is_Target_Found(){
		auto Dist = fabs(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTarget->Get_Transform()->Get_State(STATE::POSITION))));
		return (Dist < m_pOwner->m_fDetectiveDistance);
	}
	virtual _bool Is_Target_Attackable() {
		auto Dist = fabs(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTarget->Get_Transform()->Get_State(STATE::POSITION))));
		return (Dist< m_pOwner->m_fAttackableDistance);
	}
	virtual void Detect_Target() {
		if (Is_Target_Found())
		{
			/* �� ���� ¥������ �ڵ带 */
			/* ������ ���� ���� ������ �� ������ */
			/* ���ʹ� Idle���¶� Provoked���� ū Ʋ�� �з��� ���� ������ Provoke���� ���� �б⸦ ������ �ɷ� */
			/* Provoke������ �ٵ�..�÷��̾����� ���� ������? */
			m_pOwner->Set_State(CSkag::SKAG_STATE::STATE_Provoked);
		}
	}
	virtual void Call_State(CSkag::SKAG_STATE eNextState) {
		//if (m_)
	}

#ifdef _CONSOLE
	void Console(_float fTimeDelta)
	{
		m_fTestTick += fTimeDelta;
		if (m_fTestTick >= 1.f)
		{
			// ���� ���� ��..
			auto Dist =/* fabs*/(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTarget->Get_Transform()->Get_State(STATE::POSITION))));

			cout << "Dist : " << Dist << endl;
			m_fTestTick = 0.f;
		}
	}
#endif // _CONSOLE

protected:
	CSkag*			m_pOwner;
	CGameObject*	m_pTarget = { nullptr };

	_float			m_fElapsedTime = {};
	CGameInstance*	m_pGameInstance = { nullptr };
	_bool			m_bChangeStateCall = { false };

	_float			m_fTestTick = {};
};

class CSkagState_Idle final : public CSkagState
{
public:
	CSkagState_Idle(class CSkag* pOwner)
		: CSkagState(pOwner) {}
	virtual ~CSkagState_Idle() = default;

public:
	virtual void Enter() override
	{

	}
	virtual void Execute(_float fTimeDelta) override
	{

	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CSkagState_Dead final : public CSkagState
{
public:
	CSkagState_Dead(class CSkag* pOwner)
		: CSkagState(pOwner) {
	}
	virtual ~CSkagState_Dead() = default;

public:
	virtual void Enter() override
	{
		cout << "[Dead]" << endl;
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CSkag::SKAG_ANIM::PhaselockLoop), false);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_Dead();
			return;
		}
		m_pOwner->m_pTransformCom->Go_Down(fTimeDelta);
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

NS_END;
