#include "PSTState.h"
#include "Model.h"


void CPSTState_Idle::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPistol::Idle_Hyperion), true);
}

void CPSTState_Idle::Execute(_float fTimeDelta)
{
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		
	}
}

void CPSTState_Idle::Exit()
{
}
