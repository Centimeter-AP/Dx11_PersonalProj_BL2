#include "Player.h"

#include "GameInstance.h"

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
	GAMEOBJECT_DESC			Desc{};

	Desc.fRotationPerSec = XMConvertToRadians(180.0f);
	Desc.fSpeedPerSec = PLAYER_DEFAULTSPEED;
	lstrcpy(Desc.szName, TEXT("Player"));

	m_fSensor = 0.1f;
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;
	
	m_pModelCom->Set_Animation(21, true, 0.2f);

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
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
	Key_Input(fTimeDelta);


}

EVENT CPlayer::Update(_float fTimeDelta)
{
	if (true == m_pModelCom->Play_Animation(fTimeDelta))
		m_pModelCom->Set_Animation(Idle, true, 0.2f);
	return EVN_NONE;
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CPlayer::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
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


	if (KEY_PRESSING(DIK_A))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
		m_pModelCom->Set_Animation(Run_L, true, 0.2f);
	}
	if (KEY_PRESSING(DIK_D))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
		m_pModelCom->Set_Animation(Run_R, true, 0.2f);
	}
	if (KEY_PRESSING(DIK_W))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
		m_pModelCom->Set_Animation(Run_F, true, 0.2f);
	}
	else
		m_isRunning = false;
	if (KEY_PRESSING(DIK_S))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
		m_pModelCom->Set_Animation(Run_F, true, 0.2f);
	}

	if (KEY_DOWN(DIK_LSHIFT))
	{
		m_pTransformCom->Set_SpeedPerSec(PLAYER_DEFAULTSPEED * 1.5f);
		m_isRunning = true;
	}

	if (m_isRunning)
		m_pModelCom->Set_Animation(Sprint, true, 0.2f);

	if (KEY_DOWN(DIK_V)) // 근접공격 인데 잠깐 총 바꾸는 모션으로 함
	{
		m_pModelCom->Set_Animation(Holster, false, 0.2f);
		m_isRunning = false;
	}
	if (KEY_DOWN(DIK_G)) // 수류탄
	{
		m_pModelCom->Set_Animation(Grenade_throw, false, 0.2f);
		m_isRunning = false;
	}
	if (KEY_DOWN(DIK_R)) // 재장전
	{
		m_pModelCom->Set_Animation(R_Vladof, false, 0.2f);
	}

	//if (KEY_DOWN(DIK_SPACE)) // 잠프
	//{
	//	m_pModelCom->Set_Animation(Jump_Start, false);
	//}











	_long			MouseMove = {};

	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM::X))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMove * fTimeDelta * m_fSensor);
	}

	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM::Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), MouseMove * fTimeDelta * m_fSensor);
	}

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
}
