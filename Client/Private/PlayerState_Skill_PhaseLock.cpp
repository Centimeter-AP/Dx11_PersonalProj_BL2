#include "PlayerState.h"
#include "Model.h"

void CPlayerState_Skill_PhaseLock::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_BASIC::BASIC_PhaseLock, false); // 애니메이션을 추가해야돼!! 
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
