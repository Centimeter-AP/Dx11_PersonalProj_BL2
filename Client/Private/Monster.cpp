#include "Monster.h"

#include "GameInstance.h"
#include <Terrain.h>
#include <Player.h>
#include "Weapon.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject { pDevice, pContext }
{

}

CMonster::CMonster(const CMonster& Prototype)
	: CGameObject ( Prototype )
{

}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);
	m_iNavIndex = pDesc->iNavigationIndex;
	m_isActive = pDesc->bActive;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTarget = GET_PLAYER;

	Safe_AddRef(m_pTarget);
	//if (FAILED(Ready_Components(pArg)))
	//	return E_FAIL;
	//
	//m_pModelCom->Set_Animation(3, true);

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
#pragma region AnimationTests
	static _uint test = {};
	if (KEY_DOWN(DIK_LBRACKET))
	{
		test > 73 ? test = 0 : test++;
		m_pModelCom->Set_Animation(test, true);
	}
	if (KEY_DOWN(DIK_RBRACKET))
	{
		test < 1 ? test = 74 : test--;
		m_pModelCom->Set_Animation(test, true);
	}
#pragma endregion


}

EVENT CMonster::Update(_float fTimeDelta)
{
	if (m_bDead)
		return Death_Delay(fTimeDelta);


	return EVN_NONE;
}

void CMonster::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CMonster::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMesh; i++)
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

HRESULT CMonster::Ready_Components(void* pArg)
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	wstring tag = L"Mushroom";
	if (nullptr != pArg)
	{
		tag = static_cast<DESC*>(pArg)->strVIBufferTag;
	}

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::MAPTOOL), TEXT("Prototype_Component_Model_") + tag,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIGATION_DESC		NaviDesc{};
	NaviDesc.iIndex = m_iNavIndex; // 32
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NaviDesc)))
		return E_FAIL;


	/* For.Com_Gravity */
	CGravity::DESC	GravityDesc{};
	GravityDesc.fGravity = -40.f;
	GravityDesc.fJumpPower = 25.f;
	GravityDesc.pOwnerNavigationCom = m_pNavigationCom;
	GravityDesc.pOwnerTransformCom = m_pTransformCom;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Gravity"),
		TEXT("Com_Gravity"), reinterpret_cast<CComponent**>(&m_pGravityCom), &GravityDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Bind_ShaderResources()
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

EVENT CMonster::Death_Delay(_float fTimeDelta)
{
	m_fDeathDelay += fTimeDelta;

	if (m_fDeathDelay > m_fForceDeathDelay)
		return EVN_DEAD;
	else
		return EVN_NONE;
}

void CMonster::Ride_Terrain()
{
	auto pTerrain = dynamic_cast<CTerrain*>(m_pGameInstance->Find_Object(
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), 0));

	auto pVIBuffer = dynamic_cast<CVIBuffer_Terrain*>(pTerrain->Get_Component(TEXT("Com_VIBuffer")));
	_float x = m_pTransformCom->Get_WorldMatrix4x4Ref()._41;
	_float z = m_pTransformCom->Get_WorldMatrix4x4Ref()._43;
	_float y = pVIBuffer->Get_Height(x, z);

	_float yOffset = 0.3f;
	XMVECTOR fixedPos = XMVectorSet(x, y + yOffset, z, 1.0f);
	m_pTransformCom->Set_State(STATE::POSITION, fixedPos);
}

void CMonster::On_Collision(_uint iMyColID, _uint iHitColID, CCollider* pHitCol)
{
	COL_ID eHitColID = static_cast<COL_ID>(iHitColID);

	if (CI_PLWEAPON(eHitColID))
	{
		m_iHP -= static_cast<const CWeapon*>(static_cast<CPlayer*>(pHitCol->Get_Owner())->Get_CurWeapon())->Get_Damage();
		if (m_iHP <= 0)
		{
			m_iHP = 0;
			Set_State_Dead();
		}
	}
	//switch (iHitColID)
	//{
	//case ENUM_CLASS(COL_ID::PLAYER_WEAPON_AR):
	//	break;
	//default:
	//	break;
	//}
}

void CMonster::Initialize_BasicStatus(_int iLevel)
{
	m_iHP = 0;
	m_iMaxHP = 0;
	m_iDamage = 0;
	m_iDefense = 0;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pColliderHeadCom);
	Safe_Release(m_pTarget);
	Safe_Release(m_pGravityCom);
	Safe_Release(m_pNavigationCom);
}
