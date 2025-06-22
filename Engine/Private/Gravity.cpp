#include "Gravity.h"
#include "Transform.h"
#include "Navigation.h"

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
	m_pTransformCom = pDesc->pOwnerTransformCom;
	m_fJumpPower = pDesc->fJumpPower;
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
		if (vCurPos.y <= fNavY + 3.f)
		{
			vCurPos.y = fNavY + 3.f;
			m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetY(vecNavPos, fNavY + 3.f));

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
