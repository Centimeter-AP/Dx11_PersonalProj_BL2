#include "PlayerState.h"
#include "Model.h"


void CPlayerState_Run_R::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPlayer::PLA_AR::Run_R), true);
}

void CPlayerState_Run_R::Execute(_float fTimeDelta)
{
	m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);
	if (KEY_PRESSING(DIK_D))
	{
		m_pOwner->m_pTransformCom->Go_Right(fTimeDelta);
		if (KEY_PRESSING(DIK_W) || KEY_PRESSING(DIK_S))
		{
			m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Run);
		}
	}
	else
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
	}
}

void CPlayerState_Run_R::Exit()
{

}
