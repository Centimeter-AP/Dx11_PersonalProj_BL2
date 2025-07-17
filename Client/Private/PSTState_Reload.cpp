#include "PSTState.h"
#include "Model.h"


void CPSTState_Reload::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPistol::PST_ANIM::R_Tor), false); // 건들지마시오
	m_pOwner->m_pSoundCom->Play("reload_pistol");
}

void CPSTState_Reload::Execute(_float fTimeDelta)
{
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CPistol::PST_STATE::STATE_Idle);
	}
}

void CPSTState_Reload::Exit()
{
}
