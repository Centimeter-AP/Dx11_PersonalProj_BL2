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

	_bool		m_isAnim = { false };
	_int		m_iCurNumBones = { };
	vector<string> m_BoneNames;

private:
	HRESULT Render_ConvertTool();
	HRESULT Convert_NonAnimFBX(const _char* pModelFilePath);
	HRESULT Convert_AnimFBX(const _char* pModelFilePath);
	HRESULT Copy_MaterialTextures();
	HRESULT Ready_FBXData(const _char* pModelFilePath, ofstream& ofs);
	HRESULT Ready_FBX_MeshData(FBXDATA& m_pMeshData);
	HRESULT Ready_FBX_AnimMeshData(FBXDATA& m_pAnimMeshData);
	HRESULT Ready_FBX_MaterialData(const _char* pModelFilePath, FBXDATA& m_pMatData);
	HRESULT Ready_FBX_BoneData(FBXDATA& m_pBoneData);
	HRESULT Ready_FBX_AnimationData(FBXDATA& m_pAnimData);

	HRESULT Ready_FBXData(const _char* pModelFilePath, FBXDATA& m_pFBXData);

private: // 멍청해서 다시쓰는중 파일스트림으로
	HRESULT Write_BoneData(const aiNode* pAINode, _int iParentBoneIndex, vector<string>& m_BoneNames, ostream& ofs);

public:
	static CConvertTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END