#include "ARState.h"
#include "Model.h"


void CARState_Idle::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CAssaultRifle::AR_ANIM::Idle), true);
}

void CARState_Idle::Execute(_float fTimeDelta)
{
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		
	}
}

void CARState_Idle::Exit()
{
}
