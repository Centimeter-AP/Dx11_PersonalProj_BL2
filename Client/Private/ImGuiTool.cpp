#include "ImGuiTool.h"
#include "GameInstance.h"

CImGuiTool::CImGuiTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
}

CImGuiTool::CImGuiTool(const CImGuiTool& Prototype)
{
}

HRESULT CImGuiTool::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

void CImGuiTool::Priority_Update(_float fTimeDelta)
{
}

void CImGuiTool::Update(_float fTimeDelta)
{
}

void CImGuiTool::Late_Update(_float fTimeDelta)
{
}

HRESULT CImGuiTool::Render()
{
    return E_NOTIMPL;
}

void CImGuiTool::Free()
{
	__super::Free();
}
