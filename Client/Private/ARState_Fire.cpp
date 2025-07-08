#include "ARState.h"
#include "Model.h"


void CARState_Fire::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CAssaultRifle::AR_ANIM::Fire_Jakobs), false);
	m_pOwner->m_iCurAmmoLeft--;
	if (m_pOwner->m_iCurAmmoLeft < 0)
		MSG_BOX("Å«ÀÏ³µµû!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
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
