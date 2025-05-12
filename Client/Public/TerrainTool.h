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
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Render_TerrainTool();

private:
	_int	m_iVerticesX = {};
	_int	m_iVerticesZ = {};

	_float3 m_bPickedPos = {};

	_bool	m_bBrushEnable = {false};

public:
	static CTerrainTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

class CTerrainBrush final : public CGameObject
{
private:
	CTerrainBrush(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrainBrush(const CTerrainBrush& Prototype);
	virtual ~CTerrainBrush() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_int	m_iVerticesX = {};
	_int	m_iVerticesZ = {};

	_float3 m_vPickedPos = {};
	_bool	m_bBrushEnable = { false };

public:
	static CTerrainBrush* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};


NS_END