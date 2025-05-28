#include "ARState.h"
#include "Model.h"


void CARState_Reload_Fast::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CAssaultRifle::AR_ANIM::R_Jakobs_Fast), false);
}

void CARState_Reload_Fast::Execute(_float fTimeDelta)
{
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CAssaultRifle::AR_STATE::STATE_Idle);
	}
}

void CARState_Reload_Fast::Exit()
{
}
