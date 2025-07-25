#include "PlayerState.h"
#include "Model.h"


void CPlayerState_Jump::Enter()
{
	m_isLanded = m_isJumping = false;
	m_pOwner->m_pGravityCom->Jump();
	m_pOwner->m_bJumped = true;
	switch (m_pOwner->m_eCurWeapon)
	{
	case CPlayer::WEAPON_TYPE::WTYPE_AR:
		m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_AR::AR_Jump_Start, false);
		break;
	case CPlayer::WEAPON_TYPE::WTYPE_PISTOL:
		m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_PST::PST_Jump_Start, false);
		break;
	case CPlayer::WEAPON_TYPE::WTYPE_UNARMED:
		break;
	default:
		break;
	}
	string strSound = "Audio_Banks_" + to_string(7523 + rand() % 3);
	m_pOwner->m_pSoundCom->SetVolume(strSound, 0.4f);
	m_pOwner->m_pSoundCom->Play(strSound);
	strSound = "Siren_PL_Start_Jump_0" + to_string(1 + rand() % 5);
	m_pOwner->m_pSoundCom->SetVolume(strSound, 0.7f);
	m_pOwner->m_pSoundCom->Play(strSound);
}

void CPlayerState_Jump::Execute(_float fTimeDelta)
{
	if (m_pOwner->m_bSprint)
		Default_Movements(fTimeDelta * 2.f);
	else
		Default_Movements(fTimeDelta);
	if (!KEY_PRESSING(DIK_W))
		m_pOwner->m_bSprint = false;
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		if (m_isJumping == false)
		{
			switch (m_pOwner->m_eCurWeapon)
			{
			case CPlayer::WEAPON_TYPE::WTYPE_AR:
				m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_AR::AR_Jump_Idle, true);
				break;
			case CPlayer::WEAPON_TYPE::WTYPE_PISTOL:
				m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_PST::PST_Jump_Idle, true);

				break;
			case CPlayer::WEAPON_TYPE::WTYPE_UNARMED:
				break;
			default:
				break;
			}
			m_isJumping = true;
		}
		else
		{

			m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
		}
	}
	if (m_pOwner->m_pGravityCom->Is_Grounded() && m_isLanded == false)
	{
		switch (m_pOwner->m_eCurWeapon)
		{
		case CPlayer::WEAPON_TYPE::WTYPE_AR:
			m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_AR::AR_Jump_End, false);
			break;
		case CPlayer::WEAPON_TYPE::WTYPE_PISTOL:
			m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_PST::PST_Jump_End, false);

			break;
		case CPlayer::WEAPON_TYPE::WTYPE_UNARMED:
			break;
		default:
			break;
		}
		m_isLanded = true;
		//if (KEY_PRESSING(DIK_W))
		//if (KEY_PRESSING(DIK_S)|| KEY_PRESSING(DIK_A)|| KEY_PRESSING(DIK_D))

	}
	if (KEY_DOWN(DIK_R))
	{
		Check_Reloadable(); return;
	}
	if (MOUSE_DOWN(DIM::LBUTTON))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Fire); return;
	}
	if (KEY_DOWN(DIK_1))
	{
		m_pOwner->Change_Weapon(CPlayer::WTYPE_AR); return;
	}
	if (KEY_DOWN(DIK_2))
	{
		m_pOwner->Change_Weapon(CPlayer::WTYPE_PISTOL); return;
	}
	if (KEY_DOWN(DIK_F) && true == m_pOwner->m_bPhaselockAble)
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Skill_PhaseLock); return;
	}
}

void CPlayerState_Jump::Exit()
{

}
