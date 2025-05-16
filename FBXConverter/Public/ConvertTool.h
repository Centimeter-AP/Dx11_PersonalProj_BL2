#pragma once
#include "ImGuiTool.h"

NS_BEGIN(Client)


class CConvertTool final : public CImGuiTool
{
private:
	CConvertTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CConvertTool(const CConvertTool& Prototype);
	virtual ~CConvertTool() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	Assimp::Importer		m_Importer;

	/* 모델에 대한 모든 정보르,ㄹ 담고 있는 구조체. */
	const aiScene* m_pAIScene = { nullptr };

	//FBXDATA m_pFBXData = { };
	list<path> materialList;
	string		savePath;

private:
	HRESULT Render_ConvertTool();
	HRESULT Convert_NonAnimFBX(const _char* pModelFilePath);
	HRESULT Copy_MaterialTextures();
	HRESULT Ready_FBXData(const _char* pModelFilePath, FBXDATA& m_pFBXData);

public:
	static CConvertTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END