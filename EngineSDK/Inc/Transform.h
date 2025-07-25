#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct tagTransformDesc
	{
		_float			fSpeedPerSec;
		_float			fRotationPerSec;

		_bool			bHasTransformPreset = { false };
		_float4x4		PresetMatrix = {};
	}TRANSFORM_DESC;

protected:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& Prototype);
	virtual ~CTransform() = default;

public:
	_vector Get_State(STATE eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[ENUM_CLASS(eState)];
	}

	_float3 Get_Scaled();

	_matrix Get_WorldMatrix_Inverse() {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}
	_matrix Get_WorldMatrix() {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}
	_float4x4& Get_WorldMatrix4x4Ref() {
		return m_WorldMatrix;
	}
	const _float4x4* Get_WorldMatrix4x4Ptr() {
		return &m_WorldMatrix;
	}

	void Set_State(STATE eState, _fvector vState)
	{
		XMStoreFloat4(reinterpret_cast<_float4*>(&m_WorldMatrix.m[ENUM_CLASS(eState)]), vState);
	}
	void Set_Matrix(_fmatrix vMatrix) {
		XMStoreFloat4x4(&m_WorldMatrix, vMatrix);
	}
	void Set_SpeedPerSec(const _float fSPS) {
		m_fSpeedPerSec = fSPS;
	}

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);


public:
	void Go_Straight(_float fTimeDelta, class CCollider* pMyCol, _uint iGroupID); //deprecated
	void Scaling(_float fX = 1.f, _float fY = 1.f, _float fZ = 1.f);
	void Scaling(const _float3& vScale);
	void Scaling_Ratio(_float fX = 1.f, _float fY = 1.f, _float fZ = 1.f);

public:
	void Go_Straight(_float fTimeDelta, class CNavigation* pNavigation = nullptr, _bool isGrounded = true); 
	void Go_Backward(_float fTimeDelta, class CNavigation* pNavigation = nullptr, _bool isGrounded = true);
	void Go_Right(_float fTimeDelta, class CNavigation* pNavigation = nullptr, _bool isGrounded = true);
	void Go_Left(_float fTimeDelta, class CNavigation* pNavigation = nullptr, _bool isGrounded = true);
	void Go_Straight_Hover(_float fTimeDelta, class CNavigation* pNavigation = nullptr, _bool isGrounded = true);
	void Go_Backward_Hover(_float fTimeDelta, class CNavigation* pNavigation = nullptr, _bool isGrounded = true);
	void Go_Target(_fvector vTarget, _float fTimeDelta, _float fMinDistance, class CNavigation* pNavigation = nullptr, _bool isGrounded = true);
	void Go_Dir(_fvector vDir, _float fTimeDelta, class CNavigation* pNavigation = nullptr, _bool isGrounded = true);
	void Go_Dir_Lerp(_fvector vDir, _float fTimeDelta, _float fLerpSpeed, class CNavigation* pNavigation = nullptr, _bool isGrounded = true);
	void Go_Up(_float fTimeDelta);
	void Go_Down(_float fTimeDelta);
	void Turn(_fvector vAxis, _float fTimeDelta);
	void Rotation(_fvector vAxis, _float fRadian);
	_matrix Rotation_Billboard(_fvector vAxis, _fmatrix SrcMatrix, _float fRadian);
	void Rotation(_float fX, _float fY, _float fZ);

public:
	void LookWithLook(_fvector vLook);
	void LookAt(_fvector vAt);
	void LookAt_NoY(_fvector vAt);
	void LookAtLerp(_fvector vAt, _float fTimeDelta, _float fLerpSpeed);
	void LookAtLerp_NoY(_fvector vAt, _float fTimeDelta, _float fLerpSpeed);

public:
	_matrix Billboard(); // 이건 내거 빌보드 후 반환
	_matrix Compute_Billboard(_fmatrix SrcMatrix); // 이건 매개변수 받아서 빌보드 후 반환
	_matrix Turn_Billboard(_fmatrix SrcMatrix, _float fTimeDelta);

public:
	_float Compute_Target_Look_Angle(_fvector vTargetPos);

public:
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName);

private:
	_float4x4				m_WorldMatrix = {};
	_float					m_fSpeedPerSec = {};
	_float					m_fRotationPerSec = {};


public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END