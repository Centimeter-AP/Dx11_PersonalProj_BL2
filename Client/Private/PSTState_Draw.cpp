#include "PSTState.h"
#include "Model.h"


void CPSTState_Draw::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPistol::Idle_Hyperion), true);
}

void CPSTState_Draw::Execute(_float fTimeDelta)
{
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CPistol::PST_STATE::STATE_Idle);
	}
}

void CPSTState_Draw::Exit()
{
}
