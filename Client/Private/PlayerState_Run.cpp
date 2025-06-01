#include "PlayerState.h"
#include "Model.h"


void CPlayerState_Run::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPlayer::PLA_AR::AR_Run_F), true);
}

void CPlayerState_Run::Execute(_float fTimeDelta)
{
	m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);
	if (!KEY_PRESSING(DIK_W))
	{
		if (!KEY_PRESSING(DIK_S))
		{
			if (KEY_PRESSING(DIK_A)){
				m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Run_L); return;
			}
			else if (KEY_PRESSING(DIK_D)){
				m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Run_R); return;
			}
			else{
				m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle); return;
			}
		}
		else
		{
			m_pOwner->m_pTransformCom->Go_Backward(fTimeDelta);
			if (KEY_PRESSING(DIK_A)){
				m_pOwner->m_pTransformCom->Go_Left(fTimeDelta);
			}
			if (KEY_PRESSING(DIK_D)){
				m_pOwner->m_pTransformCom->Go_Right(fTimeDelta);
			}
			return;
		}
	}
	if (KEY_PRESSING(DIK_W))
	{
		m_pOwner->m_pTransformCom->Go_Straight(fTimeDelta);
		if (KEY_PRESSING(DIK_LSHIFT)){
			m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Sprint); return;
		}
	}
	if (KEY_PRESSING(DIK_A)){
		m_pOwner->m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_D)){
		m_pOwner->m_pTransformCom->Go_Right(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_S)){
		m_pOwner->m_pTransformCom->Go_Backward(fTimeDelta);
	}

	if (KEY_DOWN(DIK_R))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Reload);
		return;
	}
}

void CPlayerState_Run::Exit()
{

}
