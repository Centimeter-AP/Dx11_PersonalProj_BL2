#include "Player.h"
#include "PlayerState.h"
#include "GameInstance.h"
#include "Camera.h"

#define PLAYER_DEFAULTSPEED 10.f


CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject { pDevice, pContext }
{

}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CGameObject ( Prototype )
{

}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	DESC			Desc{};

	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = PLAYER_DEFAULTSPEED;
	Desc.szName  = TEXT("Player");

	m_fSensor = XMConvertToRadians(4.f);

	m_pWeapons.resize(WTYPE_END);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects(pArg)))
		return E_FAIL;
	
	if (FAILED(Ready_PlayerStates()))
		return E_FAIL;

	//m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 20.f, 0.f, 1.f));

	m_pModelCom->Set_Animation(AR_Idle, true, 0.2f);
	Set_State(STATE_Idle);


	m_pModelCom->Play_Animation(0.01f);

	m_iCameraBoneIdx = m_pModelCom->Find_BoneIndex("Camera");
	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Priority_Update(fTimeDelta);
	}
	m_pColliderCom->Set_ColliderColor(RGBA_GREEN);

#pragma region 애니메이션 테스트
	static _uint test = {};
	if (KEY_DOWN(DIK_Z))
	{
#ifdef _CONSOLE
		cout << "애니메이션 : " << test << endl;
#endif
		test > PLA_ALL_END - 1 ? test = 0 : test++;
		m_pModelCom->Set_Animation(test, true, 0.2f);
	}
	if (KEY_DOWN(DIK_X))
	{
#ifdef _CONSOLE
		cout << "애니메이션 : " << test << endl;
#endif
		test < 1 ? test = PLA_ALL_END - 1 : test--;
		m_pModelCom->Set_Animation(test, true, 0.2f);
	}
#pragma endregion

	Update_State(fTimeDelta);
	Key_Input(fTimeDelta);
	Raycast_Object();
	//Spine3
}

EVENT CPlayer::Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Update(fTimeDelta);
	}

	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
	//auto val = m_pModelCom->Get_CombinedTransformationMatrix(m_pModelCom->Find_BoneIndex("Camera"))



	return EVN_NONE;
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Late_Update(fTimeDelta);
	}
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CPlayer::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMesh; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", static_cast<_uint>(i), aiTextureType_DIFFUSE, 0)))
			continue;
		//m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0);
		m_pModelCom->Bind_Bone_Matrices(m_pShaderCom, "g_BoneMatrices", static_cast<_uint>(i));
		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(static_cast<_uint>(i))))
			return E_FAIL;
	}
	
#ifdef _DEBUG
	
	m_pColliderCom->Render();





#endif 

	return S_OK;
}

void CPlayer::On_Collision(_uint iColID)
{
	m_pColliderCom->Set_ColliderColor(RGBA_RED);

}

void CPlayer::Key_Input(_float fTimeDelta)
{
	_long			MouseMove = {};

	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM::X))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMove * fTimeDelta * m_fSensor);
	}

	_float fDeltaPitch = {};
	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM::Y))
	{
		m_fPitch += MouseMove * fTimeDelta * m_fSensor;
		m_fPitch = CLAMP(m_fPitch, -XMConvertToRadians(28.f), XMConvertToRadians(28.f));

		fDeltaPitch = m_fPitch - m_fPreviousPitch;
		m_fPreviousPitch = m_fPitch;

		m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), fDeltaPitch);
	}
}

void CPlayer::Set_State(PLA_STATE eState)
{
	//( Set_State를 불렀을 때 ReEnter를 할 것인지? 이건 나한텐 필요 없을지도,,)
	// 아니 존나 필요해...
	m_ePrevState = m_eCurState;
	m_eCurState = eState;

	m_pCurState->Exit();
	m_pCurState = m_pStates[eState];
	m_pCurState->Enter();
}

void CPlayer::Update_State(_float fTimeDelta)
{
	//if (m_ePrevState != m_eCurState)
	//{
	//	m_pCurState->Enter();
	//	m_ePrevState = m_eCurState;
	//}
	m_pCurState->Execute(fTimeDelta);
}

void CPlayer::Raycast_Object()
{
	_matrix matFinal = XMLoadFloat4x4(m_pModelCom->Get_CombinedTransformationMatrix(m_iCameraBoneIdx)) * m_pTransformCom->Get_WorldMatrix();

	_vector vEye = XMVectorSetW(matFinal.r[3], 1.f);
	_vector vLook = XMVectorSetW(XMVector4Normalize(matFinal.r[0]), 0.f);
	_float Dist = {};
	m_pCurPickedCollider = m_pGameInstance->Raycast(vEye, vLook, 500.f, ENUM_CLASS(COL_GROUP::MONSTER), Dist);
	//if (m_pCurPickedCollider != nullptr)
	//	int a = 0;
}

HRESULT CPlayer::Ready_Components(void* pArg)
{
	CBounding_AABB::AABB_DESC AABBDesc = {};
	AABBDesc.pOwner = this;
	AABBDesc.eType = COLLIDER::AABB;
	AABBDesc.iColliderGroup = ENUM_CLASS(COL_GROUP::PLAYER);

	AABBDesc.iColliderID = ENUM_CLASS(COL_ID::PLAYER_BODY);
	AABBDesc.vExtents = _float3(0.3f, 0.8f, 0.3f);
	AABBDesc.vCenter = _float3(0.0f, AABBDesc.vExtents.y, 0.f);
	/* For.Com_Collider */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Siren_Hand"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	m_pModelCom->Add_Animations(R"(../Bin/Resources/Models/Bin_Anim/1st_Person_Pistol.anim)");
	m_pModelCom->Add_Animations(R"(../Bin/Resources/Models/Bin_Anim/1st_Person_Unarmed.anim)");
	m_pModelCom->Add_Animations(R"(../Bin/Resources/Models/Bin_Anim/Siren_1st.anim)");
	m_pModelCom->Add_Animations(R"(../Bin/Resources/Models/Bin_Anim/Base_Siren_All.anim)");

	// need: collider, sound, maybe gravity?

	m_pColliderCom->Set_ColliderColor(RGBA_GREEN);


	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects(void* pArg)
{



	return Ready_Weapons(pArg);
}

HRESULT CPlayer::Ready_Weapons(void* pArg)
{
	CGameObject::DESC Desc = {};

	Desc.fRotationPerSec = XMConvertToRadians(180.f);
	Desc.fSpeedPerSec = 10.f;
	Desc.pParentMatrix = &m_pTransformCom->Get_WorldMatrix4x4Ref();
	Desc.pParentObject = this;

	/* For.PartObject_Player_Weapon_Pistol */
	Desc.strVIBufferTag = TEXT("Prototype_Component_Model_Pistol");
	Desc.szName = L"Weapon_Pistol";
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("PartObject_Player_Weapon_Pistol"), TEXT("Prototype_GameObject_Pistol"), &Desc)))
		return E_FAIL;

	/* For.PartObject_Player_Weapon_AssaultRifle */
	Desc.strVIBufferTag = TEXT("Prototype_Component_Model_AR");
	Desc.szName = L"Weapon_AR";
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("PartObject_Player_Weapon_AR"), TEXT("Prototype_GameObject_AssaultRifle"), &Desc)))
		return E_FAIL;


	Find_PartObject(L"PartObject_Player_Weapon_Pistol")->Set_Active(true);
	Find_PartObject(L"PartObject_Player_Weapon_AR")->Set_Active(false);
	m_ePrevWeapon = m_eCurWeapon = WTYPE_PISTOL;
	return S_OK;
}

HRESULT CPlayer::Ready_PlayerStates()
{
	m_pStates[PLA_STATE::STATE_Idle]				= new CPlayerState_Idle(this);
	m_pStates[PLA_STATE::STATE_Run]					= new CPlayerState_Run(this);
	m_pStates[PLA_STATE::STATE_Run_L]				= new CPlayerState_Run_L(this);
	m_pStates[PLA_STATE::STATE_Run_R]				= new CPlayerState_Run_R(this);
	m_pStates[PLA_STATE::STATE_Sprint]				= new CPlayerState_Sprint(this);
	m_pStates[PLA_STATE::STATE_Jump]				= new CPlayerState_Jump(this);
	m_pStates[PLA_STATE::STATE_Fire]				= new CPlayerState_Fire(this);
	m_pStates[PLA_STATE::STATE_Reload]				= new CPlayerState_Reload(this);
	m_pStates[PLA_STATE::STATE_CloseAttack]			= new CPlayerState_CloseAttack(this);
	m_pStates[PLA_STATE::STATE_HolsterWeapon]		= new CPlayerState_HolsterWeapon(this);
	m_pStates[PLA_STATE::STATE_DrawWeapon]			= new CPlayerState_DrawWeapon(this);
	m_pStates[PLA_STATE::STATE_ThrowGrenade]		= new CPlayerState_ThrowGrenade(this);
	m_pStates[PLA_STATE::STATE_Skill_PhaseLock]		= new CPlayerState_Skill_PhaseLock(this);

	m_pCurState = m_pStates[PLA_STATE::STATE_Idle];

	return S_OK;
}

HRESULT CPlayer::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_Light(0);

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;


	return S_OK;
}

void CPlayer::Change_Weapon(WEAPON_TYPE eWeaponType)
{
	if (m_eCurWeapon == eWeaponType)
		return;

	m_eCurWeapon = eWeaponType;
	Set_State(PLA_STATE::STATE_HolsterWeapon);
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);




	//for (auto& Weapon : m_pWeapons)
	//	Safe_Release(Weapon);

	for (auto& State : m_pStates)
		Safe_Release(State);

}
