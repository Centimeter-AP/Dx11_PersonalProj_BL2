#include "PlayerState.h"
#include "Model.h"


void CPlayerState_Sprint::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_AR::Sprint, true);
}

void CPlayerState_Sprint::Execute(_float fTimeDelta)
{
	if (!KEY_PRESSING(DIK_W))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
	}
	else
	{
		m_pOwner->m_pTransformCom->Go_Straight(fTimeDelta * m_fSpeedMultiplier);

		if (KEY_PRESSING(DIK_A))
		{
			m_pOwner->m_pTransformCom->Go_Left(fTimeDelta * m_fSpeedMultiplier);
		}
		if (KEY_PRESSING(DIK_D))
		{
			m_pOwner->m_pTransformCom->Go_Right(fTimeDelta * m_fSpeedMultiplier);
		}
		if (KEY_PRESSING(DIK_S))
		{
			m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
		}
	}
}

void CPlayerState_Sprint::Exit()
{

}
