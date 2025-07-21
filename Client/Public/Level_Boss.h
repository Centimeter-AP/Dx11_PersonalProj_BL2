#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "GameInstance.h"

NS_BEGIN(Client)

class CLevel_Boss final : public CLevel
{
private:
	CLevel_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Boss() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_BackGround(const _wstring strLayerTag);
	HRESULT Ready_Layer_Sky(const _wstring strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring strLayerTag);
	HRESULT Ready_Layer_Monster(const _wstring strLayerTag);
	HRESULT Ready_Layer_Boss(const _wstring strLayerTag);
	HRESULT Ready_Layer_UI(const _wstring strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring strLayerTag);
	HRESULT Ready_Layer_MapObject(const _wstring strLayerTag);
	HRESULT Ready_Lights();
	HRESULT Ready_Sound();

	void Intersect();
	
	void Key_Input();

private:
	CSound_Core* m_pBGM = { nullptr };
	CSound_Core* m_pEnv[3] = { nullptr };

public:
	static CLevel_Boss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END