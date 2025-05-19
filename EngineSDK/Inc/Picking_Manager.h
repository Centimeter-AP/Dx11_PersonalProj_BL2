#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CPicking_Manager final : public CBase
{
public:
	typedef struct tagRay
	{
		_float3 vOrigin;
		_float3 vDirection;
	}RAY, *LPRAY;

private:
	CPicking_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPicking_Manager() = default;

public:
	HRESULT Initialize(HWND hWnd);
	void Update();

public:
	_bool Picking(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	_bool Pick_Mesh(_fmatrix WorldMatrix, const _float3* pVertices, const _uint* pIndices, _uint iNumIndices, _float3& vOutPickedPos);
	class CGameObject* Pick_Object_In_Layer(_uint iLevelIndex, const _wstring& strLayerTag, _float3& fPickedPos);


private:
	class CGameInstance* m_pGameInstance = { nullptr };
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	HWND m_hWnd = { nullptr };
	RAY  m_tRay = {};

	_float3 m_vRayPos = {};
	_float3 m_vRayDir = {};


public:
	static CPicking_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);
	virtual void Free() override;
};

NS_END