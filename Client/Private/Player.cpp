#include "Player.h"
#include "PlayerState.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Monster.h"
#include "Bullet.h"
#include "Terrain.h"
#include "UI_Aim.h"
#include "UI_Ammo.h"
#include "UI_HP.h"
#include "UI_Shield.h"
#include "UI_Phaselock.h"
#include "UI_Exp.h"
#include "UI_HPShieldPannel.h"
#include "UI_AmmoPannel.h"
#include "UI_EnemyHP.h"
#include "Screen_Hit.h"

constexpr _float PLAYER_DEFAULTSPEED = 10.f;
constexpr _float NONCOMBAT_TIMER = 10.f;


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
	DESC*	pDesc = static_cast<DESC*>(pArg);
	m_iNavIndex = pDesc->iNavigationIndex;

	m_fSensor = XMConvertToRadians(4.f);

	m_pWeapons.resize(WTYPE_END);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Initialize_BasicStatus();

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PlayerStates()))
		return E_FAIL;

	m_pModelCom->Set_Animation(AR_Idle, true, 0.2f);
	Set_State(STATE_Idle);

	m_pModelCom->Play_Animation(0.01f);

	m_iCameraBoneIdx = m_pModelCom->Find_BoneIndex("Camera");

	m_pTransformCom->Set_State(STATE::POSITION, m_pNavigationCom->Get_CurCenterPoint());

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Priority_Update(fTimeDelta);
	}
	m_pColliderCom->Set_ColliderColor(RGBA_GREEN);

	Raycast_Object();
	EnemyHPUI_Check();

	//Spine3
}

EVENT CPlayer::Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return EVN_NONE;
	Key_Input(fTimeDelta);
	Update_State(fTimeDelta);
	//Ride_Terrain();
	if (m_pGravityCom->Is_Grounded())
		m_pTransformCom->Set_State(Engine::STATE::POSITION, m_pNavigationCom->SetUp_Height(m_pTransformCom->Get_State(Engine::STATE::POSITION), 5.f));
	m_pGravityCom->Update(fTimeDelta);
	Check_Player_NoHitTime(fTimeDelta);
	Cooldown_Phaselock(fTimeDelta);

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
	/*if (m_isActive == false)
		return;*/
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Late_Update(fTimeDelta);
	}
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);

	if (m_pNavigationCom != nullptr)
		m_pGameInstance->Add_DebugComponent(m_pNavigationCom);
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
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS, 0)))
			continue;
		m_pModelCom->Bind_Bone_Matrices(m_pShaderCom, "g_BoneMatrices", static_cast<_uint>(i));
		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(static_cast<_uint>(i))))
			return E_FAIL;
	}
	
	if (m_pLastPickedCollider != m_pCurPickedCollider && m_pCurPickedCollider != nullptr)
	{
		_wstring strCurPickedName = m_pCurPickedCollider->Get_Owner()->Get_Name();
	}

#ifdef _DEBUG

	_wstring strTest;
	_wstring strCurPickedCollider;
	if (m_pCurPickedCollider != nullptr)
		strCurPickedCollider = m_pCurPickedCollider->Get_Owner()->Get_VIBufferTag();

	strTest = TEXT("PlayerVariables");
	strTest += TEXT("\nCurPickedCollider : ") + strCurPickedCollider;
	strTest += TEXT("\nCurPickedDistance : ") + to_wstring(m_fCurPickedDistance);
	strTest += TEXT("\nPlayer HP : ") + to_wstring(m_iHP) + L" / " + to_wstring(m_iMaxHP);
	m_pGameInstance->Draw_Font(TEXT("Font_WillowBody"), strTest.c_str(), _float2(400.f, 0.f), XMVectorSet(0.7f, 0.1f, 0.f, 1.f), 0.f, _float2(0.f, 0.f), 0.4f);

#endif 

	return S_OK;
}

void CPlayer::On_Collision(_uint iMyColID, _uint iHitColID, CCollider* pHitCol)
{
	COL_ID eHitColID = static_cast<COL_ID>(iHitColID);

	m_pColliderCom->Set_ColliderColor(RGBA_RED);
	if (CI_MONSTERATK(eHitColID))
	{
		CMonster* pHitOwner = static_cast<CMonster*>(pHitCol->Get_Owner());
		static_cast<CScreen_Hit*>(m_PartObjects.find(TEXT("PartObject_Player_UI_Screen_Hit"))->second)->Show_Effect(pHitOwner->Get_Transform()->Get_State(STATE::POSITION));
		if (pHitOwner->Is_FirstHit())
		{
			if (m_bShield)
			{
				m_fShield -= static_cast<_float>(pHitOwner->Get_Damage());
				if (m_fShield <= 0.f)
				{
					// ShieldBreak();  /* 쉴드 뽀가진 이펙트? */

					m_fShield = 0.f;
					m_bShield = false;
				}
			}
			else
			{
				m_iHP -= pHitOwner->Get_Damage();
			}
			pHitOwner->Set_FirstHit_False(); // true로 바꾸는 건 피격 당사자만
			m_fNoHitTimeTicker = 0.f;
		}
	}

	if (CI_MONSTERBULLET(eHitColID))
	{
		CBullet* pHitOwner = static_cast<CBullet*>(pHitCol->Get_Owner());
		static_cast<CScreen_Hit*>(m_PartObjects.find(TEXT("PartObject_Player_UI_Screen_Hit"))->second)->Show_Effect(pHitOwner->Get_Transform()->Get_State(STATE::POSITION));
		if (m_bShield)
		{
			m_fShield -= static_cast<_float>(pHitOwner->Get_Damage());
			if (m_fShield <= 0.f)
			{
				// ShieldBreak();  /* 쉴드 뽀가진 이펙트? */
				m_fShield = 0.f;
				m_bShield = false;
			}
		}
		else
		{
			m_iHP -= pHitOwner->Get_Damage();
		}
		pHitOwner->Set_Dead();
		m_fNoHitTimeTicker = 0.f;
	}

	//if (CI_MONSTER(eHitColID))
	//	;
	//if (m_iHP <= 0)
	//	;
}

void CPlayer::Key_Input(_float fTimeDelta)
{
	_long			MouseMove = {};

	if (KEY_PRESSING(DIK_C))
	{
		m_iHP--;
		m_fShield -= 8.f;
	}
	if (KEY_DOWN(DIK_X))
	{
		m_iHP = m_iMaxHP;
		m_fShield = m_fMaxShield;
	}
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
	m_pLastPickedCollider = m_pCurPickedCollider;
	m_pCurPickedCollider = m_pGameInstance->Raycast(vEye, vLook, 500.f, ENUM_CLASS(COL_GROUP::MONSTER), m_fCurPickedDistance);
	//m_pCurPickedCollider = m_pGameInstance->Raycast(vEye, vLook, 500.f, ENUM_CLASS(COL_GROUP::BOSS), m_fCurPickedDistance);

}

void CPlayer::EnemyHPUI_Check()
{
	if (m_pLastPickedCollider != m_pCurPickedCollider)
	{
		if (m_pCurPickedCollider != nullptr)
			static_cast<CUI_EnemyHP*>(m_PartObjects.find(TEXT("PartObject_Player_UI_EnemyHP"))->second)->Show_UI(m_pCurPickedCollider->Get_Owner());
		else
			static_cast<CUI_EnemyHP*>(m_PartObjects.find(TEXT("PartObject_Player_UI_EnemyHP"))->second)->Hide_UI();
	}
}

//void CPlayer::Ride_Terrain()
//{
//	auto pTerrain = dynamic_cast<CTerrain*>(m_pGameInstance->Find_Object(
//		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), 0));
//
//	auto pVIBuffer = dynamic_cast<CVIBuffer_Terrain*>(pTerrain->Get_Component(TEXT("Com_VIBuffer")));
//	_float x = m_pTransformCom->Get_WorldMatrix4x4Ref()._41;
//	_float z = m_pTransformCom->Get_WorldMatrix4x4Ref()._43;
//	_float y = pVIBuffer->Get_Height(x, z);
//
//	_float yOffset = 0.5f;
//	XMVECTOR fixedPos = XMVectorSet(x, y + yOffset, z, 1.0f);
//	//cout << y << endl;
//	m_pTransformCom->Set_State(STATE::POSITION, fixedPos);
//}

HRESULT CPlayer::Ready_Components(void* pArg)
{
	CBounding_AABB::AABB_DESC AABBDesc = {};
	AABBDesc.pOwner = this;
	AABBDesc.eType = COLLIDER::AABB;
	AABBDesc.iColliderGroup = ENUM_CLASS(COL_GROUP::PLAYER);

	AABBDesc.iColliderID = ENUM_CLASS(COL_ID::PLAYER_BODY);
	AABBDesc.vExtents = _float3(0.4f, 1.5f, 0.4f);
	AABBDesc.vCenter = _float3(0.0f, AABBDesc.vExtents.y - 3.f, 0.f);
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

	/* For.Com_Navigation */
	CNavigation::NAVIGATION_DESC		NaviDesc{};
	NaviDesc.iIndex = m_iNavIndex; // 32
	if (FAILED(__super::Add_Component(m_iLevelID, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;

	/* For.Com_Gravity */
	CGravity::DESC	GravityDesc{};
	GravityDesc.fGravity = -40.f;
	GravityDesc.fJumpPower = 20.f;
	GravityDesc.pOwnerNavigationCom = m_pNavigationCom;
	GravityDesc.pOwnerTransformCom = m_pTransformCom;
	GravityDesc.fOffsetY = 5.f;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Gravity"),
		TEXT("Com_Gravity"), reinterpret_cast<CComponent**>(&m_pGravityCom), &GravityDesc)))
		return E_FAIL;



	m_pModelCom->Add_Animations(R"(../Bin/Resources/Models/Bin_Anim/1st_Person_Pistol.anim)");
	m_pModelCom->Add_Animations(R"(../Bin/Resources/Models/Bin_Anim/1st_Person_Unarmed.anim)");
	m_pModelCom->Add_Animations(R"(../Bin/Resources/Models/Bin_Anim/Siren_1st.anim)");
	m_pModelCom->Add_Animations(R"(../Bin/Resources/Models/Bin_Anim/Base_Siren_All.anim)");


	m_pColliderCom->Set_ColliderColor(RGBA_GREEN);

	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects(void* pArg)
{
	if (FAILED(Ready_Weapons(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Effects(pArg)))
		return E_FAIL;

	if (FAILED(Ready_UIObjects(pArg)))
		return E_FAIL;

	return S_OK;
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

HRESULT CPlayer::Ready_UIObjects(void* pArg)
{
	CUI_Aim::DESC			UIAimDesc;
	UIAimDesc.fX = g_iWinSizeX * 0.5f;
	UIAimDesc.fY = g_iWinSizeY * 0.5f;
	UIAimDesc.fSizeX = 50.0f;
	UIAimDesc.fSizeY = 50.0f;
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("PartObject_Player_UI_Aim"), TEXT("Prototype_GameObject_UI_Aim"), &UIAimDesc)))
		return E_FAIL;

	CUI_HPShieldPannel::DESC			UIHPSHDesc;
	UIHPSHDesc.fX = 180.f;
	UIHPSHDesc.fY = g_iWinSizeY - 70.f;
	UIHPSHDesc.fSizeX = 1.2f;
	UIHPSHDesc.fSizeY = 1.2f;
	UIHPSHDesc.iMaxHP = &m_iMaxHP;
	UIHPSHDesc.iHP = &m_iHP;
	UIHPSHDesc.fMaxShield = &m_fMaxShield;
	UIHPSHDesc.fShield = &m_fShield;
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("PartObject_Player_UI_HPShieldPannel"), TEXT("Prototype_GameObject_UI_HPShieldPannel"), &UIHPSHDesc)))
		return E_FAIL;


	CUI_AmmoPannel::DESC			UIAmmoPDesc;
	UIAmmoPDesc.fX = g_iWinSizeX - 180.f;
	UIAmmoPDesc.fY = g_iWinSizeY - 70.f;
	UIAmmoPDesc.fSizeX = 1.15f;
	UIAmmoPDesc.fSizeY = 1.15f;
	UIAmmoPDesc.iARAmmo = static_cast<CAssaultRifle*>(m_PartObjects.find(TEXT("PartObject_Player_Weapon_AR"))->second)->Get_CurAmmoLeft_Ptr();
	UIAmmoPDesc.iARMaxAmmo = &m_iAmmo[WTYPE_AR];
	UIAmmoPDesc.iPstAmmo = static_cast<CPistol*>(m_PartObjects.find(TEXT("PartObject_Player_Weapon_Pistol"))->second)->Get_CurAmmoLeft_Ptr();
	UIAmmoPDesc.iPstMaxAmmo = &m_iAmmo[WTYPE_PISTOL];

	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("PartObject_Player_UI_AmmoPannel"), TEXT("Prototype_GameObject_UI_AmmoPannel"), &UIAmmoPDesc)))
		return E_FAIL;

	CUI_Exp::DESC			UIExpDesc;
	UIExpDesc.fX = g_iWinSizeX * 0.5f;// 640 640
	UIExpDesc.fY = g_iWinSizeY - 80.f;
	UIExpDesc.fSizeX = 380.f;
	UIExpDesc.fSizeY = 27.55f;
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("PartObject_Player_UI_Exp"), TEXT("Prototype_GameObject_UI_Exp"), &UIExpDesc)))
		return E_FAIL;

	CUI_Phaselock::DESC			UIPSDesc;
	UIPSDesc.fX = g_iWinSizeX * 0.5f;
	UIPSDesc.fY = g_iWinSizeY - 160.f;
	UIPSDesc.fSizeX = 439.f;
	UIPSDesc.fSizeY = 125.f;
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("PartObject_Player_UI_Phaselock"), TEXT("Prototype_GameObject_UI_Phaselock"), &UIPSDesc)))
		return E_FAIL;

	m_PartObjects.find(TEXT("PartObject_Player_UI_Phaselock"))->second->Set_Active(false);

	CUI_EnemyHP::DESC			UIEnemyHPDesc;
	UIEnemyHPDesc.fX = g_iWinSizeX * 0.5f; // 0.72%
	UIEnemyHPDesc.fY = g_iWinSizeY * 0.35f;
	UIEnemyHPDesc.fSizeX = 148.f;
	UIEnemyHPDesc.fSizeY = 24.f;
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("PartObject_Player_UI_EnemyHP"), TEXT("Prototype_GameObject_UI_EnemyHP"), &UIEnemyHPDesc)))
		return E_FAIL;

	m_PartObjects.find(TEXT("PartObject_Player_UI_EnemyHP"))->second->Set_Active(false);

	CScreen_Hit::DESC			ScreenHitDesc;
	ScreenHitDesc.fX = g_iWinSizeX * 0.5f; // 0.72%
	ScreenHitDesc.fY = g_iWinSizeY * 0.5f;
	ScreenHitDesc.fSizeX = 514.f;
	ScreenHitDesc.fSizeY = 131.f;
	ScreenHitDesc.pPlayerOnShield = &m_bShield;
	ScreenHitDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix4x4Ptr();
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("PartObject_Player_UI_Screen_Hit"), TEXT("Prototype_GameObject_UI_Screen_Hit"), &ScreenHitDesc)))
		return E_FAIL;

	m_PartObjects.find(TEXT("PartObject_Player_UI_Screen_Hit"))->second->Set_Active(false);


	return S_OK;
}

HRESULT CPlayer::Ready_Effects(void* pArg)
{
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
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
	//	return E_FAIL;

	//const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_Light(0);

	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CPlayer::Change_Level(_uint iLevelIndex)
{
	if (m_iLevelID == iLevelIndex)
		return E_FAIL;
	m_iLevelID = iLevelIndex;

	for (auto& pState : m_pStates)
	{
		if (pState)
			pState->Delete_CurNavigation();
	}
	if (m_pGravityCom != nullptr)
	{
		auto iter = m_Components.find(TEXT("Com_Gravity"));
		if (iter != m_Components.end())
		{
			Safe_Release(iter->second);
			m_Components.erase(iter);
		}
		Safe_Release(m_pGravityCom);
		m_pGravityCom = nullptr;
	}

	if (m_pNavigationCom != nullptr)
	{
		auto iter = m_Components.find(TEXT("Com_Navigation"));
		if (iter != m_Components.end())
		{
			Safe_Release(iter->second);
			m_Components.erase(iter);
		}
		Safe_Release(m_pNavigationCom);
		m_pNavigationCom = nullptr;
	}
	/* For.Com_Navigation */
	CNavigation::NAVIGATION_DESC		NaviDesc{};
	switch (m_iLevelID)
	{
	case ENUM_CLASS(LEVEL::GAMEPLAY):
		NaviDesc.iIndex = 32;
		break;
	case ENUM_CLASS(LEVEL::BOSS):
		NaviDesc.iIndex = 32;
		break;
	default:
		break;
	}
	if (FAILED(__super::Add_Component(m_iLevelID, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;

	/* For.Com_Gravity */
	CGravity::DESC	GravityDesc{};
	GravityDesc.fGravity = -40.f;
	GravityDesc.fJumpPower = 20.f;
	GravityDesc.pOwnerNavigationCom = m_pNavigationCom;
	GravityDesc.pOwnerTransformCom = m_pTransformCom;
	GravityDesc.fOffsetY = 5.f;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Gravity"),
		TEXT("Com_Gravity"), reinterpret_cast<CComponent**>(&m_pGravityCom), &GravityDesc)))
		return E_FAIL;
	for (auto& pState : m_pStates)
	{
		if (pState)
			pState->Replace_Navigation(m_pNavigationCom);
	}

	m_pTransformCom->Set_State(STATE::POSITION, m_pNavigationCom->Get_CurCenterPoint());

	m_pGameInstance->Add_Collider(ENUM_CLASS(COL_GROUP::PLAYER), m_pColliderCom);
}

void CPlayer::Skill_Cooldowns(_float fTimeDelta)
{
	if (false == m_bPhaselockAble)
	{
		m_fPhaselockCooldownTicker += fTimeDelta;
		if (m_fPhaselockCooldownTicker >= m_fPhaselockCooldown)
		{
			m_fPhaselockCooldownTicker = 0.f;
			m_bPhaselockAble = true;
		}
	}
}

const CGameObject* CPlayer::Get_CurWeapon()
{
	switch (m_eCurWeapon)
	{
	case CPlayer::WEAPON_TYPE::WTYPE_AR:
		return Find_PartObject(L"PartObject_Player_Weapon_AR");
	case CPlayer::WEAPON_TYPE::WTYPE_PISTOL:
		return Find_PartObject(L"PartObject_Player_Weapon_Pistol");
	default:
		break;
	}
	return nullptr;
}

void CPlayer::Check_Player_NoHitTime(_float fTimeDelta)
{
	m_fNoHitTimeTicker += fTimeDelta;
	if (m_fShield < m_fMaxShield && m_fNoHitTimeTicker >= m_fShieldRechargeDelay)
	{
		Recharge_Shield(fTimeDelta);
	}
	if (true == m_bCombat && m_fNoHitTimeTicker >= NONCOMBAT_TIMER)
		m_bCombat = false;
	if (m_fNoHitTimeTicker >= 1000.f)
		m_fNoHitTimeTicker = 0.f;
}

void CPlayer::Recharge_Shield(_float fTimeDelta)
{
	_float fRechargePerTick = m_iShieldRechargeRate * fTimeDelta;
	m_bShield = true;
	m_fShield += fRechargePerTick;

	if (m_fShield >= m_fMaxShield)
		m_fShield = m_fMaxShield;
}

void CPlayer::Initialize_BasicStatus()
{
	m_iLevel = { 1 };

	m_iMaxHP = m_iHP = { 90 };
	
	m_fMaxShield = m_fShield = {100.f};
	m_bShield = { true }; // 이걸따로두는게맞나?
	m_iShieldRechargeRate = {21};
	m_fShieldRechargeDelay = {3.3f};

	m_fPhaselockDuration = { 3.f };
	m_fPhaselockCooldown = { 5.f };
	m_fPhaselockCooldownTicker = {};
	m_fPhaselockUsableDistance = {100.f};

	m_iAmmo[WTYPE_AR] = 400;
	m_iAmmo[WTYPE_PISTOL] = 100;
}

void CPlayer::Cooldown_Phaselock(_float fTimeDelta)
{
	if (false == m_bPhaselockAble)
	{
		m_fPhaselockCooldownTicker += fTimeDelta;
		if (m_fPhaselockCooldownTicker >= m_fPhaselockCooldown)
		{
			m_bPhaselockAble = true;
			m_fPhaselockCooldownTicker = 0.f;
		}
	}
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
	Safe_Release(m_pGravityCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);

	//for (auto& Weapon : m_pWeapons)
	//	Safe_Release(Weapon);

	for (auto& State : m_pStates)
		Safe_Release(State);

}
