#include "PSTState.h"
#include "Model.h"


void CPSTState_Fire::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPistol::Fire_Hyperion), false);
}

void CPSTState_Fire::Execute(_float fTimeDelta)
{
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CPistol::PST_STATE::STATE_Idle);
	}
}

void CPSTState_Fire::Exit()
{
}
