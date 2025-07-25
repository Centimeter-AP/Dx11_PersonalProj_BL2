#include "PlayerState.h"
#include "Model.h"


void CPlayerState_Run_L::Enter()
{
	switch (m_pOwner->m_eCurWeapon)
	{
	case CPlayer::WEAPON_TYPE::WTYPE_AR:
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPlayer::AR_Run_L), true);
		break;
	case CPlayer::WEAPON_TYPE::WTYPE_PISTOL:
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPlayer::PST_Run_L), true);
		break;
	case CPlayer::WEAPON_TYPE::WTYPE_UNARMED:
		break;
	default:
		break;
	}
}

void CPlayerState_Run_L::Execute(_float fTimeDelta)
{
	m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);
	m_pOwner->m_bWalking = true;
	if (KEY_PRESSING(DIK_A))
	{
		m_pOwner->m_pTransformCom->Go_Left(fTimeDelta, m_pOwnerNavi, *m_pIsGrounded);
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
		Check_Reloadable(); return;
	}
	if (KEY_DOWN(DIK_F) && true == m_pOwner->m_bPhaselockAble)
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Skill_PhaseLock); return;
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
}

void CPlayerState_Run_L::Exit()
{
}
