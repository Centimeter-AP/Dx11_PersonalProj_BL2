#pragma once
#include "ImGuiTool.h"

NS_BEGIN(Engine)
class CVIBuffer_Terrain;
class CCell;
class CShader;
NS_END

NS_BEGIN(Client)

class CNavigationTool final : public CImGuiTool
{
private:
	enum PICKTYPE { TYPE_TERRIAN, TYPE_MESH, TYPE_CELL, TYPE_END };
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
	HRESULT Render_Cells();

private:
	HRESULT Render_NavimeshTool();
	HRESULT Pick_Terrain();
	HRESULT Pick_Objects();
	HRESULT Pick_Cells();
	HRESULT Make_Cell();
	HRESULT Save_Navigation();
	HRESULT Load_Navigation();
	
	_float3 Snap_PickedPos(const _float3& vPos, _float fSnapRadius);

	void Key_Input();
	void EnsureClockwise(_float3& a, _float3& b, _float3& c);

private:
	CShader* m_pShader = { nullptr };


	_bool		m_bEnablePicking = { false };
	PICKTYPE	m_ePickingType = {};

	_float3		m_fPickedPos[3] = {};
	_uint		m_iCurCellIndex = {};
	_int		m_iSelectedCellIndex = { -1 };

	vector<CCell*>			m_Cells;
	vector<_float3>			m_Points;

	class CTerrain*		m_pCurTerrain = { nullptr };
	CVIBuffer_Terrain*	m_pCurTerrainBuffer = { nullptr };

	list<CGameObject*>* m_pObjectList = { nullptr };

public:
	static CNavigationTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END