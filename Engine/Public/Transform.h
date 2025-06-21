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
	void Scaling(_float fX = 1.f, _float fY = 1.f, _float fZ = 1.f);
	void Scaling(const _float3& vScale);
public:
	void Go_Straight(_float fTimeDelta, class CCollider* pMyCol, _uint iGroupID);
	void Go_Straight(_float fTimeDelta, class CNavigation* pNavigation = nullptr); 
	void Go_Backward(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Right(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Left(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Straight_Hover(_float fTimeDelta);
	void Go_Backward_Hover(_float fTimeDelta);
	void Go_Target(_fvector vTarget, _float fTimeDelta, _float fMinDistance, class CNavigation* pNavigation = nullptr);
	void Go_Dir(_fvector vDir, _float fTimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Dir_Lerp(_fvector vDir, _float fTimeDelta, _float fLerpSpeed, class CNavigation* pNavigation = nullptr);
	void Go_Up(_float fTimeDelta);
	void Go_Down(_float fTimeDelta);
	void Turn(_fvector vAxis, _float fTimeDelta);
	void Rotation(_fvector vAxis, _float fRadian);
	void Rotation(_float fX, _float fY, _float fZ);

public:
	void LookWithLook(_fvector vLook);
	void LookAt(_fvector vAt);
	void LookAtLerp(_fvector vAt, _float fTimeDelta, _float fLerpSpeed);
	

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