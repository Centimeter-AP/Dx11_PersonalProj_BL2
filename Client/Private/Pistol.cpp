#include "Pistol.h"
#include "PSTState.h"
#include "MuzzleFlash.h"
#include "GameInstance.h"

CPistol::CPistol(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWeapon{ pDevice, pContext }
{

}

CPistol::CPistol(const CPistol& Prototype)
	: CWeapon(Prototype)
{

}

HRESULT CPistol::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPistol::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);
	if (pDesc == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PSTStates()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects(pArg)))
		return E_FAIL;

	Initialize_BasicStatus();

	m_pModelCom->Set_Animation(3, false);
	m_pModelCom->Set_Animation_TickPerSecond_All(30.f);
	m_pModelCom->Play_Animation(0.01f);
	return S_OK;
}

void CPistol::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
	__super::Priority_Update(fTimeDelta);
	Update_State(fTimeDelta);

}

EVENT CPistol::Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return EVN_NONE;

	return __super::Update(fTimeDelta);
}

void CPistol::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
	__super::Late_Update(fTimeDelta);
}

HRESULT CPistol::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;



	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			continue;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NormalTexture", 0)))
			return E_FAIL;
		m_pModelCom->Bind_Bone_Matrices(m_pShaderCom, "g_BoneMatrices", i);
		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

void CPistol::Set_State(PST_STATE eState)
{
	m_ePrevState = m_eCurState;
	m_eCurState = eState;

	m_pCurState->Exit();
	m_pCurState = m_pStates[eState];
	m_pCurState->Enter();
}

void CPistol::Update_State(_float fTimeDelta)
{
	//if (m_ePrevState != m_eCurState)
	//{
	//	m_pCurState->Enter();
	//	m_ePrevState = m_eCurState;
	//}
	m_pCurState->Execute(fTimeDelta);
}


HRESULT CPistol::Ready_Components(void* pArg)
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Pistol"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_PST_Nor"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CPistol::Ready_PSTStates()
{
	m_pStates[PST_STATE::STATE_Idle] = new CPSTState_Idle(this);
	m_pStates[PST_STATE::STATE_Draw] = new CPSTState_Draw(this);
	m_pStates[PST_STATE::STATE_Fire] = new CPSTState_Fire(this);
	m_pStates[PST_STATE::STATE_Reload] = new CPSTState_Reload(this);
	m_pStates[PST_STATE::STATE_Reload_Fast] = new CPSTState_Reload_Fast(this);
	m_pCurState = m_pStates[PST_STATE::STATE_Idle];

	return S_OK;
}

HRESULT CPistol::Ready_PartObjects(void* pArg)
{
	CMuzzleFlash::DESC			MuzzleDesc;
	MuzzleDesc.pParentObject = this;
	//MuzzleDesc.pParentMatrix = m_pModelCom->Get_CombinedTransformationMatrix(m_pModelCom->Find_BoneIndex("WeaponOffset"));
	MuzzleDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix4x4Ptr();
	MuzzleDesc.iLevelID = ENUM_CLASS(LEVEL::STATIC);
	MuzzleDesc.eShaderPath = POSTEX_GRID_PICK;
	MuzzleDesc.bHasTransformPreset = true;
	XMStoreFloat4x4(&MuzzleDesc.PresetMatrix,XMMatrixIdentity());
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC),
		TEXT("PartObject_Weapon_Effect_MuzzleFlash"), TEXT("Prototype_GameObject_Effect_MuzzleFlash"), &MuzzleDesc)))
		return E_FAIL;

	return S_OK;
}

void CPistol::Initialize_BasicStatus()
{
	m_iDamage = { 23 };
	m_fAccuracy = { 92.2f };
	m_fFireRate = { 16.7f };
	m_iMagazineSize = { 6 };
	m_iCurAmmoLeft = { 6 };
}

CPistol* CPistol::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPistol* pInstance = new CPistol(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPistol");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPistol::Clone(void* pArg)
{
	CPistol* pInstance = new CPistol(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPistol");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPistol::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	for (auto& State : m_pStates)
		Safe_Release(State);
}
