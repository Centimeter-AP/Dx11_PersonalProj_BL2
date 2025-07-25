#pragma once

/* 직교투영, 카메라, 객체만듬, 후처리 쉐이딩, 블러, 그림자, 소프트이펙트, 
픽셀피킹, 모델 로딩, 애니메이팅, 파티클, 인스턴싱, 스프레팅쉐이더 */

/* 특정 헤더파일에 소속되지 않는 전역정보들을 선언해두는 역활 */
/* 기타 다른 정의파일(.h)들을 인클루드하는 역활. */
#include "../Default/framework.h"
#include <process.h>
#include "Client_Enum.h"
#include "Client_Struct.h"
#include "Client_Macro.h"

#define _CONSOLE

#ifdef _CONSOLE
#include <iostream>
#endif // _CONSOLE

#pragma push_macro("new")
#undef new
#include "json.hpp"
using json = nlohmann::json;
#pragma pop_macro("new")

#ifdef USE_IMGUI
#pragma push_macro("new")
#undef new
#include "imgui.h"
#include "imgui_internal.h"
#pragma pop_macro("new")
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "ImGuizmo.h"
#include "ImGuiFileDialog.h"
using namespace ImGui;
#endif

namespace Client
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;
}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;


using namespace Client;




/* 1. 피해의식갑.(여) */
/* 1. 추천빌런.(여) */
/* 1. 여미새, 남미새 */
/* 1. 5기 호로새끼.(남) */
/* 1. 야겜만든중2병.(남) */
/* 1. 착한쁘락지.(남) */
/* 1. 게이.(남) */
/* 1. 코스프레.(남, 여) */
/* 1. 어소트락. */
/* 1. 서울게임아카데미 */
/* 1. 쥬신의 과거 */
/* 1. 쥬신의 탈피. (펜션, 애견 놀이터, 애견 카페, ) */






