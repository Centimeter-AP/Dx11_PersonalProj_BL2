#pragma once

/* 엔진과 클라이언트 간의 링크의 역활을 수행한다. */
/* 엔진 내에 존재하는 유일한 싱글톤 클래스이다. */
/* 엔진 개발자가 클라개밫자에게 보여주고싶은 함수를 ... */
#include "Prototype_Manager.h"
#include "Shadow.h"

NS_BEGIN(Engine)


class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance);

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, _Out_ ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut);
	void Update_Engine(_float fTimeDelta);
	void Clear(_uint iLevelIndex);
	HRESULT Begin_Draw();
	HRESULT Draw();
	HRESULT End_Draw();

	_float Compute_Random_Normal();
	_float Compute_Random(_float fMin, _float fMax);

	_float AddVariance(_float fValue, _float fPercentRange) // 입력한 퍼센트 만큼의 오차범위를 넣어 반환해주는 함수
	{
		_float r = ((rand() % 1001) / 1000.f) * 2.f - 1.f; // -1.0 ~ +1.0
		return fValue * (1.f + r * fPercentRange);
	}

#pragma region LEVEL_MANAGER
public:
	HRESULT Change_Level(_uint iLevelIndex, class CLevel* pNewLevel);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	CBase* Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
	// 프로토타입 삭제
	HRESULT Delete_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag);
	// 프로토타입 이미 있었다면 삭제 후 재생성
	HRESULT Replace_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
#pragma endregion

#pragma region OBJECT_MANAGER
	HRESULT Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	// 매개변수에 맞는 오브젝트를 찾아서 반환해준다.
	class CGameObject* Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iVectorIndex = 0);
	/* 매개변수에 맞는 레이어를 찾아서 반환. */
	class CLayer* Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);
	// 레이어 내 오브젝트 강제 철거 
	void Clear_Layer(_uint iLevelIndex, const _wstring& strLayerTag);
	void Active_Layer(_uint iLevelIndex, const _wstring& strLayerTag, _bool bActive = true);
#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
#ifdef _DEBUG
public:
	HRESULT Add_DebugComponent(class CComponent* pDebugCom);
#endif
#pragma endregion

#pragma region TIMER_MANAGER
	_float Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT Add_Timer(const _wstring& strTimerTag);
	void Update_Timer(const _wstring& strTimerTag);
#pragma endregion


#pragma region 	PIPELINE
	void Set_Transform(D3DTS eState, _fmatrix TransformMatrix);
	const _float4x4* Get_Transform_Float4x4(D3DTS eState) const;
	_matrix Get_Transform_Matrix(D3DTS eState) const;
	const _float4x4* Get_Transform_Float4x4_Inv(D3DTS eState) const;
	_matrix Get_Transform_Matrix_Inv(D3DTS eState) const;
	const _float4* Get_CamPosition() const;
#pragma endregion

#pragma region INPUT_DEVICE
	_byte	Get_DIKeyState(_ubyte byKeyID);
	_byte	Get_DIMouseState(DIM eMouse);
	_long	Get_DIMouseMove(DIMM eMouseState);
	_bool	Mouse_Down(DIM eMouse);
	_bool	Mouse_Pressing(DIM eMouse);
	_bool	Mouse_Up(DIM eMouse);
	_bool	Key_Pressing(_ubyte eKeyID);
	_bool	Key_Up(_ubyte eKeyID);
	_bool	Key_Down(_ubyte eKeyID);
#pragma endregion
	//
	#pragma region PICKING
	//	void Transform_Picking_ToLocalSpace(const _float4x4& WorldMatrixInverse);
	//	_bool Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	//	_bool Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	_bool Picking(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	_bool Pick_Mesh(_fmatrix WorldMatrix, const _float3* pVertices, const _uint* pIndices, _uint iNumIndices, _float3& vOutPickedPos, _Out_ _float& fHitDist);
	CGameObject* Pick_Object_In_Layer(_uint iLevelIndex, const _wstring& strLayerTag, _float3& fPickedPos);
	#pragma endregion

#pragma region LIGHT_MANAGER
	const LIGHT_DESC* Get_Light(_uint iIndex);
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	void Clear_Lights();
	_uint Get_LastLight_Index();
	void Delete_Light(_uint iIndex);
	void Delete_LastLight();
#pragma endregion

#pragma region FONT_MANAGER
	HRESULT Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath);
	void Draw_Font(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f), _float fRotation = 0.f, const _float2& vOrigin = _float2(0.f, 0.f), _float fScale = 1.f);
	_vector MeasureFont(const _wstring& strFontTag, const _tchar* pText, _bool ignoreWhiteSpace = true);
#pragma endregion

#pragma region COLLIDER_MANAGER
	HRESULT Add_Collider(_uint iGroupID, class CCollider* pCollider);
	void Intersect(class CCollider* pDst, class CCollider* pSrc);
	void Intersect_Group(_uint iSrcGroupID, _uint iDstGroupID, _bool bMovePenetrate = true);
	class CCollider* Raycast(_fvector vRayOrigin, _fvector vRayDir, _float fRayLength, _uint iColliderGroupID, _float& fRayDist);
	void Delete_Collider(const class CGameObject* pOwner);
	_float3 Resolve_Slide_AABB(class CCollider* pMyCol, const _float3 vDesiredMove, _uint iGroupID);
	void Clear_Collider();
#pragma endregion


#pragma region TARGET_MANAGER
	HRESULT Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag);
	HRESULT Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV = nullptr, _bool isTargetClear = true, _bool isDepthClear = false);
	HRESULT End_MRT();
	HRESULT Bind_RT_ShaderResource(const _wstring& strTargetTag, class CShader* pShader, const _char* pContantName);
	HRESULT Copy_RT_Resource(const _wstring& strTargetTag, ID3D11Texture2D* pDest);

#ifdef _DEBUG
	HRESULT Ready_RT_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_MRT_Debug(const _wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif

#pragma endregion

#pragma region PICKING
	_bool Picking(_float4* pOut);
#pragma endregion

#pragma region SHADOW
	HRESULT Ready_Light_For_Shadow(const CShadow::SHADOW_DESC& Desc);
	const _float4x4* Get_Light_ViewMatrix();
	const _float4x4* Get_Light_ProjMatrix();
#pragma region

#pragma region FRUSTUM	
	_bool isIn_Frustum_WorldSpace(_fvector vWorldPos, _float fRange = 0.f);
#pragma region

#pragma region SOUND_DEVICE
	// Out에 nullptr 넣으면 싱글사운드로 등록됨
	HRESULT LoadSound(const string& Path, _bool is3D = FALSE, _bool loop = FALSE, _bool stream = FALSE, unordered_map<string, class CSound_Core*>* _Out_ pOut = nullptr);
	// 난 에드래퍼 올릴거다 분명히 말했다 래퍼 올린다 릴리즈 시키시오
	class CSound_Core* Get_Single_Sound(const string& strKey);
	// 리스너 포지션 제어(업룩포 요구해서 트랜스폼 받았음)
	void Set_Listener_Position(class CTransform* pTransform, const _float3& vel);
	// 전체 볼륨 제어
	void Set_Master_Volume(_float volume);
#pragma endregion


private:
	class CGraphic_Device*		m_pGraphic_Device = { nullptr };
	class CInput_Device*		m_pInput_Device = { nullptr };
	class CSound_Device*		m_pSound_Device = { nullptr };
	class CPrototype_Manager*	m_pPrototype_Manager = { nullptr };
	class CObject_Manager*		m_pObject_Manager = { nullptr };
	class CLevel_Manager*		m_pLevel_Manager = { nullptr };
	class CRenderer*			m_pRenderer = { nullptr };
	class CPipeLine*			m_pPipeLine = { nullptr };
	class CPicking_Manager*		m_pPicking_Manager = { nullptr };
	class CTimer_Manager*		m_pTimer_Manager = { nullptr };
	class CLight_Manager*		m_pLight_Manager = { nullptr };
	class CFont_Manager*		m_pFont_Manager = { nullptr };
	class CCollider_Manager*	m_pCollider_Manager = { nullptr };
	class CCamera_Manager*		m_pCamera_Manager = { nullptr };
	class CTarget_Manager*		m_pTarget_Manager = { nullptr };
	class CPicking*				m_pPicking = { nullptr };
	class CShadow*				m_pShadow = { nullptr };
	class CFrustum*				m_pFrustum = { nullptr };


public:
	void Release_Engine();
	virtual void Free() override;
};

NS_END