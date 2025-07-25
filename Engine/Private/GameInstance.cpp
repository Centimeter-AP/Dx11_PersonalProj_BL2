#include "GameInstance.h"

#include "Picking.h"
#include "Picking_Manager.h"
#include "Renderer.h"
#include "PipeLine.h"
#include "Frustum.h"
#include "Font_Manager.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Light_Manager.h"
#include "Timer_Manager.h"
#include "Collider_Manager.h"
#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Camera_Manager.h"
#include "Target_Manager.h"
#include "Sound_Device.h"

IMPLEMENT_SINGLETON(CGameInstance);

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, _Out_ ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut)
{
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, ppDeviceOut, ppContextOut);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pInput_Device = CInput_Device::Create(EngineDesc.hInstance, EngineDesc.hWnd);
	if (nullptr == m_pInput_Device)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pTarget_Manager = CTarget_Manager::Create(*ppDeviceOut, *ppContextOut);
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppDeviceOut, *ppContextOut);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	m_pSound_Device = CSound_Device::Create();
	if (nullptr == m_pSound_Device)
		return E_FAIL;

	m_pPicking_Manager = CPicking_Manager::Create(*ppDeviceOut, *ppContextOut, EngineDesc.hWnd);
	if (nullptr == m_pPicking_Manager)
		return E_FAIL;

	m_pLight_Manager = CLight_Manager::Create();
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	m_pFont_Manager = CFont_Manager::Create(*ppDeviceOut, *ppContextOut);
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	m_pCollider_Manager = CCollider_Manager::Create(EngineDesc.iNumColliderGroups);
	if (nullptr == m_pCollider_Manager)
		return E_FAIL;

	m_pShadow = CShadow::Create(*ppDeviceOut, *ppContextOut);
	if (nullptr == m_pShadow)
		return E_FAIL;

	m_pFrustum = CFrustum::Create();
	if (nullptr == m_pFrustum)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	m_pInput_Device->Update();
	m_pSound_Device->Update();

	m_pObject_Manager->Priority_Update(fTimeDelta);

	m_pObject_Manager->Update(fTimeDelta);

	m_pPipeLine->Update();

	m_pFrustum->Transform_ToWorldSpace();

	m_pObject_Manager->Late_Update(fTimeDelta);

	m_pLevel_Manager->Update(fTimeDelta);

	m_pPicking_Manager->Update();
}

HRESULT CGameInstance::Begin_Draw()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Clear_BackBuffer_View(_float4(0.f, 0.0f, 1.f, 1.f));
	m_pGraphic_Device->Clear_DepthStencil_View();

	return S_OK;
}

HRESULT CGameInstance::Draw()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pRenderer->Draw();

	m_pLevel_Manager->Render();

	return S_OK;
}

HRESULT CGameInstance::End_Draw()
{
	m_pGraphic_Device->Present();

	return S_OK;
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	/* 특정 레벨의 자원을 삭제한다. */
	/* 특정 레벨의 객체을 삭제한다. */
	m_pObject_Manager->Clear(iLevelIndex);

	/* 특정 레벨의 원형객을 삭제한다. */
	m_pPrototype_Manager->Clear(iLevelIndex);
}


_float CGameInstance::Compute_Random_Normal()
{
	return rand() / static_cast<_float>(RAND_MAX);
}

_float CGameInstance::Compute_Random(_float fMin, _float fMax)
{
	return fMin + (fMax - fMin) * Compute_Random_Normal();
}

#pragma region LEVEL_MANAGER

HRESULT CGameInstance::Change_Level(_uint iLevelIndex, CLevel* pNewLevel)
{
	return m_pLevel_Manager->Change_Level(iLevelIndex, pNewLevel);
}
#pragma endregion

#pragma region PROTOTYPE_MANAGER

HRESULT CGameInstance::Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{
	return m_pPrototype_Manager->Add_Prototype(iPrototypeLevelIndex, strPrototypeTag, pPrototype);
}

CBase* CGameInstance::Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	return m_pPrototype_Manager->Clone_Prototype(ePrototypeType, iPrototypeLevelIndex, strPrototypeTag, pArg);
}
HRESULT CGameInstance::Delete_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag)
{
	return m_pPrototype_Manager->Delete_Prototype(iPrototypeLevelIndex, strPrototypeTag);
}
HRESULT CGameInstance::Replace_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{
	return m_pPrototype_Manager->Replace_Prototype(iPrototypeLevelIndex, strPrototypeTag, pPrototype);
}
#pragma endregion

#pragma region OBJECT_MANAGER
HRESULT CGameInstance::Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pObject_Manager->Add_GameObject(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg);
}

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
	return m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex);
}

CGameObject* CGameInstance::Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iVectorIndex)
{
	return m_pObject_Manager->Find_Object(iLevelIndex, strLayerTag, iVectorIndex);
}

CLayer* CGameInstance::Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Find_Layer(iLevelIndex, strLayerTag);
}

void CGameInstance::Clear_Layer(_uint iLevelIndex, const _wstring& strLayerTag)
{
	m_pObject_Manager->Clear_Layer(iLevelIndex, strLayerTag);
}

void CGameInstance::Active_Layer(_uint iLevelIndex, const _wstring& strLayerTag, _bool bActive)
{
	m_pObject_Manager->Active_Layer(iLevelIndex, strLayerTag, bActive);
}

#pragma endregion

#pragma region RENDERER

HRESULT CGameInstance::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
	return m_pRenderer->Add_RenderGroup(eRenderGroup, pRenderObject);
}

HRESULT CGameInstance::Add_DebugComponent(CComponent* pDebugCom)
{
	return m_pRenderer->Add_DebugComponent(pDebugCom);
}

#pragma endregion

#pragma region TIMER_MANAGER

_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Update_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Update(strTimerTag);
}
#pragma endregion

#pragma region PIPELINE

void CGameInstance::Set_Transform(D3DTS eState, _fmatrix TransformMatrix)
{
	m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

const _float4x4* CGameInstance::Get_Transform_Float4x4(D3DTS eState) const
{
	return m_pPipeLine->Get_Transform_Float4x4(eState);
}

_matrix CGameInstance::Get_Transform_Matrix(D3DTS eState) const
{
	return m_pPipeLine->Get_Transform_Matrix(eState);
}

const _float4x4* CGameInstance::Get_Transform_Float4x4_Inv(D3DTS eState) const
{
	return m_pPipeLine->Get_Transform_Float4x4_Inv(eState);
}

_matrix CGameInstance::Get_Transform_Matrix_Inv(D3DTS eState) const
{
	return m_pPipeLine->Get_Transform_Matrix_Inv(eState);
}

const _float4* CGameInstance::Get_CamPosition() const
{
	return m_pPipeLine->Get_CamPosition();
}

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

_byte CGameInstance::Get_DIMouseState(DIM eMouse)
{
	return m_pInput_Device->Get_DIMouseState(eMouse);
}

_long CGameInstance::Get_DIMouseMove(DIMM eMouseState)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseState);
}

FORCEINLINE
_bool CGameInstance::Mouse_Down(DIM eMouse)
{
	return m_pInput_Device->Mouse_Down(eMouse);
}

FORCEINLINE
_bool CGameInstance::Mouse_Pressing(DIM eMouse)
{
	return m_pInput_Device->Mouse_Pressing(eMouse);
}

FORCEINLINE
_bool CGameInstance::Mouse_Up(DIM eMouse)
{
	return m_pInput_Device->Mouse_Up(eMouse);
}
#pragma endregion

FORCEINLINE
_bool CGameInstance::Key_Pressing(_ubyte eKeyID)
{
	return m_pInput_Device->Key_Pressing(eKeyID);
}

FORCEINLINE
_bool CGameInstance::Key_Up(_ubyte eKeyID)
{
	return m_pInput_Device->Key_Up(eKeyID);
}

FORCEINLINE
_bool CGameInstance::Key_Down(_ubyte eKeyID)
{
	return m_pInput_Device->Key_Down(eKeyID);
}



#pragma endregion

#pragma region PICKING


_bool CGameInstance::Picking(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	return m_pPicking_Manager->Picking(vPickedPos, vPointA, vPointB, vPointC);;
}

_bool CGameInstance::Pick_Mesh(_fmatrix WorldMatrix, const _float3* pVertices, const _uint* pIndices, _uint iNumIndices, _float3& vOutPickedPos, _Out_ _float& fHitDist)
{
	return m_pPicking_Manager->Pick_Mesh(WorldMatrix, pVertices, pIndices, iNumIndices, vOutPickedPos, fHitDist);
}
CGameObject* CGameInstance::Pick_Object_In_Layer(_uint iLevelIndex, const _wstring& strLayerTag, _float3& fPickedPos)
{
	return m_pPicking_Manager->Pick_Object_In_Layer(iLevelIndex, strLayerTag, fPickedPos);
}
//void CGameInstance::Transform_Picking_ToLocalSpace(const _float4x4& WorldMatrixInverse)
//{
//	m_pPicking->Transform_ToLocalSpace(WorldMatrixInverse);
//}
//_bool CGameInstance::Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
//{
//	return m_pPicking->Picking_InWorld(vPickedPos, vPointA, vPointB, vPointC);
//}
//_bool CGameInstance::Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
//{
//	return m_pPicking->Picking_InLocal(vPickedPos, vPointA, vPointB, vPointC);
//}
//
#pragma endregion

#pragma region LIGHT_MANAGER

const LIGHT_DESC* CGameInstance::Get_Light(_uint iIndex)
{
	return m_pLight_Manager->Get_Light(iIndex);
}

HRESULT CGameInstance::Add_Light(const LIGHT_DESC& LightDesc)
{
	return m_pLight_Manager->Add_Light(LightDesc);
}

HRESULT CGameInstance::Render_Lights(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pLight_Manager->Render_Lights(pShader, pVIBuffer);
}
void CGameInstance::Clear_Lights()
{
	m_pLight_Manager->Clear();
}
_uint CGameInstance::Get_LastLight_Index()
{
	return m_pLight_Manager->Get_LastLight_Index();
}
void CGameInstance::Delete_Light(_uint iIndex)
{
	m_pLight_Manager->Delete_Light(iIndex);
}
void CGameInstance::Delete_LastLight()
{
	m_pLight_Manager->Delete_LastLight();
}
#pragma endregion

#pragma region FONT_MANAGER

HRESULT CGameInstance::Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath)
{
	return m_pFont_Manager->Add_Font(strFontTag, pFontFilePath);
}

void CGameInstance::Draw_Font(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin, _float fScale)
{
	m_pFont_Manager->Draw(strFontTag, pText, vPosition, vColor, fRotation, vOrigin, fScale);
}
_vector CGameInstance::MeasureFont(const _wstring& strFontTag, const _tchar* pText, _bool ignoreWhiteSpace)
{
	return m_pFont_Manager->MeasureFont(strFontTag, pText, ignoreWhiteSpace);
}
#pragma endregion

#pragma region COLLIDER_MANAGER

HRESULT CGameInstance::Add_Collider(_uint iGroupID, CCollider* pCollider)
{
	return m_pCollider_Manager->Add_Collider(iGroupID, pCollider);
}

void CGameInstance::Intersect(CCollider* pDst, CCollider* pSrc)
{
	m_pCollider_Manager->Intersect(pDst, pSrc);
}

void CGameInstance::Intersect_Group(_uint iSrcGroupID, _uint iDstGroupID, _bool bMovePenetrate)
{
	m_pCollider_Manager->Intersect_Group(iSrcGroupID, iDstGroupID, bMovePenetrate);
}

CCollider* CGameInstance::Raycast(_fvector vRayOrigin, _fvector vRayDir, _float fRayLength, _uint iColliderGroupID, _float& fRayDist)
{
	return m_pCollider_Manager->Raycast(vRayOrigin, vRayDir, fRayLength, iColliderGroupID, fRayDist);
}

void CGameInstance::Delete_Collider(const CGameObject* pOwner)
{
	m_pCollider_Manager->Delete_Collider(pOwner);
}

_float3 CGameInstance::Resolve_Slide_AABB(CCollider* pMyCol, const _float3 vDesiredMove, _uint iGroupID)
{
	return m_pCollider_Manager->Resolve_Slide_AABB(pMyCol, vDesiredMove, iGroupID);
}

void CGameInstance::Clear_Collider()
{
	m_pCollider_Manager->Clear();
}

#pragma endregion


#pragma region TARGET_MANAGER
HRESULT CGameInstance::Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	return m_pTarget_Manager->Add_RenderTarget(strTargetTag, iWidth, iHeight, ePixelFormat, vClearColor);
}

HRESULT CGameInstance::Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag)
{
	return m_pTarget_Manager->Add_MRT(strMRTTag, strTargetTag);
}

HRESULT CGameInstance::Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV, _bool isTargetClear, _bool isDepthClear)
{
	return m_pTarget_Manager->Begin_MRT(strMRTTag, pDSV, isTargetClear, isDepthClear);
}

HRESULT CGameInstance::End_MRT()
{
	return m_pTarget_Manager->End_MRT();
}

HRESULT CGameInstance::Bind_RT_ShaderResource(const _wstring& strTargetTag, CShader* pShader, const _char* pContantName)
{
	return m_pTarget_Manager->Bind_ShaderResource(strTargetTag, pShader, pContantName);
}

HRESULT CGameInstance::Copy_RT_Resource(const _wstring& strTargetTag, ID3D11Texture2D* pDest)
{
	return m_pTarget_Manager->Copy_Resource(strTargetTag, pDest);
}

#ifdef _DEBUG

HRESULT CGameInstance::Ready_RT_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	return m_pTarget_Manager->Ready_Debug(strTargetTag, fX, fY, fSizeX, fSizeY);
}

HRESULT CGameInstance::Render_MRT_Debug(const _wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pTarget_Manager->Render_Debug(strMRTTag, pShader, pVIBuffer);
}

#endif

#pragma endregion

#pragma region PICKING
_bool CGameInstance::Picking(_float4* pOut)
{
	return m_pPicking->Picking(pOut);
}
#pragma endregion
#pragma region SHADOW

HRESULT CGameInstance::Ready_Light_For_Shadow(const CShadow::SHADOW_DESC& Desc)
{
	return m_pShadow->Ready_Light_For_Shadow(Desc);
}

const _float4x4* CGameInstance::Get_Light_ViewMatrix()
{
	return m_pShadow->Get_Light_ViewMatrix();
}
const _float4x4* CGameInstance::Get_Light_ProjMatrix()
{
	return m_pShadow->Get_Light_ProjMatrix();
}
#pragma endregion

#pragma region FRUSTUM

_bool CGameInstance::isIn_Frustum_WorldSpace(_fvector vWorldPos, _float fRange)
{
	return m_pFrustum->isIn_WorldSpace(vWorldPos, fRange);
}
HRESULT CGameInstance::LoadSound(const string& Path, _bool is3D, _bool loop, _bool stream, unordered_map<string, class CSound_Core*>* pOut)
{
	return m_pSound_Device->LoadSound(Path, is3D, loop, stream, pOut);
}
CSound_Core* CGameInstance::Get_Single_Sound(const string& strKey)
{
	return m_pSound_Device->Get_Single_Sound(strKey);
}
void CGameInstance::Set_Listener_Position(CTransform* pTransform, const _float3& vel)
{
	m_pSound_Device->Set_Listener_Position(pTransform, vel);
}
void CGameInstance::Set_Master_Volume(_float volume)
{
	m_pSound_Device->Set_Master_Volume(volume);
}
#pragma endregion

void CGameInstance::Release_Engine()
{
	Safe_Release(m_pShadow);

	Safe_Release(m_pFrustum);

	Safe_Release(m_pTarget_Manager);

	Safe_Release(m_pPicking_Manager);

	Safe_Release(m_pPicking);

	Safe_Release(m_pFont_Manager);

	Safe_Release(m_pPipeLine);

	Safe_Release(m_pTimer_Manager);

	Safe_Release(m_pRenderer);

	Safe_Release(m_pObject_Manager);

	Safe_Release(m_pPrototype_Manager);

	Safe_Release(m_pLevel_Manager);

	Safe_Release(m_pInput_Device);

	Safe_Release(m_pSound_Device);

	Safe_Release(m_pGraphic_Device);

	Safe_Release(m_pLight_Manager);

	Safe_Release(m_pCollider_Manager);


	Destroy_Instance();
}

void CGameInstance::Free()
{
	__super::Free();
}
