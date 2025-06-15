#include "PlayerState.h"
#include "Model.h"



void CPlayerState_Fire::Enter()
{
	switch (m_pOwner->m_eCurWeapon)
	{
	case CPlayer::WEAPON_TYPE::WTYPE_AR:
		m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_AR::AR_Crouch, false);
		m_pOwner->m_pModelCom->Reset_CurAnimationFrame();
		static_cast<CAssaultRifle*>(m_pOwner->m_PartObjects.find(TEXT("PartObject_Player_Weapon_AR"))->second)->Set_State(CAssaultRifle::STATE_Fire);
		break;
	case CPlayer::WEAPON_TYPE::WTYPE_PISTOL:
		m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_PST::PST_Crouch, false);
		m_pOwner->m_pModelCom->Reset_CurAnimationFrame();
		static_cast<CPistol*>(m_pOwner->m_PartObjects.find(TEXT("PartObject_Player_Weapon_Pistol"))->second)->Set_State(CPistol::STATE_Fire);
		break;
	case CPlayer::WEAPON_TYPE::WTYPE_UNARMED:
		break;
	default:
		break;
	}
	if (m_pOwner->m_pCurPickedCollider != nullptr)
		m_pOwner->m_pCurPickedCollider->Get_Owner()->Set_State_Dead();
}

void CPlayerState_Fire::Execute(_float fTimeDelta)
{
	Default_Movements(fTimeDelta);
	if (MOUSE_DOWN(DIM::LBUTTON))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Fire); return;
	}
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
	}

}

void CPlayerState_Fire::Exit()
{
}
