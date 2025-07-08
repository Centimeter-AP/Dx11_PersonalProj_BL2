#include "Transform.h"

#include "Shader.h"
#include "Collider.h"
#include "GameInstance.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent { pDevice, pContext }
{
}

CTransform::CTransform(const CTransform& Prototype)
	: CComponent( Prototype )
	, m_WorldMatrix { Prototype.m_WorldMatrix }
{
}

_float3 CTransform::Get_Scaled()
{	
	return _float3(XMVectorGetX(XMVector3Length(Get_State(STATE::RIGHT))),
		XMVectorGetX(XMVector3Length(Get_State(STATE::UP))),
		XMVectorGetX(XMVector3Length(Get_State(STATE::LOOK))));		
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);

	m_fSpeedPerSec = pDesc->fSpeedPerSec;
	m_fRotationPerSec = pDesc->fRotationPerSec;

	if (pDesc->bHasTransformPreset)
	{
		Set_Matrix(XMLoadFloat4x4(&pDesc->PresetMatrix));
	}

	return S_OK;
}

void CTransform::Scaling(_float fX, _float fY, _float fZ)
{
	Set_State(STATE::RIGHT, XMVector3Normalize(Get_State(STATE::RIGHT)) * fX);
	Set_State(STATE::UP, XMVector3Normalize(Get_State(STATE::UP)) * fY);
	Set_State(STATE::LOOK, XMVector3Normalize(Get_State(STATE::LOOK)) * fZ);
}

void CTransform::Scaling(const _float3& vScale)
{
	Set_State(STATE::RIGHT, XMVector3Normalize(Get_State(STATE::RIGHT)) * vScale.x);
	Set_State(STATE::UP, XMVector3Normalize(Get_State(STATE::UP)) * vScale.y);
	Set_State(STATE::LOOK, XMVector3Normalize(Get_State(STATE::LOOK)) * vScale.z);
}

void CTransform::Go_Straight(_float fTimeDelta, CCollider* pMyCol, _uint iGroupID)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vLook = Get_State(STATE::LOOK);
	
	_vector vMove = XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;
	_float3 vDesiredMove = {};
	XMStoreFloat3(&vDesiredMove, vMove);


	_float3 res = m_pGameInstance->Resolve_Slide_AABB(pMyCol, vDesiredMove, iGroupID);

	vPosition += XMLoadFloat3(&res);

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Straight(_float fTimeDelta, CNavigation* pNavigation, _bool isGrounded)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vLook = Get_State(STATE::LOOK);

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;
	

	if (nullptr == pNavigation || true == pNavigation->isMove(vPosition, isGrounded))
		Set_State(STATE::POSITION, vPosition);

}

void CTransform::Go_Backward(_float fTimeDelta, CNavigation* pNavigation, _bool isGrounded)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vLook = Get_State(STATE::LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;


	if (nullptr == pNavigation || true == pNavigation->isMove(vPosition, isGrounded))
		Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta, CNavigation* pNavigation, _bool isGrounded)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vRight = Get_State(STATE::RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;


	if (nullptr == pNavigation || true == pNavigation->isMove(vPosition, isGrounded))
		Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta, CNavigation* pNavigation, _bool isGrounded)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vRight = Get_State(STATE::RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;


	if (nullptr == pNavigation || true == pNavigation->isMove(vPosition, isGrounded))
		Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Straight_Hover(_float fTimeDelta, CNavigation* pNavigation, _bool isGrounded)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vLook = Get_State(STATE::LOOK);
	vLook = XMVectorSetY(vLook, 0.0f);
	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation || true == pNavigation->isMove(vPosition, isGrounded))
		Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Backward_Hover(_float fTimeDelta, CNavigation* pNavigation, _bool isGrounded)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vLook = Get_State(STATE::LOOK);
	vLook = XMVectorSetY(vLook, 0.0f);
	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation || true == pNavigation->isMove(vPosition, isGrounded))
		Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Target(_fvector vTarget, _float fTimeDelta, _float fMinDistance, CNavigation* pNavigation, _bool isGrounded)
{
	_vector		vMoveDir = vTarget - Get_State(STATE::POSITION);

	if (fMinDistance <= XMVectorGetX(XMVector3Length(vMoveDir)))
	{
		auto pos = Get_State(STATE::POSITION) + XMVector3Normalize(vMoveDir) * m_fSpeedPerSec * fTimeDelta;
		if (nullptr == pNavigation || true == pNavigation->isMove(pos, isGrounded))
			Set_State(STATE::POSITION, pos);
	}
}

void CTransform::Go_Dir(_fvector vDir, _float fTimeDelta, CNavigation* pNavigation, _bool isGrounded)
{
	// 1. 방향 정규화
    _vector vNormDir = XMVector3Normalize(vDir);

    // 2. 현재 위치 가져오기
    _vector vPosition = Get_State(STATE::POSITION);

    // 3. 속도 * 시간 계산 후 위치 갱신
    vPosition += vNormDir * (m_fSpeedPerSec * fTimeDelta);

    // 4. WorldMatrix 에 반영
    Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Dir_Lerp(_fvector vDir, _float fTimeDelta, _float fLerpSpeed, CNavigation* pNavigation, _bool isGrounded)
{
	// 1. 현재 LOOK 방향
	_vector vCurrentLook = XMVector3Normalize(Get_State(STATE::LOOK));

	// 2. 목표 방향 정규화
	_vector vTargetDir = XMVector3Normalize(vDir);

	// 3. Lerp 로 부드럽게 보간
	_vector vBlendedDir = XMVector3Normalize(
		XMVectorLerp(vCurrentLook, vTargetDir, fTimeDelta * fLerpSpeed)
	);

	// 4. 새로운 LOOK/RIGHT/UP basis 설정
	LookWithLook(vBlendedDir);

	// 5. 보간된 방향으로 실제 이동
	Go_Dir(vBlendedDir, fTimeDelta);
}

void CTransform::Go_Up(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	vPosition += XMVector3Normalize(vUp) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Down(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	vPosition -= XMVector3Normalize(vUp) * m_fSpeedPerSec * fTimeDelta * 1.12f;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);

	Set_State(STATE::RIGHT, XMVector4Transform(Get_State(STATE::RIGHT), RotationMatrix));
	Set_State(STATE::UP, XMVector4Transform(Get_State(STATE::UP), RotationMatrix));
	Set_State(STATE::LOOK, XMVector4Transform(Get_State(STATE::LOOK), RotationMatrix));
}

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_float3		vScaled = Get_Scaled();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);	

	Set_State(STATE::RIGHT, XMVector4Transform(vRight, RotationMatrix));
	Set_State(STATE::UP, XMVector4Transform(vUp, RotationMatrix));
	Set_State(STATE::LOOK, XMVector4Transform(vLook, RotationMatrix));
}

void CTransform::Rotation(_float fX, _float fY, _float fZ)
{
	_float3		vScaled = Get_Scaled();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

	_vector		vQuaternion = XMQuaternionRotationRollPitchYaw(fX, fY, fZ);

	_matrix		RotationMatrix = XMMatrixRotationQuaternion(vQuaternion);

	Set_State(STATE::RIGHT, XMVector4Transform(vRight, RotationMatrix));
	Set_State(STATE::UP, XMVector4Transform(vUp, RotationMatrix));
	Set_State(STATE::LOOK, XMVector4Transform(vLook, RotationMatrix));
}

void CTransform::LookWithLook(_fvector vLook)
{
	_float3		vScaled = Get_Scaled();

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE::RIGHT, XMVector3Normalize(vRight) * vScaled.x);
	Set_State(STATE::UP, XMVector3Normalize(vUp) * vScaled.y);
	Set_State(STATE::LOOK, XMVector3Normalize(vLook) * vScaled.z);
}

void CTransform::LookAt(_fvector vAt)
{
	_float3		vScaled = Get_Scaled();

	_vector		vLook = vAt - Get_State(STATE::POSITION);
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE::RIGHT, XMVector3Normalize(vRight) * vScaled.x);
	Set_State(STATE::UP, XMVector3Normalize(vUp) * vScaled.y);
	Set_State(STATE::LOOK, XMVector3Normalize(vLook) * vScaled.z);
}

void CTransform::LookAt_NoY(_fvector vAt)
{
	_float4	vCustomAt = {};
	XMStoreFloat4(&vCustomAt, vAt);
	vCustomAt.y = m_WorldMatrix.m[ENUM_CLASS(STATE::POSITION)][1];
	vCustomAt.w = 1.f;
	_vector vResAt = XMLoadFloat4(&vCustomAt);

	_float3		vScaled = Get_Scaled();

	_vector		vLook = vResAt - Get_State(STATE::POSITION);
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE::RIGHT, XMVector3Normalize(vRight) * vScaled.x);
	Set_State(STATE::UP, XMVector3Normalize(vUp) * vScaled.y);
	Set_State(STATE::LOOK, XMVector3Normalize(vLook) * vScaled.z);
}

void CTransform::LookAtLerp(_fvector vAt, _float fTimeDelta, _float fLerpSpeed)
{
	_float3        vScaled = Get_Scaled();
	_vector        vPos = Get_State(STATE::POSITION);
	_vector        vLook = Get_State(STATE::LOOK);

	_vector        vTargetDir = XMVector3Normalize(vAt - vPos);

	/* Lerp 방식 보간 */
	_vector vInterpolatedDir = XMVector3Normalize(
		XMVectorLerp(vLook, vTargetDir, fTimeDelta * fLerpSpeed)
	);

	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE::RIGHT, XMVector3Normalize(vRight) * vScaled.x);
	Set_State(STATE::UP, XMVector3Normalize(vUp) * vScaled.y);
	Set_State(STATE::LOOK, XMVector3Normalize(vInterpolatedDir) * vScaled.z);

}

void CTransform::LookAtLerp_NoY(_fvector vAt, _float fTimeDelta, _float fLerpSpeed)
{
	_float4	vCustomAt = {};
	XMStoreFloat4(&vCustomAt, vAt);
	vCustomAt.y = m_WorldMatrix.m[ENUM_CLASS(STATE::POSITION)][1];
	vCustomAt.w = 1.f;
	_vector vResAt = XMLoadFloat4(&vCustomAt);

	_float3         vScaled = Get_Scaled();
	_vector         vPos = Get_State(STATE::POSITION);
	_vector			vLook = Get_State(STATE::LOOK);

	_vector        vTargetDir = XMVector3Normalize(vResAt - vPos);

	/* Lerp 방식 보간 */
	_vector vInterpolatedDir = XMVector3Normalize(
		XMVectorLerp(vLook, vTargetDir, fTimeDelta * fLerpSpeed)
	);

	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE::RIGHT, XMVector3Normalize(vRight) * vScaled.x);
	Set_State(STATE::UP, XMVector3Normalize(vUp) * vScaled.y);
	Set_State(STATE::LOOK, XMVector3Normalize(vInterpolatedDir) * vScaled.z);

}

_matrix CTransform::Billboard() 
{
	_float4x4 m_Return = {};
	XMStoreFloat4x4(&m_Return, XMMatrixIdentity());
	_float3	vScaled = Get_Scaled();
	_vector	vPosition = Get_State(STATE::POSITION);

	_matrix matCamWorld = m_pGameInstance->Get_Transform_Matrix_Inv(D3DTS::VIEW);
	_float4 vCameraPos = *m_pGameInstance->Get_CamPosition();
	
	_vector vRight = matCamWorld.r[0]; // X axis
	_vector vUp = matCamWorld.r[1]; // Y axis
	_vector vLook = matCamWorld.r[2]; // Z axis

	XMStoreFloat3((_float3*)&m_Return.m[0][0], XMVector3Normalize(vRight) * vScaled.x);
	XMStoreFloat3((_float3*)&m_Return.m[1][0], XMVector3Normalize(vUp) * vScaled.y);
	XMStoreFloat3((_float3*)&m_Return.m[2][0], XMVector3Normalize(vLook) * vScaled.z);
	XMStoreFloat3((_float3*)&m_Return.m[3][0], vPosition);
	m_Return._44 = 1.f;

	//_float3 vRight = reinterpret_cast<_float3*>(&matCamWorld.m[0][0])->Normalize() * vScaled.x;
	////_float3 vUp = reinterpret_cast<_float3*>(&matCamWorld.m[1][0])->Normalize() * vScaled.y;
	//_float3 vLook = reinterpret_cast<_float3*>(&matCamWorld.m[2][0])->Normalize() * vScaled.z;
	
	return XMLoadFloat4x4(&m_Return);
}

_float CTransform::Compute_Target_Look_Angle(_fvector vTargetPos)
{
	_vector vPos = Get_State(STATE::POSITION);
	_vector vTargetDir = vTargetPos - vPos;
	vTargetDir.m128_f32[1] = 0.f;
	_vector vLook = Get_State(STATE::LOOK);
	vLook.m128_f32[1] = 0.f;

	_float dot = XMVectorGetX(XMVector3Dot(XMVector3Normalize(vLook), XMVector3Normalize(vTargetDir)));
	dot = clamp(dot, -1.0f, 1.0f); // 오차 방지

	return acosf(dot);
}


HRESULT CTransform::Bind_ShaderResource(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);	
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTransform* pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;

}

void CTransform::Free()
{
	__super::Free();

}
