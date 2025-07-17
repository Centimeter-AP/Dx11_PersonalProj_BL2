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
	HRESULT Initialize_Player(); // 임시 함수임 카메라매니저좀 만드삼
	
	void Start_Recoil();
	void Start_Shake();

private:
	HRESULT Set_PlayerBone();
	void Mouse_Fix();
	void Update_Camera(_float fTimeDelta);
	void Update_Recoil(_float fTimeDelta);

private:
	_float				m_fSensor{};

	class CGameObject*		m_pPlayer = { nullptr }; //		
	class CTransform*		m_pPlayerTransform = { nullptr };
	CModel*					m_pPlayerModel = { nullptr };
	string					m_szPlayerCameraBoneName;
	_float4x4*				m_pPlayerWorldMatrix = { nullptr };
	_float4x4*				m_pCameraBoneCombinedMatrix = { nullptr };
	_int					m_iBoneIndex = {};

private: // 반동용 변수
	// 반동 오프셋(월드 위치 보정용)
	_float3 m_vRecoilOffset = {};
	// 반동 복원 속도
	_float3 m_vRecoilVelocity = {};
	// 반동이 진행 중인지 플래그
	_bool   m_bRecoiling = false;

public:
	static CCamera_FPS* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END