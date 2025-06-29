#include "Gravity.h"
#include "Transform.h"
#include "Navigation.h"
#include <VIBuffer_Terrain.h>

CGravity::CGravity(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CGravity::CGravity(const CGravity& Prototype)
	: CComponent( Prototype )
	, m_fGravity {Prototype.m_fGravity}
	, m_fVerticalVelocity{ Prototype.m_fVerticalVelocity }
{
}

HRESULT CGravity::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CGravity::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	DESC* pDesc = static_cast<DESC*>(pArg);

	m_fGravity = pDesc->fGravity;
	m_pNavigationCom = pDesc->pOwnerNavigationCom;
	Safe_AddRef(m_pNavigationCom);
	m_pTransformCom = pDesc->pOwnerTransformCom;
	m_fJumpPower = pDesc->fJumpPower;
	m_fOffsetY = pDesc->fOffsetY;
    return S_OK;
}

void CGravity::Update(_float fTimeDelta)
{
	if (m_isGrounded)
		return;

	// 중력 속도 갱신
	m_fVerticalVelocity += m_fGravity * fTimeDelta;

	_vector vPos = m_pTransformCom->Get_State(STATE::POSITION);
	vPos.m128_f32[1] += m_fVerticalVelocity * fTimeDelta;
	m_pTransformCom->Set_State(STATE::POSITION, vPos);

	// 현재 위치의 NavMesh Y값 얻기
	_float3 vCurPos;
	XMStoreFloat3(&vCurPos, vPos);

	_float3 vNavPos = {};
	_vector vecNavPos = m_pNavigationCom->SetUp_Height(vPos, 0.f);
	XMStoreFloat3(&vNavPos, vecNavPos);

	_float fNavY = vNavPos.y;
	//if (m_pNavigationCom->Get_Height(vCurPos, fNavY))  // 예: NavMesh 높이 반환
	{
		if (vCurPos.y <= fNavY + m_fOffsetY)
		{
			vCurPos.y = fNavY + m_fOffsetY;
			m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetY(vecNavPos, fNavY + m_fOffsetY));

			m_fVerticalVelocity = 0.f;
			m_isGrounded = true;
		}
	}
}


void CGravity::Update_NoNav(_float fTimeDelta, CVIBuffer_Terrain* pTerrainBuffer)
{
	if (m_isGrounded)
		return;

	// 중력 속도 갱신
	m_fVerticalVelocity += m_fGravity * fTimeDelta;

	_vector vPos = m_pTransformCom->Get_State(STATE::POSITION);
	vPos.m128_f32[1] += m_fVerticalVelocity * fTimeDelta;
	m_pTransformCom->Set_State(STATE::POSITION, vPos);

	// 현재 위치의 NavMesh Y값 얻기
	_float3 vCurPos;
	XMStoreFloat3(&vCurPos, vPos);




	
	_float x = m_pTransformCom->Get_WorldMatrix4x4Ref()._41;
	_float z = m_pTransformCom->Get_WorldMatrix4x4Ref()._43;
	_float y = pTerrainBuffer->Get_Height(x, z);

	_float yOffset = 0.3f;
	XMVECTOR fixedPos = XMVectorSet(x, y + yOffset, z, 1.0f);
	m_pTransformCom->Set_State(STATE::POSITION, fixedPos);



	////
	_float3 vNavPos = {};
	_vector vecNavPos = m_pNavigationCom->SetUp_Height(vPos, 0.f);
	XMStoreFloat3(&vNavPos, vecNavPos);

	_float fNavY = vNavPos.y;
	//if (m_pNavigationCom->Get_Height(vCurPos, fNavY))  // 예: NavMesh 높이 반환
	{
		if (vCurPos.y <= fNavY + m_fOffsetY)
		{
			vCurPos.y = fNavY + m_fOffsetY;
			m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetY(vecNavPos, fNavY + m_fOffsetY));

			m_fVerticalVelocity = 0.f;
			m_isGrounded = true;
		}
	}
}

void CGravity::Jump()
{
	if (m_isGrounded)
	{
		m_fVerticalVelocity = m_fJumpPower;
		m_isGrounded = false;
	}
}
CGravity* CGravity::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGravity* pInstance = new CGravity(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGravity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CGravity::Clone(void* pArg)
{
	CGravity* pInstance = new CGravity(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGravity");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CGravity::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pTransformCom);
}
