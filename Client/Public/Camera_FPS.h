#pragma once

#include "Client_Defines.h"
#include "Camera.h"

NS_BEGIN(Engine)
class CModel;
NS_END

NS_BEGIN(Client)

class CCamera_FPS final : public CCamera
{
public:
	typedef struct tagCameraFPSDesc : public CCamera::CAMERA_DESC
	{
		_float			fSensor;
		CGameObject*	pPlayer;
	}DESC;
private:
	CCamera_FPS(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_FPS(const CCamera_FPS& Prototype);
	virtual ~CCamera_FPS() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	
private:
	HRESULT Set_PlayerBone();
	void Mouse_Fix();


private:
	_float				m_fSensor{};

	class CGameObject*		m_pPlayer = { nullptr }; //		
	class CTransform*		m_pPlayerTransform = { nullptr };
	CModel*					m_pPlayerModel = { nullptr };
	string					m_szPlayerCameraBoneName;
	_float4x4*				m_pPlayerWorldMatrix = { nullptr };
	_float4x4*				m_pCameraBoneCombinedMatrix = { nullptr };
	_int					m_iBoneIndex = {};

public:
	static CCamera_FPS* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END