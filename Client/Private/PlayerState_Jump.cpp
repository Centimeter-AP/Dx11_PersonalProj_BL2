#include "PlayerState.h"
#include "Model.h"


void CPlayerState_Jump::Enter()
{
	m_isJumping = false;
	m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_AR::AR_Jump_Start, false);
	m_pOwner->m_pGravityCom->Jump();
}

void CPlayerState_Jump::Execute(_float fTimeDelta)
{
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		if (m_isJumping == false)
		{
			m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_AR::AR_Jump_Idle, true);
			m_isJumping = true;
		}
		else
			m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
	}
	if (m_pOwner->m_pGravityCom->Is_Grounded())
	{
		m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_AR::AR_Jump_End, false);
	}
	if (KEY_DOWN(DIK_R))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Reload); return;
	}
	if (MOUSE_DOWN(DIM::LBUTTON))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Fire); return;
	}
}

void CPlayerState_Jump::Exit()
{

}
