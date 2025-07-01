#include "PlayerState.h"
#include "Model.h"
#include "Monster.h"
#include <UI_Phaselock.h>

void CPlayerState_Skill_PhaseLock::Enter()
{
	m_bSuccess = false;
	if (m_pOwner->m_pCurPickedCollider != nullptr)
	{
		m_pOwner->m_bPhaselockAble = false;
		m_pOwner->m_pModelCom->Set_Animation_TickPerSecond(ENUM_CLASS(CPlayer::PLA_BASIC::BASIC_PhaseLock), 13.f);
		m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_BASIC::BASIC_PhaseLock, false);
		static_cast<CMonster*>(m_pOwner->m_pCurPickedCollider->Get_Owner())->OnHit_Phaselock();
		m_bSuccess = true;
	}
	else
	{
		m_pOwner->m_bPhaselockAble = false;
		m_pOwner->m_fPhaselockCooldownTicker = m_pOwner->m_fPhaselockCooldown - 1.f;
		m_pOwner->m_pModelCom->Set_Animation(CPlayer::PLA_BASIC::BASIC_PhaseLock_Fail, false);
	}
	m_bCatched = false;
}

void CPlayerState_Skill_PhaseLock::Execute(_float fTimeDelta)
{
	Default_Movements(fTimeDelta);
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CPlayer::PLA_STATE::STATE_Idle);
	}
	_float fTrackPos = m_pOwner->m_pModelCom->Get_CurrentTrackPosition();
	if (19.f <= fTrackPos && m_bCatched == false && m_bSuccess == true)
	{
		static_cast<CUI_Phaselock*>(m_pOwner->m_PartObjects.find(TEXT("PartObject_Player_UI_Phaselock"))->second)->Set_UsingUI();
		m_bCatched = true;
	}
}

void CPlayerState_Skill_PhaseLock::Exit()
{
}
