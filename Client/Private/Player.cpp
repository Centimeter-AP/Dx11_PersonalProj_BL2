#include "Player.h"

#include "GameInstance.h"
#include <Camera.h>

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
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects(pArg)))
		return E_FAIL;
	
	if (FAILED(Ready_PlayerStates()))
		return E_FAIL;

	//m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, 20.f, 0.f, 1.f));
	
	m_pModelCom->Set_Animation(Idle, true, 0.2f);

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Priority_Update(fTimeDelta);
	}
	static _uint test = {};
	if (KEY_DOWN(DIK_Z))
	{
		test > 36 ? test = 0 : test++;
		m_pModelCom->Set_Animation(test, true, 0.2f);
	}
	if (KEY_DOWN(DIK_X))
	{
		test < 1 ? test = 37 : test--;
		m_pModelCom->Set_Animation(test, true, 0.2f);
	}
	Update_State(fTimeDelta);
	Key_Input(fTimeDelta);
	//Spine3
}

EVENT CPlayer::Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject.second)
			pPartObject.second->Update(fTimeDelta);
	}
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
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
			continue;
		//m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0);
		m_pModelCom->Bind_Bone_Matrices(m_pShaderCom, "g_BoneMatrices", i);
		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}
	


	return S_OK;
}

void CPlayer::Key_Input(_float fTimeDelta)
{
	/*if (!static_cast<CCamera*>(m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::MAPTOOL), L"Layer_Camera", 0))->Is_Using())
	{
		if (!m_isPlayingNonLoopAnim)
			m_pModelCom->Set_Animation(ENUM_CLASS(PLA_AR::Idle), true, 0.1f);
		if (KEY_PRESSING(DIK_A))
		{
			m_pTransformCom->Go_Left(fTimeDelta);
			m_pModelCom->Set_Animation(ENUM_CLASS(PLA_AR::Run_L), true, 0.1f);
		}
		if (KEY_PRESSING(DIK_D))
		{
			m_pTransformCom->Go_Right(fTimeDelta);
			m_pModelCom->Set_Animation(ENUM_CLASS(PLA_AR::Run_R), true, 0.1f);
		}
		if (KEY_PRESSING(DIK_W))
		{
			m_pTransformCom->Go_Straight(fTimeDelta);
			m_pModelCom->Set_Animation(ENUM_CLASS(PLA_AR::Run_F), true, 0.1f);
		}
		else
			m_isRunning = false;
		if (KEY_PRESSING(DIK_S))
		{
			m_pTransformCom->Go_Backward(fTimeDelta);
			m_pModelCom->Set_Animation(ENUM_CLASS(PLA_AR::Run_F), true, 0.1f);
		}

		if (KEY_DOWN(DIK_LSHIFT))
		{
			m_pTransformCom->Set_SpeedPerSec(PLAYER_DEFAULTSPEED * 1.5f);
			m_isRunning = true;
		}

		if (m_isRunning)
			m_pModelCom->Set_Animation(ENUM_CLASS(PLA_AR::Sprint), true, 0.1f);



	}*/

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
#ifdef _CONSOLE
	static _float consoleTicker = {};
	consoleTicker += fTimeDelta;
	if (consoleTicker >= 0.3f)
	{
		cout << "m_fPitch : " << m_fPitch << endl;
		cout << "m_fPreviousPitch : " << m_fPreviousPitch << endl;
		cout << "fDeltaPitch : " << fDeltaPitch << endl; 
		cout << endl;
		consoleTicker = 0.f;
	}
#endif // _CONSOLE



	//if (KEY_DOWN(DIK_V)) // 근접공격 인데 잠깐 총 바꾸는 모션으로 함
	//{
	//	m_pModelCom->Set_Animation(Holster, false, 0.1f);
	//	m_isPlayingNonLoopAnim = true;
	//	m_isRunning = false;
	//}
	//if (KEY_DOWN(DIK_G)) // 수류탄
	//{
	//	m_pModelCom->Set_Animation(Grenade_throw, false, 0.1f);
	//	m_isPlayingNonLoopAnim = true;
	//	m_isRunning = false;
	//}
	//if (KEY_DOWN(DIK_R)) // 재장전
	//{
	//	m_pModelCom->Set_Animation(R_Jakobs, false, 0.1f);
	//	m_isPlayingNonLoopAnim = true;
	//}

	//if (KEY_DOWN(DIK_SPACE)) // 잠프z
	//{
	//	m_pModelCom->Set_Animation(Jump_Start, false);
	//	m_isJumping = true;
	//	m_isPlayingNonLoopAnim = true;
	//}


}

void CPlayer::Set_State(PLA_STATE eState)
{
	//( Set_State를 불렀을 때 ReEnter를 할 것인지? 이건 나한텐 필요 없을지도,,)
	m_ePrevState = m_eCurState;
	m_eCurState = eState;

	m_pCurState->Exit();
	m_pCurState = m_pStates[eState];
}

void CPlayer::Update_State(_float fTimeDelta)
{
	if (m_ePrevState != m_eCurState)
	{
		m_pCurState->Enter();
		m_ePrevState = m_eCurState;
	}
	m_pCurState->Execute(fTimeDelta);
}

HRESULT CPlayer::Ready_Components(void* pArg)
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Model_Siren_Hand"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	// need: collider, ...


	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects(void* pArg)
{
	CGameObject::DESC Desc = {};

	Desc.fRotationPerSec = XMConvertToRadians(180.f);
	Desc.fSpeedPerSec = 10.f;
	Desc.pParentMatrix = &m_pTransformCom->Get_WorldMatrix4x4Ref();
	Desc.pParentObject = this;

	/* For.PartObject_Player_Weapon_AssaultRifle */
	Desc.strVIBufferTag = TEXT("Prototype_Component_Model_AR");
	Desc.szName = L"Weapon_AR";
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("PartObject_Player_Weapon_AssaultRifle"), TEXT("Prototype_GameObject_AssaultRifle"), &Desc)))
		return E_FAIL;

	///* For.PartObject_Player_Weapon_Pistol */
	//Desc.strVIBufferTag = TEXT("Prototype_Component_Model_Pistol");
	//lstrcpy(Desc.szName, L"Weapon_Pistol");
	//if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("PartObject_Player_Weapon_Pistol"), TEXT("Prototype_GameObject_Pistol"), &Desc)))
	//	return E_FAIL;

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
	m_pStates[PLA_STATE::STATE_ChangeWeapon]		= new CPlayerState_ChangeWeapon(this);
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
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);


	for (auto State : m_pStates)
		Safe_Delete(State);

}
//홍동완 신승훈 왔다감