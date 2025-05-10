#pragma once

#include "Base.h"
#include "Client_Defines.h"


using namespace ImGui;
NS_BEGIN(Client)

class CImGuiTool abstract : public CBase
{
protected:
	CImGuiTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CImGuiTool(const CImGuiTool& Prototype);
	virtual ~CImGuiTool() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	ISOPEN*				m_tWindowData;


public:
	virtual void Free() override;
};

NS_END