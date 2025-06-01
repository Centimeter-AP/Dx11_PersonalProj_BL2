#include "PlayerState.h"
#include "Model.h"

void CPlayerState_Idle::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPlayer::PLA_AR::AR_Idle), true);
}

void CPlayerState_Idle::Execute(_float fTimeDelta)
{
	m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);
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

	if (KEY_DOWN(DIK_R))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Reload); return;
	}
	if (KEY_DOWN(DIK_G))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_ThrowGrenade); return;
	}
	if (KEY_DOWN(DIK_V))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_CloseAttack); return;
	}
	if (KEY_DOWN(DIK_F))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Skill_PhaseLock); return;
	}

	/*임시단축키*/
	if (KEY_DOWN(DIK_1))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_HolsterWeapon); return;
	}
	if (MOUSE_DOWN(DIM::LBUTTON))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Fire); return;
	}

	/* 총 쏘는 애니메이션 쓸 수 있게 만들어 보던가 수동 반동 만들던가.. */
	//if (KEY_DOWN(DIK_F))
	//{
	//	m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Fire); return;
	//}
}

void CPlayerState_Idle::Exit()
{

}
