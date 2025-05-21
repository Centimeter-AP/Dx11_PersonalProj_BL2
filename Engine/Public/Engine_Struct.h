#pragma once

namespace Engine
{
	typedef struct tagEngineDesc 
	{
		HINSTANCE		hInstance;
		HWND			hWnd;
		bool			isWindowed;
		unsigned int	iWinSizeX;
		unsigned int	iWinSizeY;
		unsigned int	iNumLevels;		
	}ENGINE_DESC;

	typedef struct tagKeyFrame
	{
		/* 행렬이 아닌이유? : 상태와 상태 사이를 보간해주기위해서. */
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vTranslation;

		/* 이 상태를 취해야하는 재생위치  */
		float			fTrackPosition;
	}KEYFRAME;

	typedef struct ENGINE_DLL tagVertexPositionTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int					iNumElements = {2};

		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertexPositionNormalTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const unsigned int					iNumElements = { 3 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertexMesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT3		vTangent;
		XMFLOAT2		vTexcoord;

		static const unsigned int					iNumElements = { 4 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXMESH;

	typedef struct ENGINE_DLL tagVertexAnimMesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT3		vTangent;
		XMFLOAT2		vTexcoord;
		XMUINT4			vBlendIndices;
		XMFLOAT4		vBlendWeights;

		static const unsigned int					iNumElements = { 6 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	}VTXANIMMESH;

#pragma region Model_Binary

	typedef struct tagBoneData
	{
		string		strBoneName;
		/* XMMatrixTranspose 후 저장할 것 !!! */
		_float4x4	TransformMatrix = {};
		_int		iParentBoneIndex = {-2};
	}FBX_BONEDATA;

	typedef struct tagAnimationData
	{

	}FBX_ANIMDATA;

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
		string strTexturePath;
	}FBX_MATDATA;

	typedef struct tagMeshData
	{
		string strMeshName; 
		_uint iMaterialIndex = {};
		_uint iNumVertices = {};
		_uint iNumIndices = {};
		vector<_uint>	vecIndices;
		vector<VTXMESH> vecVertices;
	}FBX_MESHDATA;


	typedef struct tagFBXData
	{
		_uint iNumBones = {};
		vector<FBX_BONEDATA> vecBones;
		_uint iNumMeshes = {};
		vector<FBX_MESHDATA> vecMeshes;
		_uint iNumMaterials = {};
		vector<vector<FBX_MATDATA>> vecMaterials;
		string strFBXName;
		_uint iNumAnimations = {};
		vector<FBX_ANIMDATA> vecAnimations;
	}FBXDATA;

#pragma endregion

}