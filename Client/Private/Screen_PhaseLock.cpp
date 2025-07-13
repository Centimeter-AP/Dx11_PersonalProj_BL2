#include "Screen_PhaseLock.h"
#include "GameInstance.h"

CScreen_PhaseLock::CScreen_PhaseLock(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CScreen_PhaseLock::CScreen_PhaseLock(const CScreen_PhaseLock& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CScreen_PhaseLock::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CScreen_PhaseLock::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    

    //m_pTransformCom->Get_Scaled();
    //m_pTransformCom->Scaling(m_pTransformCom->Get_Scaled().x * 1.15f, m_pTransformCom->Get_Scaled().y * 1.15f, m_pTransformCom->Get_Scaled().z * 1.15f);

    return S_OK;
}

void CScreen_PhaseLock::Priority_Update(_float fTimeDelta)
{
    if (m_isActive == false)
        return;
}

EVENT CScreen_PhaseLock::Update(_float fTimeDelta)
{
    m_fCoolDown += fTimeDelta;
    if (m_isActive == false)
        return EVN_NONE;

    m_fShowTicker += fTimeDelta;
    if (m_fShowTicker <= 0.1f)
    {
        m_vColorPercent = { 3.f, 0.f, 0.f, 1.f };
    }
    else
    {
        if (m_vColorPercent.y < 1.f ||
            m_vColorPercent.z < 1.f)
        {
            m_vColorPercent.x -= fTimeDelta * 6.f;
            m_vColorPercent.y += fTimeDelta * 4.f;
            m_vColorPercent.z += fTimeDelta * 4.f;
        }
        else
            m_vColorPercent = { 1.f, 1.f, 1.f, 1.f };
    }

    if (m_fShowTicker >= 1.f)
    {
		m_fOpacity -= fTimeDelta * 2.f;
    }

    return EVN_NONE;
}

void CScreen_PhaseLock::Late_Update(_float fTimeDelta)
{
    if (m_isActive == false)
        return;
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CScreen_PhaseLock::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(POSTEX_ALPHABLEND_AND_COLOR)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

void CScreen_PhaseLock::Show_Effect(_fvector vHitPos)
{
	if (m_fCoolDown < 0.5f)
		return;
    m_isActive = true;
    m_fShowTicker = 0.f;
    m_fCoolDown = 0.f;
    m_fOpacity = 1.f;;
    m_vColorPercent = { 3.f, 0.f, 0.f, 1.f };
}

HRESULT CScreen_PhaseLock::Ready_Components()
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
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Screen_Hit"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;
    

    return S_OK;
}

HRESULT CScreen_PhaseLock::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fOpacity", &m_fOpacity, sizeof(_float))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColorPercent, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", *m_pPlayerOnShield?1:0)))
        return E_FAIL;

    return S_OK;
}

CScreen_PhaseLock* CScreen_PhaseLock::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CScreen_PhaseLock* pInstance = new CScreen_PhaseLock(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CScreen_PhaseLock");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CScreen_PhaseLock::Clone(void* pArg)
{
    CScreen_PhaseLock* pInstance = new CScreen_PhaseLock(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CScreen_PhaseLock");
        Safe_Release(pInstance);
    }

    return pInstance;
}


void CScreen_PhaseLock::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
}
