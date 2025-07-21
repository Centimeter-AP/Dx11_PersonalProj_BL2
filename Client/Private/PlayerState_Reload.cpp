#include "PlayerState.h"
#include "Model.h"


void CPlayerState_Reload::Enter()
{
	switch (m_pOwner->m_eCurWeapon)
	{
	case CPlayer::WEAPON_TYPE::WTYPE_AR:
		m_pOwner->m_pModelCom->Set_Animation(CPlayer::AR_R_Jakobs, false);
		static_cast<CAssaultRifle*>(m_pOwner->m_PartObjects.find(TEXT("PartObject_Player_Weapon_AR"))->second)->Set_State(CAssaultRifle::STATE_Reload);
		static_cast<CAssaultRifle*>(m_pOwner->m_PartObjects.find(TEXT("PartObject_Player_Weapon_AR"))->second)->Reload_Magazine(m_pOwner->m_iAmmo[CPlayer::WTYPE_AR]);
		break;

	case CPlayer::WEAPON_TYPE::WTYPE_PISTOL:
		m_pOwner->m_pModelCom->Set_Animation(CPlayer::PST_R_Jakobs, false);
		static_cast<CPistol*>(m_pOwner->m_PartObjects.find(TEXT("PartObject_Player_Weapon_Pistol"))->second)->Set_State(CPistol::STATE_Reload);
		static_cast<CPistol*>(m_pOwner->m_PartObjects.find(TEXT("PartObject_Player_Weapon_Pistol"))->second)->Reload_Magazine(m_pOwner->m_iAmmo[CPlayer::WTYPE_PISTOL]);
		break;
	case CPlayer::WEAPON_TYPE::WTYPE_UNARMED:
		break;
	default:
		break;
	}
}

void CPlayerState_Reload::Execute(_float fTimeDelta)
{
	Default_Movements(fTimeDelta);
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
	}
	if (KEY_DOWN(DIK_SPACE))
	{
		if (m_pOwner->m_pGravityCom->Is_Grounded())
		{
			m_pOwner->m_pGravityCom->Jump();
			m_pOwner->m_bJumped = true; 
			return;
		}
	}
}

void CPlayerState_Reload::Exit()
{
}
