#include "ARState.h"
#include "Model.h"


void CARState_Draw::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CAssaultRifle::AR_ANIM::Draw), false);
}

void CARState_Draw::Execute(_float fTimeDelta)
{
	if (true == m_pOwner->m_pModelCom->Play_Animation(fTimeDelta))
	{
		m_pOwner->Set_State(CAssaultRifle::AR_STATE::STATE_Idle);
	}
}

void CARState_Draw::Exit()
{
}
