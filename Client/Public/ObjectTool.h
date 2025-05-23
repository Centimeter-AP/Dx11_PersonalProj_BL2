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
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Render_ObjectTool();
	HRESULT Guizmo_Tool();
	HRESULT Window_ObjectList();

private:
	HRESULT Open_FileDirectory(path& CurPath);
	HRESULT Ready_Prototype();
	HRESULT Ready_Object();

private:
	_bool	m_isObjectSnapToTerrain = { false };
	_float  m_fSnapOffset = {};

	//_uint	m_iMapObjectCnt = {};
	_uint   m_iSelectObjectIndex = {};
	CGameObject* m_pSelectedObj = { nullptr };
	_float3 m_fMeshPickedPosition = {};
	_float3 m_fTerrainPickedPosition = {};

	_bool					m_isGizmoEnable = { false };
	ImGuizmo::OPERATION		m_eOperation = {ImGuizmo::TRANSLATE};
	ImGuizmo::MODE			m_eMode = { ImGuizmo::WORLD };

	vector<_wstring>		m_ModelNames;

	_bool					m_isAnim = { false };
	MODEL					m_eModelType = { MODEL::NONANIM };

public:
	static CObjectTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END