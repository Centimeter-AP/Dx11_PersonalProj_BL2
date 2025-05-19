#ifndef InputDev_h__
#define InputDev_h__

#include "Base.h"

NS_BEGIN(Engine)

class  CInput_Device : public CBase
{
private:
	CInput_Device(void);
	virtual ~CInput_Device(void) = default;
	
public:
	_byte	Get_DIKeyState(_ubyte byKeyID)		
	{ 
		return m_byKeyState[byKeyID]; 
	}

	_byte	Get_DIMouseState(DIM eMouse)
	{ 	
		return m_tMouseState.rgbButtons[static_cast<_uint>(eMouse)]; 	
	}

	// 현재 마우스의 특정 축 좌표를 반환
	_long	Get_DIMouseMove(DIMM eMouseState)
	{	
		return *((reinterpret_cast<_int*>(&m_tMouseState)) + static_cast<_uint>(eMouseState));
	}

	_bool Key_Down(_ubyte eKeyID)
	{
		return (m_byKeyState[eKeyID] & 0x80) && !(m_byPrevKeyState[eKeyID] & 0x80);
	}

	_bool Key_Pressing(_ubyte eKeyID)
	{
		return (m_byKeyState[eKeyID] & 0x80);
	}

	_bool Key_Up(_ubyte eKeyID)
	{
		return !(m_byKeyState[eKeyID] & 0x80) && (m_byPrevKeyState[eKeyID] & 0x80);
	}

	_bool Mouse_Down(DIM eMouse)
	{
		return (m_tMouseState.rgbButtons[ENUM_CLASS(eMouse)] & 0x80) && !(m_tPrevMouseState.rgbButtons[ENUM_CLASS(eMouse)] & 0x80);
	}

	_bool Mouse_Pressing(DIM eMouse)
	{
		return (m_tMouseState.rgbButtons[ENUM_CLASS(eMouse)] & 0x80);
	}

	_bool Mouse_Up(DIM eMouse)
	{
		return !(m_tMouseState.rgbButtons[ENUM_CLASS(eMouse)] & 0x80) && (m_tPrevMouseState.rgbButtons[ENUM_CLASS(eMouse)] & 0x80);
	}

public:
	HRESULT Initialize(HINSTANCE hInst, HWND hWnd);
	void	Update(void);

private:
	LPDIRECTINPUT8			m_pInputSDK = { nullptr };

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = { nullptr };
	LPDIRECTINPUTDEVICE8	m_pMouse = { nullptr };

	
	


private:
	_byte					m_byKeyState[256] = {};
	_byte					m_byPrevKeyState[256] = { 0 };


	
	DIMOUSESTATE			m_tMouseState = {};
	DIMOUSESTATE			m_tPrevMouseState{};

public:
	static CInput_Device* Create(HINSTANCE hInstance, HWND hWnd);
	virtual void Free(void);

};

NS_END
#endif // InputDev_h__


