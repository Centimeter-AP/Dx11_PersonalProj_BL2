#include "PlayerState.h"
#include "Model.h"


void CPlayerState_Jump::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_AR::Jump_Start, false);
}

void CPlayerState_Jump::Execute(_float fTimeDelta)
{
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
	}
}

void CPlayerState_Jump::Exit()
{

}
