#include "PlayerState.h"
#include "Model.h"



void CPlayerState_CloseAttack::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_BASIC::BASIC_CloseAttack, false);
}

void CPlayerState_CloseAttack::Execute(_float fTimeDelta)
{
	Default_Movements(fTimeDelta);
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
	}
}

void CPlayerState_CloseAttack::Exit()
{
}
