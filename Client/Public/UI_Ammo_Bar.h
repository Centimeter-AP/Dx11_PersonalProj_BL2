#pragma once
#include "UIObject.h"
#include "Player.h"
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CUI_Ammo_Bar final : public CUIObject
{
public:
	typedef struct tagUIAimDesc : public CUIObject::UIOBJECT_DESC
	{
		_int* iPstAmmo = { nullptr };
		_int* iPstMaxAmmo = { nullptr };
		_int* iARAmmo = { nullptr };
		_int* iARMaxAmmo = { nullptr };
	}DESC;

private:
	CUI_Ammo_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Ammo_Bar(const CUI_Ammo_Bar& Prototype);
	virtual ~CUI_Ammo_Bar() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual EVENT Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	void Change_Weapon(CPlayer::WEAPON_TYPE eWeaponType) { m_eWeaponType = eWeaponType; }

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	//_int* m_iPstAmmo = { nullptr };
	//_int* m_iPstMaxAmmo = { nullptr };
	//_int* m_iARAmmo = { nullptr };
	//_int* m_iARMaxAmmo = { nullptr };

	CPlayer::WEAPON_TYPE m_eWeaponType = { CPlayer::WTYPE_PISTOL };
	_int* m_iAmmo[CPlayer::WTYPE_END] = { nullptr };
	_int* m_iMaxAmmo[CPlayer::WTYPE_END] = { nullptr };
	_int m_iMaxMagazine[CPlayer::WTYPE_END] = {};

	_float m_fCurPosX = {};
	_float m_fCurPosY = {};

private:
	HRESULT Ready_Components();

public:
	static CUI_Ammo_Bar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};
NS_END
