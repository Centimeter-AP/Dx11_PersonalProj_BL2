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
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Main_Tool();

public:
	static CTerrainTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg = nullptr);
	virtual void Free() override;
};

NS_END