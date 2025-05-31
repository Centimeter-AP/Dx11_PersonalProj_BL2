#include "PlayerState.h"
#include "Model.h"


void CPlayerState_Run_L::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPlayer::PLA_AR::Run_L), true);
}

void CPlayerState_Run_L::Execute(_float fTimeDelta)
{
	m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);
	if (KEY_PRESSING(DIK_A))
	{
		m_pOwner->m_pTransformCom->Go_Left(fTimeDelta);
		if (KEY_PRESSING(DIK_W) || KEY_PRESSING(DIK_S))
		{
			m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Run);
		}
	}
	else
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
	}

	if (KEY_DOWN(DIK_R))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Reload);
		return;
	}
}

void CPlayerState_Run_L::Exit()
{

}
