#include "UI_EnemyHP.h"
#include "GameInstance.h"
#include "Monster.h"

CUI_EnemyHP::CUI_EnemyHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CUI_EnemyHP::CUI_EnemyHP(const CUI_EnemyHP& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CUI_EnemyHP::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_EnemyHP::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	_float3 vPos = {};
	XMStoreFloat3(&vPos, m_pTransformCom->Get_State(STATE::POSITION));
	XMStoreFloat4x4(&m_BarWorldMatrix, XMMatrixScaling(112.f, 24.f, 1.f) * XMMatrixTranslation(vPos.x, vPos.y, vPos.z));

	return S_OK;
}

void CUI_EnemyHP::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
	if (m_bRender)
	{
		m_fRenderTime += fTimeDelta;
		m_fOpacity = m_fRenderTime * 4.f;
		if (m_fOpacity >= 1.f)
		{
			m_fOpacity = 1.f;
		}
	}
	else
	{
		m_fRenderTime += fTimeDelta;
		m_fOpacity = 1.f - m_fRenderTime * 4.f;
		if (m_fOpacity <= 0.f)
		{
			m_fOpacity = 0.f;
			m_isActive = false;
		}
	}

	if (m_pOwner && *m_pHP == 0)
		Hide_UI();
}

EVENT CUI_EnemyHP::Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return EVN_NONE;

	return EVN_NONE;
}

void CUI_EnemyHP::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false || m_pOwner == nullptr)
		return ;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CUI_EnemyHP::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fOpacity", &m_fOpacity, sizeof(_float))))
		return E_FAIL;
	_float fPercentage = {};
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fPercentage", &fPercentage, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pTextureCom[0]->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(POSTEX_ALPHABLEND)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	/************************************************************************/
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_BarWorldMatrix)))
		return E_FAIL;
	fPercentage = 1.f - (*m_pHP / static_cast<_float>(*m_pMaxHP));
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fPercentage", &fPercentage, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pTextureCom[1]->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(POSTEX_UI_BLEND)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	_float fFontPosX = { g_iWinSizeX * 0.5f + m_BarWorldMatrix._41 - m_fSizeX * 0.5f };
	_float fFontPosY = { g_iWinSizeY * 0.5f - m_BarWorldMatrix._42 + 10.f };
	m_pGameInstance->Draw_Font(TEXT("Font_WillowBody"), m_strName.c_str(),
		_float2(fFontPosX, fFontPosY), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.4f);
	_wstring strLevel = to_wstring(m_iLevel);
	m_pGameInstance->Draw_Font(TEXT("Font_WillowBody"), strLevel.c_str(),
		_float2(fFontPosX - 20.f, fFontPosY - 7.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.55f);


	return S_OK;
}

void CUI_EnemyHP::Show_UI(CGameObject* pEnemy)
{
	if (pEnemy == nullptr)
	{
		m_pOwner = nullptr;
		return;
	}
	if (m_pOwner == nullptr)
	{
		m_fRenderTime = 0.f;
		m_fOpacity = 0.f;
	}
	m_isActive = true;
	m_bRender = true;
	m_pOwner = pEnemy;
	Set_EnemyStatus(pEnemy);
}

void CUI_EnemyHP::Set_EnemyStatus(CGameObject* pEnemy)
{
	CMonster* pMonster = static_cast<CMonster*>(pEnemy);
	m_strName = pMonster->Get_Name();
	m_pHP = pMonster->Get_HP_Ptr();
	m_pMaxHP = pMonster->Get_MaxHP_Ptr();
	m_iLevel = pMonster->Get_Level();
}

HRESULT CUI_EnemyHP::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_EnemyHP_Back"),
		TEXT("Com_Texture_EnemyHP_Bar"), reinterpret_cast<CComponent**>(&m_pTextureCom[0]))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Bar_EnemyHP"),
		TEXT("Com_Texture_EnemyHP"), reinterpret_cast<CComponent**>(&m_pTextureCom[1]))))
		return E_FAIL;


	return S_OK;
}

CUI_EnemyHP* CUI_EnemyHP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_EnemyHP* pInstance = new CUI_EnemyHP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_EnemyHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_EnemyHP::Clone(void* pArg)
{
	CUI_EnemyHP* pInstance = new CUI_EnemyHP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_EnemyHP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_EnemyHP::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom[0]);
	Safe_Release(m_pTextureCom[1]);
}
