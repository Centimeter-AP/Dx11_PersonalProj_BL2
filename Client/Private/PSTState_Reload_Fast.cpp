#include "PSTState.h"
#include "Model.h"


void CPSTState_Reload_Fast::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPistol::PST_ANIM::R_Hyperion_Fast), false);
}

void CPSTState_Reload_Fast::Execute(_float fTimeDelta)
{
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CPistol::PST_STATE::STATE_Idle);
	}
}

void CPSTState_Reload_Fast::Exit()
{
}
