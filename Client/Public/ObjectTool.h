#pragma once
#include "ImGuiTool.h"

NS_BEGIN(Client)


class CObjectTool final : public CImGuiTool
{
private:
	CObjectTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObjectTool(const CObjectTool& Prototype);
	virtual ~CObjectTool() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	void	Key_Input();

private:
	HRESULT Render_ObjectTool();
	HRESULT Guizmo_Tool();
	HRESULT Window_ObjectList();
	HRESULT Make_Player();
	HRESULT Make_Particles();

private:
	HRESULT Open_ModelDirectory(path& CurPath);

	HRESULT Autosave(_float fTimeDelta);

	HRESULT Open_SaveFile();
	HRESULT Save_Objects(path SavePath);

	HRESULT Open_LoadFile();
	HRESULT Load_Objects(path SavePath);

private:
	_bool	m_isObjectSnapToTerrain = { false };
	_float  m_fSnapOffset = {};

	//_uint	m_iMapObjectCnt = {};
	_uint   m_iSelectObjectIndex = {};
	CGameObject* m_pSelectedObj = { nullptr };
	_float3 m_fMeshPickedPosition = {};
	_float3 m_fTerrainPickedPosition = {};

	_bool					m_isGizmoEnable = { false };
	_bool					m_bGizmoLock = { false };
	ImGuizmo::OPERATION		m_eOperation = {ImGuizmo::TRANSLATE};
	ImGuizmo::MODE			m_eMode = { ImGuizmo::WORLD };

	vector<_wstring>		m_ModelNames;

	_bool					m_isAnim = { false };
	MODEL					m_eModelType = { MODEL::NONANIM };

	_bool					m_isPlayerExists = { false };

	_float					m_fAutosaveTimeAcc = {};

	/*******For Particles *******/
	enum PARTICLE_TYPE { PTYPE_SPREAD, PTYPE_DROP, PTYPE_SPDROP, PTYPE_END };
	_bool					m_isParticlePreview = { false };	
	PARTICLE_TYPE			m_eParticleType = PTYPE_END;
	_float3					m_vPivot = {0.f, 0.f, 0.f};
	_float2					m_vLifeTime = {2.f, 4.f};
	_float2					m_vSpeed = {5.f, 10.f};
	_bool					m_isLoop = { true };
	_int					m_iNumInstance = { 100 };
	_float3					m_vRange = { 10.f, 10.f, 30.f};
	_float2					m_vSize = {1.f, 2.f};
	_float3					m_vCenter = {0.f, 0.f, 0.f};
	class CToolParticle*	m_pToolParticle = { nullptr };

public:
	static CObjectTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END