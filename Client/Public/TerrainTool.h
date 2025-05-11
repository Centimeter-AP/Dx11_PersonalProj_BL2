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

public:
	static CTerrainTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};
NS_END