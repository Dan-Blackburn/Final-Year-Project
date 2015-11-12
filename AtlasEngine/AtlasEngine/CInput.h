#pragma once

#ifndef _CINPUT_
#define _CINPUT_

//Pre-Processing Directives
#define DIRECTINPUT_VERSION 0x0800

//Linking
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//Includes
#include <dinput.h>

//Input Class
class CInput
{
public:
	CInput();
	CInput(const CInput&);
	~CInput();

	bool Initialise(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_Keyboard;
	IDirectInputDevice8* m_Mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_viewportWidth, m_viewportHeight;
	int m_MouseX, m_MouseY;
};

#endif