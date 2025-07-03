#include "Screen_Snow.h"
#include "GameInstance.h"

CScreen_Snow::CScreen_Snow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CScreen_Snow::CScreen_Snow(const CScreen_Snow& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CScreen_Snow::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CScreen_Snow::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CScreen_Snow::Priority_Update(_float fTimeDelta)
{
    if (m_isActive == false)
        return;
}

EVENT CScreen_Snow::Update(_float fTimeDelta)
{
    if (m_isActive == false)
        return EVN_NONE;

    return EVN_NONE;
}

void CScreen_Snow::Late_Update(_float fTimeDelta)
{
    if (m_isActive == false)
        return;
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CScreen_Snow::Render()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(1)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CScreen_Snow::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Aim"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

CScreen_Snow* CScreen_Snow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CScreen_Snow* pInstance = new CScreen_Snow(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CScreen_Snow");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CScreen_Snow::Clone(void* pArg)
{
    CScreen_Snow* pInstance = new CScreen_Snow(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CScreen_Snow");
        Safe_Release(pInstance);
    }

    return pInstance;
}


void CScreen_Snow::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
}
