#include "Screen_Hit.h"
#include "GameInstance.h"

CScreen_Hit::CScreen_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CScreen_Hit::CScreen_Hit(const CScreen_Hit& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CScreen_Hit::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CScreen_Hit::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

	m_pPlayerOnShield = pDesc->pPlayerOnShield;
    

    //m_pTransformCom->Get_Scaled();
    //m_pTransformCom->Scaling(m_pTransformCom->Get_Scaled().x * 1.15f, m_pTransformCom->Get_Scaled().y * 1.15f, m_pTransformCom->Get_Scaled().z * 1.15f);

    return S_OK;
}

void CScreen_Hit::Priority_Update(_float fTimeDelta)
{
    if (m_isActive == false)
        return;
}

EVENT CScreen_Hit::Update(_float fTimeDelta)
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

void CScreen_Hit::Late_Update(_float fTimeDelta)
{
    if (m_isActive == false)
        return;
    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CScreen_Hit::Render()
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

void CScreen_Hit::Show_Effect(_fvector vHitPos)
{
	if (m_fCoolDown < 0.5f)
		return;
    m_isActive = true;
    m_fShowTicker = 0.f;
    m_fCoolDown = 0.f;
    m_fOpacity = 1.f;;
    m_vColorPercent = { 3.f, 0.f, 0.f, 1.f };
    Calc_HitPos(vHitPos);
}

void CScreen_Hit::Calc_HitPos(_fvector vHitPos)
{
    _vector vDir = vHitPos - XMLoadFloat4x4(m_pParentMatrix).r[3];
	vDir.m128_f32[1] = 0.f;
	vDir = XMVector3Normalize(vDir);

    _vector vPlayerLook = XMLoadFloat4x4(m_pParentMatrix).r[2];
	vPlayerLook.m128_f32[1] = 0.f; 
	vPlayerLook = XMVector3Normalize(vPlayerLook); 


    _vector vPlayerRight = XMLoadFloat4x4(m_pParentMatrix).r[0];
	vPlayerRight.m128_f32[1] = 0.f;
	vPlayerRight = XMVector3Normalize(vPlayerRight); 


    _float fDotFront = XMVectorGetX(XMVector3Dot(vPlayerLook, vDir));   // 앞인지 뒤인지
	_float fDotRight = XMVectorGetX(XMVector3Dot(vPlayerRight, vDir));  // 오른쪽인지 왼쪽인지

    // cos(θ) 1일 때 같은 방향 벡터
	// -1일 때 반대 방향
    // 0일 때 직각
	// 0.707f는 cos(45도) 값이라고 하네요


    if (fDotFront > 0.707f)
        m_eHitPos = HITPOS_DOWN;
    else if (fDotFront < -0.707f)
        m_eHitPos = HITPOS_UP;
    else if (fDotRight > 0.0f)
        m_eHitPos = HITPOS_RIGHT;
    else
        m_eHitPos = HITPOS_LEFT;


    switch (m_eHitPos)
    {
    case HITPOS_UP:
        m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, m_fSizeY * 0.5f - g_iWinSizeY * 0.5f, 0.f, 1.f));
        m_pTransformCom->Rotation(0.f, 0.f, 0.f);
        break;
    case HITPOS_DOWN:
        m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.f, -m_fSizeY * 0.5f + g_iWinSizeY * 0.5f, 0.f, 1.f));
        m_pTransformCom->Rotation(0.f, 0.f, XMConvertToRadians(180.f));
        break;
    case HITPOS_LEFT:
        m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fSizeY * 0.5f - g_iWinSizeX * 0.5f, 0.f, 0.f, 1.f));
        m_pTransformCom->Rotation(0.f, 0.f, XMConvertToRadians(270.f));
        break;
    case HITPOS_RIGHT:
        m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-m_fSizeY * 0.5f + g_iWinSizeX * 0.5f, 0.f, 0.f, 1.f));
        m_pTransformCom->Rotation(0.f, 0.f, XMConvertToRadians(90.f));
        break;
    default:
        break;
    }

}

HRESULT CScreen_Hit::Ready_Components()
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

HRESULT CScreen_Hit::Bind_ShaderResources()
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

CScreen_Hit* CScreen_Hit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CScreen_Hit* pInstance = new CScreen_Hit(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CScreen_Hit");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CScreen_Hit::Clone(void* pArg)
{
    CScreen_Hit* pInstance = new CScreen_Hit(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CScreen_Hit");
        Safe_Release(pInstance);
    }

    return pInstance;
}


void CScreen_Hit::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
}
