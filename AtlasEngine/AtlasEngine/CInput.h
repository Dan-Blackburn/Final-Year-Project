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

//Prototypes
enum Keyboard;

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

	bool KeyPressed(int Key_Pressed);
	bool KeyHeld();
	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);

	enum Keyboard {Key_W, Key_A, Key_S, Key_D, Key_Up, Key_Down, Key_Left, Key_Right};

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