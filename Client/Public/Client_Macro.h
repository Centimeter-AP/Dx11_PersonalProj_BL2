#pragma once



#pragma region 유틸리티

#define GET_PLAYER m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::STATIC),TEXT("Layer_Player"))
#define GET_PLAYER_TRANSFORM static_cast<CTransform*>(GET_PLAYER->Find_Component(TEXT("Com_Transform")))

#define GET_CAMERA											\
_float4x4 matCamWorld;										\
m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matCamWorld);	\
matCamWorld.MakeInverseMat(matCamWorld);

#define Get_CamaraPos GET_CAMERA											\
_float3 vCameraPos = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };	

#define _VUp		_float3(0.f,1.f,0.f)

#define FWINCX static_cast<_float>(g_iWinSizeX)
#define FWINCY static_cast<_float>(g_iWinSizeY)

/* RGBA Color Float4 */
#define RGBA_WHITE      _float4{1.f, 1.f, 1.f, 1.f}
#define RGBA_BLACK      _float4{0.f, 0.f, 0.f, 1.f}
#define RGBA_RED		_float4{1.f, 0.f, 0.f, 1.f}
#define RGBA_GREEN      _float4{0.f, 1.f, 0.f, 1.f}
#define RGBA_BLUE		_float4{0.f, 0.f, 1.f, 1.f}
#define RGBA_YELLOW     _float4{1.f, 1.f, 0.f, 1.f}
#define RGBA_MAGENTA	_float4{1.f, 0.f, 1.f, 1.f}
#define RGBA_CYAN		_float4{0.f, 1.f, 1.f, 1.f}

#define KEY_DOWN		m_pGameInstance->Key_Down
#define	KEY_PRESSING	m_pGameInstance->Key_Pressing
#define KEY_UP			m_pGameInstance->Key_Up

#define MOUSE_DOWN		m_pGameInstance->Mouse_Down
#define MOUSE_PRESSING	m_pGameInstance->Mouse_Pressing
#define MOUSE_UP		m_pGameInstance->Mouse_Up

#define CLAMP(x, min, max) (x < min ? min : (x > max ? max : x))

#define CAM_FREE 0
#define CAM_FPS 1

#pragma endregion
