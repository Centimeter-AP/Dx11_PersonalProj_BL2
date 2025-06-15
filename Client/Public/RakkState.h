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
		m_pTarget = m_pOwner->m_pTarget;
		//Safe_AddRef(m_pGameInstance);
	}
	virtual ~CRakkState() = default;

public:
	virtual void Enter() PURE;
	virtual void Execute(_float fTimeDelta) PURE;
	virtual void Exit() PURE;
	virtual void Free() override { __super::Free(); /*Safe_Release(m_pGameInstance);*/ }

	virtual _bool Is_Target_Found() {
		auto Dist = fabs(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTarget->Get_Transform()->Get_State(STATE::POSITION))));
		return (Dist < m_pOwner->m_fDetectiveDistance);
	}
	virtual _bool Is_Target_Attackable() {
		auto Dist = fabs(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTarget->Get_Transform()->Get_State(STATE::POSITION))));
		return (Dist < m_pOwner->m_fAttackableDistance);
	}
	virtual void Detect_Target() {
		if (Is_Target_Found())
		{
			/* 뭘 어케 짜야하지 코드를 */
			/* 무조건 공격 모드로 가야할 것 같은데 */
			/* 몬스터는 Idle상태랑 Provoked상태 큰 틀로 분류한 다음 전투는 Provoke에서 전부 분기를 나누는 걸로 */
			/* Provoke에서는 근데..플레이어한테 오는 정도만? */
			m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Provoked);
		}
	}
	virtual void Call_State(CRakk::RAKK_STATE eNextState) {
		//if (m_)
	}

#ifdef _CONSOLE
	void Console(_float fTimeDelta)
	{
		m_fTestTick += fTimeDelta;
		if (m_fTestTick >= 1.f)
		{
			// 쓰고 싶은 것..
			auto Dist =/* fabs*/(XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - m_pTarget->Get_Transform()->Get_State(STATE::POSITION))));

			cout << "Dist : " << Dist << endl;
			m_fTestTick = 0.f;
		}
	}
#endif // _CONSOLE

protected:
	CRakk* m_pOwner;
	CGameObject* m_pTarget = { nullptr };

	_float			m_fElapsedTime = {};
	CGameInstance* m_pGameInstance = { nullptr };
	_bool			m_bChangeStateCall = { false };

	_float			m_fTestTick = {};
};

class CRakkState_Idle final : public CRakkState
{
public:
	CRakkState_Idle(class CRakk* pOwner)
		: CRakkState(pOwner) {
	}
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
		////cout << "[Flying_Idle]" << endl;
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
		Console(fTimeDelta);
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

		Detect_Target();
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
		//cout << "[idle_G]" << endl;

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
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::Hover), false);
		//cout << "[Hover]" << endl;

	}
	virtual void Execute(_float fTimeDelta) override
	{
		Console(fTimeDelta);
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			if (rand() % 100 < 20)
				m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Flying_Idle);
			else
				m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Hover);
		}

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
		//cout << "[Attack]" << endl;
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::Attack), false);

	}
	virtual void Execute(_float fTimeDelta) override
	{
		Console(fTimeDelta);
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
		{
			rand() % 2 == 0 ?
				m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Provoked) :
				m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Provoked);
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
		//cout << "[Suicide]" << endl;
		rand() % 2 == 0 ?
			m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::SuicideDive1), false) :
			m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::SuicideDive2), false);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		_vector vTargetPos = m_pOwner->m_pTarget->Get_Transform()->Get_State(STATE::POSITION);
		m_pOwner->m_pTransformCom->Go_Target(vTargetPos, fTimeDelta * 2.f, 0.3f);
		if (XMVectorGetX(XMVector3Length(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION) - vTargetPos)) <= 3.f)
		{
			m_pOwner->Set_Dead();
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
	_float m_fPrePhaseTime = {};

public:
	virtual void Enter() override
	{
		//cout << "[Phaselockd]" << endl;
		m_fPhaselockTime = 0.f;
		m_fPrePhaseTime = 0.f;
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::Hover), true);
		//m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::PhaselockLoop), true);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		if (m_fPrePhaseTime < 1.f)
		{
			m_fPrePhaseTime += fTimeDelta;
			m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);
			m_pOwner->m_pTransformCom->Go_Up(fTimeDelta);
			if (m_fPrePhaseTime >= 1.f)
				m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::PhaselockLoop), true);
		}
		else
		{
			m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);
			m_fPhaselockTime += fTimeDelta;
			if (m_fPhaselockTime >= 2.f)
				m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Hover);
		}

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
		//cout << "[Takeoff]" << endl;
		//vDir = {};
		//_vector vPos = m_pOwner->m_pTransformCom->Get_State(STATE::POSITION);
		//_float3 vTakeoffPos = {};

		//vTakeoffPos.x = XMVectorGetX(vPos) + _float(rand() % 3 - 1.f);
		//vTakeoffPos.y = XMVectorGetY(vPos) + _float(rand() % 3 + 1.f);
		//vTakeoffPos.z = XMVectorGetZ(vPos) + _float(rand() % 3 - 1.f);
		//vDir = XMVector3Normalize(XMLoadFloat3(&vTakeoffPos) - vPos);

		//_vector vToLook = vPos + vDir * 10.f;

		////_float3 vScale = m_pOwner->m_pTransformCom->Get_Scaled();
		////vDir *= XMLoadFloat3(&vScale);
		////m_pOwner->m_pTransformCom->Set_State(STATE::LOOK, vDir);
		//m_pOwner->m_pTransformCom->LookAt(vToLook);

		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::Takeoff), false);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		//m_pOwner->m_pTransformCom->Go_Straight(fTimeDelta);
		m_pOwner->m_pTransformCom->Go_Up(fTimeDelta);
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
		m_pOwner->m_pTransformCom->Go_Target(XMVectorSetY(m_pOwner->m_pTransformCom->Get_State(STATE::POSITION), 0.3f), fTimeDelta * 0.7f, 0.3f);
		if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
			m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Ground_Idle);
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CRakkState_Provoked final : public CRakkState
{
public:
	CRakkState_Provoked(class CRakk* pOwner)
		: CRakkState(pOwner) {
	}
	virtual ~CRakkState_Provoked() = default;

public:
	virtual void Enter() override
	{
		//cout << "[Provoked]" << endl;

		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::Glide01), false);
	}
	virtual void Execute(_float fTimeDelta) override
	{
		m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);

		if (Is_Target_Attackable())
		{
			m_bChangeStateCall = true; // 글라이드 너무길어서 이건 빼고 다른곳에 써야겠다
		}
		if (m_bChangeStateCall)
		{
			m_bChangeStateCall = false;
			if (rand() % 10 == 0)
				m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Flying_SuicidalDive);
			else
				m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Flying_Attack);
			return;
		}
		Console(fTimeDelta);
		m_pOwner->m_pTransformCom->LookAtLerp(m_pTarget->Get_Transform()->Get_State(STATE::POSITION), fTimeDelta, 8.f);
		m_pOwner->m_pTransformCom->Go_Target(m_pOwner->m_pTarget->Get_Transform()->Get_State(STATE::POSITION), fTimeDelta, 0.3f);


		if (!Is_Target_Found())
		{
			m_pOwner->Set_State(CRakk::RAKK_STATE::STATE_Flying_Idle);
			return;
		}
	}
	virtual void Exit() override
	{

	}
	virtual void Free() override { __super::Free(); }
};

class CRakkState_Dead final : public CRakkState
{
public:
	CRakkState_Dead(class CRakk* pOwner)
		: CRakkState(pOwner) {
	}
	virtual ~CRakkState_Dead() = default;

public:
	virtual void Enter() override
	{
		//cout << "[Dead]" << endl;
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CRakk::RAKK_ANIM::PhaselockLoop), false);
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
