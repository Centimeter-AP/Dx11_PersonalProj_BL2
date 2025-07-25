#pragma once

#include "GameObject.h"

/* CUIObject_2D */
/* CUIObject_3D */

NS_BEGIN(Engine)

class ENGINE_DLL CUIObject abstract : public CGameObject
{
public:
	typedef struct tagUIObjectDesc : public CGameObject::DESC
	{
		_float			fX, fY, fSizeX, fSizeY;
		_uint			iUIDepth{};

	}UIOBJECT_DESC;
protected:
	CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual EVENT Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	_uint Get_UIDepth() const { return m_iUIDepth; }

protected:
	/* 뷰포트 상의 유아이의 중심위치 fX, fY, 사이즈 fSiuzeX, fSizeY */
	_float			m_fX{}, m_fY{}, m_fSizeX{}, m_fSizeY{};

	/* 직교 투영을 위한 행렬. */
	_float4x4		m_ViewMatrix{}, m_ProjMatrix{};
	_uint			m_iUIDepth{};	

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END