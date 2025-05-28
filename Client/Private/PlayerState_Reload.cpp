#include "PlayerState.h"
#include "Model.h"



void CPlayerState_Reload::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_AR::R_Jakobs, false);
}

void CPlayerState_Reload::Execute(_float fTimeDelta)
{
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
	}
}

void CPlayerState_Reload::Exit()
{
}
