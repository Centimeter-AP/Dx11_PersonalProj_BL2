#pragma once

#include "Rakk.h"
#include "GameInstance.h"

NS_BEGIN(Client)

class CRakkState abstract : public CBase
{
public:
	CRakkState(class CRakk* pOwner)
		: m_pOwner(pOwner),
		m_pGameInstance(CGameInstance::Get_Instance())
	{
		Safe_AddRef(m_pGameInstance);
	}
	virtual ~CRakkState() = default;

public:
	virtual void Enter() PURE;
	virtual void Execute(_float fTimeDelta) PURE;
	virtual void Exit() PURE;
	virtual void Free() override { __super::Free(); Safe_Release(m_pGameInstance); }

protected:
	CRakk*			m_pOwner;
	CGameObject*	m_pTarget = { nullptr };

	_float			m_fElapsedTime = {};
	CGameInstance*	m_pGameInstance = { nullptr };
	_bool			m_bChangeStateCall = { false };
};

class CRakkState_Idle final : public CRakkState
{
public:
	CRakkState_Idle(class CRakk* pOwner)
		: CRakkState(pOwner) {}
	virtual ~CRakkState_Idle() = default;

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

class CRakkState_Flying_Idle final : public CRakkState
{
public:
	CRakkState_Flying_Idle(class CRakk* pOwner)
		: CRakkState(pOwner) {
	}
	virtual ~CRakkState_Flying_Idle() = default;

public:
	virtual void Enter() override
	{
		switch (rand() % 4)
		{
		case 0:
			m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::Fly01), false);
			break;
		case 1:
			m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::Fly02), false);
			break;
		case 2:
			m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::Fly04), false);
			break;
		case 3:
			m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::Glide01), false);
			break;
		default:
			m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::Fly01), false);
			break;
		}
	}

	virtual void Execute(_float fTimeDelta) override
	{
		m_pOwner->m_fRotatingDegree += fTimeDelta * 45.f;

		m_pOwner->m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_pOwner->m_fRotatingDegree));
		m_pOwner->m_pTransformCom->Go_Straight(fTimeDelta);

		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Flying_Idle);
		}

		if (KEY_DOWN(DIK_F))
		{
			m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_PhaseLocked); 
			return;
		}
		
		if (KEY_DOWN(DIK_0))
		{
			m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Landing);
			return;
		}

	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CRakkState_Ground_Idle final : public CRakkState
{
public:
	CRakkState_Ground_Idle(class CRakk* pOwner)
		: CRakkState(pOwner) {
	}
	virtual ~CRakkState_Ground_Idle() = default;

public:
	virtual void Enter() override
	{
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::GroundIdle), true);

	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
			return;
		if (KEY_DOWN(DIK_SPACE))
			m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Takeoff);

	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CRakkState_Hover final : public CRakkState
{
public:
	CRakkState_Hover(class CRakk* pOwner)
		: CRakkState(pOwner) {
	}
	virtual ~CRakkState_Hover() = default;

private:
	_float m_fHoverTimer = {};

public:
	virtual void Enter() override
	{
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::Hover), true);

	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
			return;
		if (rand() % 100 < 8)
			m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Flying_Idle);
		if (rand() % 100 < 30)
			m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Flying_Attack);

	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CRakkState_Attack final : public CRakkState
{
public:
	CRakkState_Attack(class CRakk* pOwner)
		: CRakkState(pOwner) {
	}
	virtual ~CRakkState_Attack() = default;

public:
	virtual void Enter() override
	{
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::Attack), false);

	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			rand() % 2 == 0 ?
				m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Hover) :
				m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Takeoff);
		}

	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CRakkState_SuicideDive final : public CRakkState
{
public:
	CRakkState_SuicideDive(class CRakk* pOwner)
		: CRakkState(pOwner) {
	}
	virtual ~CRakkState_SuicideDive() = default;

public:
	virtual void Enter() override
	{
		rand() % 2 == 0 ?
			m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::SuicideDive1), false) :
			m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::SuicideDive2), false);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		_vector vTargetPos = m_pOwner->m_pTarget->Get_Transform()->Get_State(STATE::POSITION);
		m_pOwner->m_pTransformCom->Go_Target(vTargetPos, fTimeDelta, 4.f);
		if (XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - vTargetPos)) <= 5.f)
		{
			m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Takeoff);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CRakkState_Phaselocked final : public CRakkState
{
public:
	CRakkState_Phaselocked(class CRakk* pOwner)
		: CRakkState(pOwner) {
	}
	virtual ~CRakkState_Phaselocked() = default;

private:
	_float m_fPhaselockTime = {};
public:
	virtual void Enter() override
	{
		m_fPhaselockTime = 0.f;
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::PhaselockLoop), true);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_fPhaselockTime += fTimeDelta;
		if (m_fPhaselockTime >= 2.f)
			m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Hover);
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CRakkState_Takeoff final : public CRakkState
{
public:
	CRakkState_Takeoff(class CRakk* pOwner)
		: CRakkState(pOwner) {
	}
	virtual ~CRakkState_Takeoff() = default;
private:
	_vector vDir = {};

public:
	virtual void Enter() override
	{
		vDir = {};
		_float3 vTakeoffPos = {};

		XMStoreFloat3(&vTakeoffPos, m_pOwner->m_pTransformCom->Get_State(STATE::POSITION));
		vTakeoffPos.x += _float(rand() % 3 - 1.f);
		vTakeoffPos.y += _float(rand() % 3 + 1.f);
		vTakeoffPos.z += _float(rand() % 3 - 1.f);
		vDir = XMVector3Normalize(XMLoadFloat3(&vTakeoffPos) - m_pOwner->m_pTransformCom->Get_State(STATE::POSITION));
		_float3 vScale = m_pOwner->m_pTransformCom->Get_Scaled();
		vDir *= XMLoadFloat3(&vScale);
		m_pOwner->m_pTransformCom->Set_State(STATE::LOOK, vDir);

		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::Takeoff), false);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_pOwner->m_pTransformCom->Go_Straight(fTimeDelta);
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Flying_Idle);
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CRakkState_Landing final : public CRakkState
{
public:
	CRakkState_Landing(class CRakk* pOwner)
		: CRakkState(pOwner) {
	}
	virtual ~CRakkState_Landing() = default;

public:
	virtual void Enter() override
	{
		rand() % 2 == 0 ?
			m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::Landing1), false) :
			m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::Landing2), false);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_pOwner->m_pTransformCom->Go_Target(XMVectorSetY(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION), 3.f), fTimeDelta * 0.7f, 0.3f);
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
			m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Ground_Idle);


	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

NS_END;
