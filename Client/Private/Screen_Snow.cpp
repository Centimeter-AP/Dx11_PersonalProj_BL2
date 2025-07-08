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

    m_iTileX = 4;
    m_iTileY = 4; 
    m_fTileSize.x = 1.0f / _float(m_iTileX);
    m_fTileSize.y = 1.0f / _float(m_iTileY);
    
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

    if (!m_bDrop)
    {
        m_fCoolDown += fTimeDelta;
        if (m_fCoolDown >= 1.f + rand() % 2)
        {
            m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(static_cast<_float>(rand() % g_iWinSizeX - g_iWinSizeX * 0.5f), static_cast<_float>(rand() % g_iWinSizeY - g_iWinSizeY * 0.5f), 0.5f, 1.f));
            m_bDrop = true;
            m_fCoolDown = 0.f;
            m_fOpacity = 1.f;
            m_fDropTicker = 0.f;

            m_iTileIdx = rand() % (m_iTileX * m_iTileY);
            m_fOffset.x = (m_iTileIdx % m_iTileX) * m_fTileSize.x;
            m_fOffset.y = (m_iTileIdx / m_iTileX) * m_fTileSize.y;
        }
    }
    else
    {
        m_fDropTicker += fTimeDelta;
        if (m_fDropTicker >= 0.8f)
        {
            m_fOpacity -= fTimeDelta * 4.f;
            if (m_fOpacity <= 0.f)
            {
                m_fOpacity = 0.f;
                m_bDrop = false;
            }
        }
    }

    return EVN_NONE;
}

void CScreen_Snow::Late_Update(_float fTimeDelta)
{
    if (m_isActive == false)
        return;
    if (m_bDrop)
        m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CScreen_Snow::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(POSTEX_GRID_PICK)))
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
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ScreenSnow"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;
    
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_ScreenSnowMask"),
        TEXT("Com_TextureMask"), reinterpret_cast<CComponent**>(&m_pTextureMaskCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CScreen_Snow::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fTileSize", &m_fTileSize, sizeof(_float2))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_fTileOffset", &m_fOffset, sizeof(_float2))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_fOpacity", &m_fOpacity, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;
    if (FAILED(m_pTextureMaskCom->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", 0)))
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
    Safe_Release(m_pTextureMaskCom);
}
