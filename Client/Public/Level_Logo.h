#pragma once

#include "Client_Defines.h"
#include "Level.h"

NS_BEGIN(Client)

class CLevel_Logo final : public CLevel
{
private:
	CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_BackGround(const _wstring strLayerTag);
	HRESULT Ready_Layer_StaticUI(const _wstring strLayerTag);


public:
	static CLevel_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END