#include "PlayerState.h"
#include "Model.h"

void CPlayerState_Idle::Enter()
{
	switch (m_pOwner->m_eCurWeapon)
	{
	case CPlayer::WEAPON_TYPE::WTYPE_AR:
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPlayer::AR_Idle), true);
		break;
	case CPlayer::WEAPON_TYPE::WTYPE_PISTOL:
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPlayer::PST_Idle), true);
		break;
	case CPlayer::WEAPON_TYPE::WTYPE_UNARMED:
		break;
	default:
		break;
	}
}

void CPlayerState_Idle::Execute(_float fTimeDelta)
{
	m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);
	if ((KEY_PRESSING(DIK_LSHIFT) && KEY_PRESSING(DIK_W)) || (m_pOwner->m_bSprint && KEY_PRESSING(DIK_W)))
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
	else
	{
		m_pOwner->m_bSprint = false;
	}

	if (KEY_DOWN(DIK_R))
	{
		Check_Reloadable(); return;
	}
	if (KEY_DOWN(DIK_G))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_ThrowGrenade); return;
	}
	if (KEY_DOWN(DIK_V))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_CloseAttack); return;
	}
	if (KEY_DOWN(DIK_F) && true == m_pOwner->m_bPhaselockAble)
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Skill_PhaseLock); return;
	}

	if (KEY_DOWN(DIK_1))
	{
		m_pOwner->Change_Weapon(CPlayer::WTYPE_AR);
	}
	if (KEY_DOWN(DIK_2))
	{
		m_pOwner->Change_Weapon(CPlayer::WTYPE_PISTOL);
	}
	if (MOUSE_DOWN(DIM::LBUTTON))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Fire); return;
	}
	if (KEY_DOWN(DIK_SPACE))
	{
		if (m_pOwner->m_pGravityCom->Is_Grounded())
		{
			m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Jump); return;
		}
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
