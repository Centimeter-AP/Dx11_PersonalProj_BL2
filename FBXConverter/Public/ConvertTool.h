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
	HRESULT Render_ConvertTool();
	HRESULT Convert_NonAnimFBX(const _char* pModelFilePath);


private:
	Assimp::Importer		m_Importer;

	/* 모델에 대한 모든 정보르,ㄹ 담고 있는 구조체. */
	const aiScene* m_pAIScene = { nullptr };

	typedef struct tagBoneData
	{

	}FBX_BONEDATA;

	typedef struct tagAnimMeshData
	{
		_uint iMaterialIndex = {};
		_uint iNumVertices = {};
		_uint iNumIndices = {};
		vector<VTXANIMMESH> vecVertices;
	}FBX_ANIMMESHDATA;

	typedef struct tagMaterialData
	{
		aiTextureType eTexType;
		_uint iNumSRVs;
		string strTexturePath;
	}FBX_MATDATA;

	typedef struct tagMeshData
	{
		_uint iMaterialIndex = {};
		_uint iNumVertices = {};
		_uint iNumIndices = {};
		vector<_uint>	vecIndices;
		vector<VTXMESH> vecVertices;
	}FBX_MESHDATA;


	typedef struct tagFBXData
	{
		_uint iNumMeshes = {};
		vector<FBX_MESHDATA> vecMeshes;
		_uint iNumMaterials = {};
		vector<FBX_MATDATA> vecMaterials;
	}FBXDATA;


	FBXDATA* m_pFBXData = { nullptr };


public:
	static CConvertTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END