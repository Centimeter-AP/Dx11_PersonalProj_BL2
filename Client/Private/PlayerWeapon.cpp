#include "PlayerWeapon.h"

#include "GameInstance.h"

CPlayerWeapon::CPlayerWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CPlayerWeapon::CPlayerWeapon(const CPlayerWeapon& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CPlayerWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerWeapon::Initialize(void* pArg)
{
	DESC* Desc = static_cast<DESC*>(pArg);
	lstrcpy(Desc->szName, TEXT("Weapon"));

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_pParentObject = GET_PLAYER;
	//m_pPlayerModel = dynamic_cast<CModel*>(m_pPlayer->Get_Component(L"Com_Model"));
	//m_pPlayerTransform = m_pPlayer->Get_Transform();
	m_szPlayerCameraBoneName = "R_Weapon_Bone";

	return S_OK;
}

void CPlayerWeapon::Priority_Update(_float fTimeDelta)
{
	// �� �ε��� ���
	_int iWeaponBoneIndex = m_pPlayerModel->Find_BoneIndex(m_szPlayerCameraBoneName.c_str());
	if (iWeaponBoneIndex < 0) return;
	// �÷��̾� ���� weapon �� �ε��� ����� 

	_matrix WeaponBoneMatrix = XMLoadFloat4x4(m_pPlayerModel->Get_CombinedTransformationMatrix(iWeaponBoneIndex));
	// weapon ���� combinedtransformationmatrix�����
	_matrix matWorld = /*m_pPlayerTransform->Get_WorldMatrix()*/{};

	// ���� ��� ���� (���� ���̴����� ���� �����ֱ⶧���� ������ ���û�����)
	_matrix matFinal = WeaponBoneMatrix * matWorld;

	m_pTransformCom->Set_Matrix(matFinal);
	m_pTransformCom->Go_Backward(fTimeDelta * 10.f);
}

EVENT CPlayerWeapon::Update(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_R)) // ������
	{
		m_pModelCom->Set_Animation(14, false, 0.1f);
	}
	if (true == m_pModelCom->Play_Animation(fTimeDelta))
		int a = 10;


	return EVN_NONE;
}

void CPlayerWeapon::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CPlayerWeapon::Render()
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

HRESULT CPlayerWeapon::Ready_Components(void* pArg)
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"), // �� ���� ���̴��� �ٲٴ��� �н��� �ٲٴ���
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

void CPlayerWeapon::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
