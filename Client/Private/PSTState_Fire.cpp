#include "PSTState.h"
#include "Model.h"
#include "MuzzleFlash.h"

void CPSTState_Fire::Enter()
{
	m_pOwner->m_pModelCom->Set_Animation(ENUM_CLASS(CPistol::Fire_Hyperion), false);
	static_cast<CMuzzleFlash*>(m_pOwner->Find_PartObject(TEXT("PartObject_Weapon_Effect_MuzzleFlash")))->Show_Effect();

	m_pOwner->m_iCurAmmoLeft--;
	if (m_pOwner->m_iCurAmmoLeft < 0)
		MSG_BOX("Å«ÀÏ³µµû!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	string tagSound = "Audio_Banks_" + to_string(329 + rand()%6);
	m_pOwner->m_pSoundCom->Play(tagSound);
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
