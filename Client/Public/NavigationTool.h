#pragma once
#include "ImGuiTool.h"

NS_BEGIN(Engine)
class CVIBuffer_Terrain;
class CCell;
NS_END

NS_BEGIN(Client)

class CNavigationTool final : public CImGuiTool
{
private:
	CNavigationTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigationTool(const CNavigationTool& Prototype);
	virtual ~CNavigationTool() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Render_NavimeshTool();
	HRESULT Pick_Terrain();
	HRESULT Pick_Objects();
	HRESULT Make_Cell();
	HRESULT Save_Navigation();
	HRESULT Load_Navigation();

private:
	_bool	m_bEnablePicking = { false };
	_bool	m_isTerrainPicking = { true };

	_float3 m_fPickedPos[3] = {};
	_uint	m_iCurIndex = {};

	vector<CCell*>	m_Cells;
	vector<_float3>			m_Points;

	class CTerrain*		m_pCurTerrain = { nullptr };
	CVIBuffer_Terrain*	m_pCurTerrainBuffer = { nullptr };

	

public:
	static CNavigationTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END