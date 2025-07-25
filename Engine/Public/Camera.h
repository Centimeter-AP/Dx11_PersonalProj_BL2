#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc : public CGameObject::DESC
	{
		_float3			vEye;
		_float3			vAt;
		_float			fFov, fNear, fFar;
	}CAMERA_DESC;
protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	_float				m_fFov = { };
	_float				m_fNear = { };
	_float				m_fFar = { };
	_float				m_fAspect = {};	

	_bool				m_isUsing = { false };

protected:
	void Bind_Matrices();

public:
	void	Set_Using(_bool isUsing) { m_isUsing = isUsing; }
	_bool	Is_Using() const { return m_isUsing; }

public:
	virtual CGameObject* Clone(void* pArg) = 0;	
	virtual void Free() override;

};

NS_END