#include "PlayerState.h"
#include "Model.h"


void CPlayerState_Jump::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_AR::AR_Jump_Start, false);
}

void CPlayerState_Jump::Execute(_float fTimeDelta)
{
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
	}
	if (KEY_DOWN(DIK_R))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Reload);
		static_cast<CAssaultRifle*>(m_pOwner->m_PartObjects.find(TEXT("PartObject_Player_Weapon_AssaultRifle"))->second)->Set_State(CAssaultRifle::STATE_Reload);
		return;
	}
}

void CPlayerState_Jump::Exit()
{

}
