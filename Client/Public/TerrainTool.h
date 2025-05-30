#pragma once
#include "ImGuiTool.h"

NS_BEGIN(Client)


class CTerrainTool final : public CImGuiTool
{
private:
	CTerrainTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrainTool(const CTerrainTool& Prototype);
	virtual ~CTerrainTool() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Render_TerrainTool();
	void Terrain_HeightEditor();

	void Terrain_Save();
	void Terrain_Load();

private:
	void Height_Update(_float fTimeDelta);

private:
	_int	m_iVerticesX = {100};
	_int	m_iVerticesZ = {100};

	_float3 m_fPickedPos = {};
	_float	m_fBrushRadius = {1.f};
	_float	m_fBrushPower = {5.f};

	_bool	m_bBrushEnable = {false};
	_bool	m_bEnablePicking = { true };

	_bool	m_bSave = { false };
	_bool	m_bLoad = { false };

public:
	static CTerrainTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

//class CTerrainBrush final : public CGameObject
//{
//private:
//	CTerrainBrush(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
//	CTerrainBrush(const CTerrainBrush& Prototype);
//	virtual ~CTerrainBrush() = default;
//
//public:
//	virtual HRESULT Initialize_Prototype();
//	virtual HRESULT Initialize(void* pArg);
//	virtual void Priority_Update(_float fTimeDelta);
//	virtual EVENT Update(_float fTimeDelta);
//	virtual void Late_Update(_float fTimeDelta);
//	virtual HRESULT Render();
//
//
//public:
//	static CTerrainBrush* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
//	virtual CGameObject* Clone(void* pArg) override;
//	virtual void Free() override;
//
//};


NS_END