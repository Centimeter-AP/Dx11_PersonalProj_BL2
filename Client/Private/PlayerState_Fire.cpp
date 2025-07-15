#include "PlayerState.h"
#include "Model.h"
#include "Camera_FPS.h"


void CPlayerState_Fire::Enter()
{
	switch (m_pOwner->m_eCurWeapon)
	{
	case CPlayer::WEAPON_TYPE::WTYPE_AR:
		if (0 >= static_cast<CAssaultRifle*>(m_pOwner->m_PartObjects.find(TEXT("PartObject_Player_Weapon_AR"))->second)->Get_CurAmmoLeft())
		{
			Check_Reloadable(); return;
		}
		m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_AR::AR_Crouch, false);
		m_pOwner->m_pModelCom->Reset_CurAnimationFrame();
		static_cast<CAssaultRifle*>(m_pOwner->m_PartObjects.find(TEXT("PartObject_Player_Weapon_AR"))->second)->Set_State(CAssaultRifle::STATE_Fire);
		if (m_pOwner->m_pCurPickedCollider != nullptr) 
			//m_pOwner->m_pCurPickedCollider->Get_Owner()->Set_State_Dead();
			m_pOwner->m_pCurPickedCollider->Get_Owner()->On_Collision(m_pOwner->m_pCurPickedCollider->Get_ColliderID(), ENUM_CLASS(COL_ID::PLAYER_WEAPON_AR), m_pOwner->m_pColliderCom);
		break;
	case CPlayer::WEAPON_TYPE::WTYPE_PISTOL:
		if (0 >= static_cast<CPistol*>(m_pOwner->m_PartObjects.find(TEXT("PartObject_Player_Weapon_Pistol"))->second)->Get_CurAmmoLeft())
		{
			Check_Reloadable(); return;
		}
		m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_PST::PST_Crouch, false);
		m_pOwner->m_pModelCom->Reset_CurAnimationFrame();
		static_cast<CPistol*>(m_pOwner->m_PartObjects.find(TEXT("PartObject_Player_Weapon_Pistol"))->second)->Set_State(CPistol::STATE_Fire);
		if (m_pOwner->m_pCurPickedCollider != nullptr)
			//m_pOwner->m_pCurPickedCollider->Get_Owner()->Set_State_Dead();
			m_pOwner->m_pCurPickedCollider->Get_Owner()->On_Collision(m_pOwner->m_pCurPickedCollider->Get_ColliderID(), ENUM_CLASS(COL_ID::PLAYER_WEAPON_PST), m_pOwner->m_pColliderCom);
		break;
	case CPlayer::WEAPON_TYPE::WTYPE_UNARMED:
		break;
	default:
		break;
	}
	//if (m_pOwner->m_pCurPickedCollider != nullptr)
	//	//m_pOwner->m_pCurPickedCollider->Get_Owner()->Set_State_Dead();
	//	m_pOwner->m_pCurPickedCollider->Get_Owner()->On_Collision(ENUM_CLASS(COL_ID::PLAYER_WEAPON_AR));
	CCamera_FPS* pCamera = dynamic_cast<CCamera_FPS*>(m_pGameInstance->Find_Object(m_pOwner->Get_CurLevel(), L"Layer_Camera", CAM_FPS));
	if (pCamera == nullptr)
	{
		MSG_BOX("CCamera_FPS is nullptr");
		return;
	}
	pCamera->Start_Recoil();
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
