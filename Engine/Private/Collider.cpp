#include "Collider.h"

#include "GameInstance.h"



CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent { pDevice, pContext }
{

}

CCollider::CCollider(const CCollider& Prototype)
	: CComponent( Prototype )
	, m_eType { Prototype.m_eType }
	, m_pBounding { Prototype.m_pBounding }
	, m_pBatch { Prototype.m_pBatch }
	, m_pEffect { Prototype.m_pEffect }
#ifdef _DEBUG
	, m_pInputLayout { Prototype.m_pInputLayout }
#endif
{
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif
}

HRESULT CCollider::Initialize_Prototype(COLLIDER eType)
{
	m_eType = eType;


#ifdef _DEBUG

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);

	const void* pShaderByteCode = { nullptr };
	size_t		iShaderByteCodeLength = {  };	

	m_pEffect->SetVertexColorEnabled(true);

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

#endif

	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	CBounding::BOUNDING_DESC* pDesc = static_cast<CBounding::BOUNDING_DESC*>(pArg);

	pDesc->eType = m_eType;
	m_pOwner = pDesc->pOwner;
	m_iColliderID = pDesc->iColliderID;
	if (m_pOwner == nullptr)
	{
		MSG_BOX("이러면안된다니까요");
		return E_FAIL;
	}

	switch (m_eType)
	{
	case COLLIDER::AABB:
		m_pBounding = CBounding_AABB::Create(m_pDevice, m_pContext, pDesc);
		break;
	case COLLIDER::OBB:
		m_pBounding = CBounding_OBB::Create(m_pDevice, m_pContext, pDesc);
		break;
	case COLLIDER::SPHERE:
		m_pBounding = CBounding_Sphere::Create(m_pDevice, m_pContext, pDesc);
		break;
	}

	m_pGameInstance->Add_Collider(pDesc->iColliderGroup, this); 
	return S_OK;
}

void CCollider::Update(_fmatrix WorldMatrix)
{
	if (false == m_isActive)
		return;
	m_pBounding->Update(WorldMatrix);
}

_bool CCollider::Intersect(CCollider* pTargetCollider)
{
	if (false == m_isActive) // 비활성화 중이면 무조건 충돌x 반환?
		return false;

	m_isColl = m_pBounding->Intersect(pTargetCollider->m_pBounding);

	return m_isColl;
}

_bool CCollider::Raycast(_fvector vRayOrigin, _fvector vRayDir, _float& fRayDist)
{
	if (false == m_isActive) // 비활성화 중이면 무조건 충돌x 반환?
		return false;

	return m_pBounding->Raycast(vRayOrigin, vRayDir, fRayDist);
}

const _float3 CCollider::Get_Pos()
{
	switch (m_eType)
	{
	case COLLIDER::AABB:
		return static_cast<CBounding_AABB*>(m_pBounding)->Get_Desc()->Center;
		break;
	case COLLIDER::OBB:
		return static_cast<CBounding_OBB*>(m_pBounding)->Get_Desc()->Center;
		break;
	case COLLIDER::SPHERE:
		return static_cast<CBounding_Sphere*>(m_pBounding)->Get_Desc()->Center;
		break;
	default:
		return _float3(0.f, 0.f, 0.f);
	}
}

const _float CCollider::Get_MaxLength()
{
	switch (m_eType)
	{
	case COLLIDER::AABB:
		return XMVectorGetX(XMVector3Length(XMLoadFloat3(&static_cast<CBounding_AABB*>(m_pBounding)->Get_Desc()->Extents)));
		break;
	case COLLIDER::OBB:
		return XMVectorGetX(XMVector3Length(XMLoadFloat3(&static_cast<CBounding_OBB*>(m_pBounding)->Get_Desc()->Extents)));
		break;
	case COLLIDER::SPHERE:
		return static_cast<CBounding_Sphere*>(m_pBounding)->Get_Desc()->Radius;
		break;
	default:
		return 0.f;
	}
}

const _float3 CCollider::ComputeCollisionNormal_AABB(CCollider* pDest)
{
	return static_cast<CBounding_AABB*>(m_pBounding)->ComputeCollisionNormal(static_cast<CBounding_AABB*>(pDest->m_pBounding));
}

const _float3 CCollider::Get_Penetrated()
{
	return m_pBounding->Get_Penetrated();
}

#ifdef _DEBUG

_bool CCollider::bColliderDraw = false;

HRESULT CCollider::Render()
{
	if (false == m_isActive)
		return S_OK;

	if (bColliderDraw == true)
	{
		m_pEffect->SetWorld(XMMatrixIdentity());
		m_pEffect->SetView(m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW));
		m_pEffect->SetProjection(m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ));

		m_pContext->IASetInputLayout(m_pInputLayout);

		m_pEffect->Apply(m_pContext);

		m_pBatch->Begin();

		m_pBounding->Render(m_pBatch, m_vRenderColor);

		m_pBatch->End();
	}

	return S_OK;
}
#endif

CCollider* CCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDER eType)
{
	CCollider* pInstance = new CCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType)))
	{
		MSG_BOX("Failed to Created : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider::Clone(void* pArg)
{
	CCollider* pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

	Safe_Release(m_pBounding);

#ifdef _DEBUG
	Safe_Release(m_pInputLayout);
	if (false == m_isCloned)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}
#endif
}
