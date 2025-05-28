#include "PlayerState.h"
#include "Model.h"


void CPlayerState_Jump::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_AR::Jump_Start, true);
}

void CPlayerState_Jump::Execute(_float fTimeDelta)
{

}

void CPlayerState_Jump::Exit()
{
}
