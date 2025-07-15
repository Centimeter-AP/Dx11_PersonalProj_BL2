#pragma once

#include "Client_Defines.h"
#include "SpriteEffect.h"


NS_BEGIN(Client)

class CPhaselockSwirl final : public CSpriteEffect
{
public:
	typedef struct tagPhaselockSphereDesc : public CSpriteEffect::DESC
	{
		_float fLastTime = {};
	}DESC;
private:
	CPhaselockSwirl(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPhaselockSwirl(const CPhaselockSwirl& Prototype);
	virtual ~CPhaselockSwirl() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

	void Show_Effect();

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	

private:
	CTexture*	m_pTextureMaskCom = { nullptr };
	CTexture*	m_pTextureSwirlCom = { nullptr };
	CTexture*	m_pTextureCom = { nullptr };
	_float		m_fTimeAcc = { 0.f };
	_float		m_fLastTime = { 0.f };
	const _float4x4* m_pSocketMatrix = { nullptr };
	_float		m_fAngle = { 0.f };

public:
	static CPhaselockSwirl* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END