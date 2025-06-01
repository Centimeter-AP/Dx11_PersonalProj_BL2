#include "PlayerState.h"
#include "Model.h"



void CPlayerState_DrawWeapon::Enter() // 무슨 무기로 바꿨는지 정보 받아야함 
{
	m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_AR::AR_Draw, false);
}

void CPlayerState_DrawWeapon::Execute(_float fTimeDelta)
{
	if (KEY_PRESSING(DIK_W))
	{
		m_pOwner->m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_S))
	{
		m_pOwner->m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_A))
	{
		m_pOwner->m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (KEY_PRESSING(DIK_D))
	{
		m_pOwner->m_pTransformCom->Go_Right(fTimeDelta);
	}
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
	}
}

void CPlayerState_DrawWeapon::Exit()
{
}
