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

private:
	HRESULT Render_ObjectTool();

private:
	HRESULT Ready_Prototype();
	HRESULT Ready_Object();

private:
	_bool	m_isGizmoEnable = { false };
	_bool	m_isObjectSnapToTerrain = { false };
	_float  m_fSnapOffset = {};

	ImGuizmo::OPERATION		m_eOperation = {ImGuizmo::TRANSLATE};
	ImGuizmo::MODE			m_eMode = { ImGuizmo::WORLD };

public:
	static CObjectTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END