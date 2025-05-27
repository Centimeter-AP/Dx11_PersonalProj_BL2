#include "Weapon.h"

#include "GameInstance.h"

CWeapon::CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CWeapon::CWeapon(const CWeapon& Prototype)
	: CGameObject( Prototype )
{

}

HRESULT CWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon::Initialize(void* pArg)
{
	GAMEOBJECT_DESC			Desc{};

	Desc.fRotationPerSec = 0.f;
	Desc.fSpeedPerSec = 0.f;
	lstrcpy(Desc.szName, TEXT("Weapon"));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_pPlayer = GET_PLAYER;
	m_pPlayerModel = dynamic_cast<CModel*>(m_pPlayer->Get_Component(L"Com_Model"));
	m_pPlayerTransform = m_pPlayer->Get_Transform();
	m_szPlayerCameraBoneName = "R_Weapon_Bone";

	return S_OK;
}

void CWeapon::Priority_Update(_float fTimeDelta)
{
	// 본 인덱스 얻기
	_int iWeaponBoneIndex = m_pPlayerModel->Find_BoneIndex(m_szPlayerCameraBoneName.c_str());
	if (iWeaponBoneIndex < 0) return ;

	_matrix WeaponBoneMatrix = XMLoadFloat4x4(m_pPlayerModel->Get_CombinedTransformationMatrix(iWeaponBoneIndex));
	_matrix matWorld = m_pPlayerTransform->Get_WorldMatrix();
	
	//for (size_t i = 0; i < 3; i++)
	//	WeaponBoneMatrix.r[i] = XMVector3Normalize(WeaponBoneMatrix.r[i]);	
	//for (size_t i = 0; i < 3; i++)
	//	matWorld.r[i] = XMVector3Normalize(matWorld.r[i]);

	// 월드 행렬 적용 (원래 셰이더에서 월드 곱해주기때문에 지금은 로컬상태임)
	_matrix matFinal = WeaponBoneMatrix * matWorld;


	m_pTransformCom->Set_Matrix(matFinal);

}

EVENT CWeapon::Update(_float fTimeDelta)
{

	if (true == m_pModelCom->Play_Animation(fTimeDelta))
		int a = 10;


	return EVN_NONE;
}

void CWeapon::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CWeapon::Render()
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

HRESULT CWeapon::Ready_Components(void* pArg)
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"), // 총 전용 셰이더로 바꾸던지 패스를 바꾸던지
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

void CWeapon::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
