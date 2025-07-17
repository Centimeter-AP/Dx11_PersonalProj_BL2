#include "PlayerState.h"
#include "Model.h"


void CPlayerState_Sprint::Enter()
{
	m_pOwner->m_bSprint = true;
	switch (m_pOwner->m_eCurWeapon)
	{
	case CPlayer::WEAPON_TYPE::WTYPE_AR:
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPlayer::AR_Sprint), true);
		break;
	case CPlayer::WEAPON_TYPE::WTYPE_PISTOL:
		m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPlayer::PST_Sprint), true);
		break;
	case CPlayer::WEAPON_TYPE::WTYPE_UNARMED:
		break;
	default:
		break;
	}
}

void CPlayerState_Sprint::Execute(_float fTimeDelta)
{
	m_pOwner->m_pModelCom->Play_Animation(fTimeDelta);
	m_pOwner->m_bWalking = true;
	if (!KEY_PRESSING(DIK_W))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
	}
	else
	{
		m_pOwner->m_pTransformCom->Go_Straight_Hover(fTimeDelta * m_fSpeedMultiplier, m_pOwnerNavi, *m_pIsGrounded);

		if (KEY_PRESSING(DIK_A))
		{
			m_pOwner->m_pTransformCom->Go_Left(fTimeDelta * m_fSpeedMultiplier, m_pOwnerNavi, *m_pIsGrounded);
		}
		if (KEY_PRESSING(DIK_D))
		{
			m_pOwner->m_pTransformCom->Go_Right(fTimeDelta * m_fSpeedMultiplier, m_pOwnerNavi, *m_pIsGrounded);
		}
		if (KEY_PRESSING(DIK_S))
		{
			m_pOwner->m_bSprint = false;
			m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
		}
	}
	if (KEY_DOWN(DIK_R))
	{
		m_pOwner->m_bSprint = false;
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Reload); return;
	}
	if (KEY_DOWN(DIK_F) && true == m_pOwner->m_bPhaselockAble)
	{
		m_pOwner->m_bSprint = false;
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Skill_PhaseLock); return;
	}

	if (MOUSE_DOWN(DIM::LBUTTON))
	{
		m_pOwner->m_bSprint = false;
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

void CPlayerState_Sprint::Exit()
{

}
