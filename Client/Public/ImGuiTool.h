#pragma once

#include "Base.h"

NS_BEGIN(Client)

class CImGuiTool final : public CBase
{
private:
	CImGuiTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CImGuiTool(const CImGuiTool& Prototype);
	virtual ~CImGuiTool() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void Free() override;
};

NS_END