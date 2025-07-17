#include "DamageFont.h"
#include "GameInstance.h"

CDamageFont::CDamageFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CDamageFont::CDamageFont(const CDamageFont& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CDamageFont::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDamageFont::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_strDamage = pDesc->strDamage;
	m_vWorldPosition = pDesc->vWorldPosition;
	if (pDesc->bCrit)
		m_vWorldPosition.y += 30.f;
	m_vFontColor = pDesc->vFontColor;

	m_fVelocity = 5.f;
	m_fLifeTime = 1.f;
	m_fXMoveSpeed = m_pGameInstance->Compute_Random(-3.f, 3.f);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CDamageFont::Priority_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return;
}

EVENT CDamageFont::Update(_float fTimeDelta)
{
	if (m_bDead)
		return EVN_DEAD;

	if (m_isActive == false)
		return EVN_NONE;

	m_fAge += fTimeDelta;
	if (m_fAge >= m_fLifeTime)
	{
		m_bDead = true;
	}


	_vector vWorldPos = XMVectorSetW(XMLoadFloat3(&m_vWorldPosition), 1.f);

	_float fDist = XMVectorGetX(XMVector3Length(vWorldPos - XMLoadFloat4(m_pGameInstance->Get_CamPosition())));
	_float fDistScale = 10.f / fDist;
	// 위로 튀어오르는 오프셋: y = v0*t – ½*g*t²
	const _float fGravity = 9.8f;
	_float offsetY = m_fVelocity * m_fAge - 0.5f * fGravity * m_fAge * m_fAge;
	_float offsetX = m_fXMoveSpeed * m_fAge * m_fVelocity * 10.f;
	// 월드 위치에 오프셋 추가
	vWorldPos = XMVectorAdd(vWorldPos, XMVectorSet(0.f, offsetY, 0.f, 0.f));

	// 스크린 투사
	_vector vScreenPos = XMVector3Project(
		vWorldPos,
		0, 0,
		(_float)g_iWinSizeX, (_float)g_iWinSizeY,
		0, 1,
		m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ),
		m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW),
		XMMatrixIdentity()
	);

	XMStoreFloat2(&m_vFontPosition, vScreenPos);
	m_vFontPosition.x += offsetX;
	// 3‑2) 페이드 아웃 (age/lifetime 비율로 알파 조절)
	_float fTime = (m_fAge / m_fLifeTime);
	m_fFontAlpha = 1.0f - powf(fTime, 3.f);

	// 3‑3) 스케일 이징 (예: 시작엔 1.5배 → 끝엔 1.0배)
	m_fFontScale = (1.0f + 0.5f * m_fFontAlpha) * fDistScale;
	// 3‑4) 실제 그리기
	// MeasureString을 이용해 중앙 정렬
	_vector vFontWH = m_pGameInstance->MeasureFont(TEXT("Font_WillowBody"), m_strDamage.c_str());
	m_fFontOrigin = { vFontWH.m128_f32[0] * 0.5f, vFontWH.m128_f32[1] * 0.5f };


	return EVN_NONE;
}

void CDamageFont::Late_Update(_float fTimeDelta)
{
	if (m_isActive == false)
		return ;
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CDamageFont::Render()
{
	m_pGameInstance->Draw_Font(
		TEXT("Font_WillowBody"), 
		m_strDamage.c_str(),
		m_vFontPosition,
		XMVectorSet(m_vFontColor.x, m_vFontColor.y, m_vFontColor.z, m_fFontAlpha),
		0.f, 
		m_fFontOrigin,
		m_fFontScale);

	return S_OK;
}

HRESULT CDamageFont::Ready_Components()
{
	///* For.Com_Shader */
	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
	//	TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	//	return E_FAIL;

	///* For.Com_VIBuffer */
	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
	//	TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
	//	return E_FAIL;

	///* For.Com_Texture */
	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_UI_Aim"),
	//	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	//	return E_FAIL; 

	return S_OK;
}

CDamageFont* CDamageFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDamageFont* pInstance = new CDamageFont(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDamageFont");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDamageFont::Clone(void* pArg)
{
	CDamageFont* pInstance = new CDamageFont(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDamageFont");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDamageFont::Free()
{
	__super::Free();
	//Safe_Release(m_pVIBufferCom);
	//Safe_Release(m_pShaderCom);
	//Safe_Release(m_pTextureCom);
}
