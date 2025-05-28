#include "ARState.h"
#include "Model.h"


void CARState_Fire::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CAssaultRifle::AR_ANIM::Fire_Jakobs), false);
}

void CARState_Fire::Execute(_float fTimeDelta)
{
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CAssaultRifle::AR_STATE::STATE_Idle);
	}
}

void CARState_Fire::Exit()
{
}
