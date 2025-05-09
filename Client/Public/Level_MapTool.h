#pragma once

#include "Client_Defines.h"
#include "Level.h"

NS_BEGIN(Client)

class CLevel_MapTool final : public CLevel
{
private:
	CLevel_MapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_MapTool() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	typedef struct tagIsMenuOpen {
		_bool	ShowTerrainMenu = false;
	}ISOPEN;

private:
	HRESULT Ready_ImGui();

private:
	HRESULT ImGui_Render();
	HRESULT ImGui_Docking_Settings();
	HRESULT Window_ObjectList();
	HRESULT Window_ObjectInspector();
	HRESULT Window_ResourceList();

private: /* ��� �� �� */
	HRESULT Toolbar_Menus();
	HRESULT Terrain_Tools(_bool* p_open);
	HRESULT Show_ExternWindows();


private:
	class CGameObject*	m_pCurSelectedObj;
	ISOPEN				m_tWindowData;

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Monster(const _wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Effect(const _wstring& strLayerTag);

public:
	static CLevel_MapTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END