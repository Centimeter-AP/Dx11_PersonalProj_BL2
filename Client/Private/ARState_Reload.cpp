#include "ARState.h"
#include "Model.h"


void CARState_Reload::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CAssaultRifle::AR_ANIM::R_Jakobs), false);

	m_pOwner->m_pSoundCom->Play("reload_AR");
}

void CARState_Reload::Execute(_float fTimeDelta)
{
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CAssaultRifle::AR_STATE::STATE_Idle);
	}
}

void CARState_Reload::Exit()
{
}
