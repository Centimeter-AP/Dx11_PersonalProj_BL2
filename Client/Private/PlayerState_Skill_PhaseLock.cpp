#include "PlayerState.h"
#include "Model.h"

void CPlayerState_Skill_PhaseLock::Enter()
{
	if (m_pOwner->m_pCurPickedCollider != nullptr)
	{
		m_pOwner->m_bPhaselockAble = false;
		m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_BASIC::BASIC_PhaseLock, false);
		m_pOwner->m_pCurPickedCollider->Get_Owner()->On_Collision(ENUM_CLASS(COL_ID::PLAYER_SKILL_PHASELOCK));
	}
	else
	{
		m_pOwner->m_bPhaselockAble = false;
		m_pOwner->m_fPhaselockCooldownTicker = m_pOwner->m_fPhaselockCooldown - 1.f;
		m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_BASIC::BASIC_PhaseLock_Fail, false);
	}
}

void CPlayerState_Skill_PhaseLock::Execute(_float fTimeDelta)
{
	Default_Movements(fTimeDelta);
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
	}
}

void CPlayerState_Skill_PhaseLock::Exit()
{
}
