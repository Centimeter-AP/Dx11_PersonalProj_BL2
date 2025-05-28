#include "PlayerState.h"
#include "Model.h"


void CPlayerState_Idle::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPlayer::PLA_AR::Idle), true);
}

void CPlayerState_Idle::Execute(_float fTimeDelta)
{
	//m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);
	if (KEY_PRESSING(DIK_LSHIFT) && KEY_PRESSING(DIK_W))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Sprint); return;
	}
	else if (KEY_PRESSING(DIK_W) || KEY_PRESSING(DIK_S))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Run); return;
	}
	else if (KEY_PRESSING(DIK_A))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Run_L); return;
	}
	else if (KEY_PRESSING(DIK_D))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Run_R); return;
	}
}

void CPlayerState_Idle::Exit()
{
	//m_pOwner->m_pModelCom->Reset_CurAnimationFrame();
}
