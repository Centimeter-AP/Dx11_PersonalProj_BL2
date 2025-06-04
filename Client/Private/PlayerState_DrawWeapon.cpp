#include "PlayerState.h"
#include "Model.h"



void CPlayerState_DrawWeapon::Enter() // 무슨 무기로 바꿨는지 정보 받아야함 
{
	switch (m_pOwner->m_eCurWeapon)
	{
	case CPlayer::WEAPON_TYPE::WTYPE_AR:
		m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_AR::AR_Draw, false);
		m_pOwner->Replace_PartObject(TEXT("PartObject_Player_Weapon"), m_pOwner->m_pWeapons[CPlayer::WTYPE_AR]);
		break;
	case CPlayer::WEAPON_TYPE::WTYPE_PISTOL:
		m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_PST::PST_Draw, false);
		m_pOwner->Replace_PartObject(TEXT("PartObject_Player_Weapon"), m_pOwner->m_pWeapons[CPlayer::WTYPE_PISTOL]);
		break;
	case CPlayer::WEAPON_TYPE::WTYPE_UNARMED:
		break;
	default:
		break;
	}
	m_pOwner->m_ePrevWeapon = m_pOwner->m_eCurWeapon;
}

void CPlayerState_DrawWeapon::Execute(_float fTimeDelta)
{
	Default_Movements(fTimeDelta);
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
	}
}

void CPlayerState_DrawWeapon::Exit()
{
}
