#include "CInput.h"

//Constructor
CInput::CInput() 
{
	m_directInput = 0;
	m_Keyboard = 0;
	m_Mouse = 0;
}

//Copy Constructor
CInput::CInput(const CInput &other) 
{

}

//Destructor
CInput::~CInput() 
{

}

//Initialise Input Class
bool CInput::Initialise(HINSTANCE hinstance, HWND hwnd, int viewportWidth, int viewportHeight)
{
	HRESULT result;

	//Store Screen Size
	m_viewportWidth = viewportWidth;
	m_viewportHeight = viewportHeight;

	//Set Mouse Position on Screen
	m_MouseX = 0;
	m_MouseY = 0;

	//Initialize DirectInput Interface
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//Initialise DirectInput for Keyboard
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_Keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	//Set Date Format
	result = m_Keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	//Set Cooperative level of Keyboard (to not share with other programs)
	result = m_Keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//Aquire Keyboard
	result = m_Keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	//Initialise DirectInput Interface for Mouse
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_Mouse, NULL);

	if (FAILED(result))
	{
		return false;
	}

	//Set Data Format for Mouse
	result = m_Mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	//Set Cooperative level of Mouse (to not share with other programs)
	result = m_Mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//Acquire the Mouse
	result = m_Mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void CInput::Shutdown()
{
	//Release the Mouse
	if (m_Mouse)
	{
		m_Mouse->Unacquire();
		m_Mouse->Release();
		m_Mouse = 0;
	}

	//Release the Keyboard
	if (m_Keyboard)
	{
		m_Keyboard->Unacquire();
		m_Keyboard->Release();
		m_Keyboard = 0;
	}

	//Release Direct Input Interface
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}

bool CInput::Frame()
{
	bool result;

	//Read Keyboard State
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	//Read Mouse State
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	//Process changes in Mouse and Keyboard
	ProcessInput();

	return true;
}

bool CInput::ReadKeyboard()
{
	HRESULT result;

	//Ready Keyboard Device
	result = m_Keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);

	if (FAILED(result))
	{
		//Reaquire Keyboard if Focus was Lost
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_Keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool CInput::ReadMouse()
{
	HRESULT result;


	//Read Mouse Device
	result = m_Mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		//Reaquire Mouse if Focus was Lost
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_Mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void CInput::ProcessInput()
{
	//Update Cursor Location based on Change during Frame
	m_MouseX += m_mouseState.lX;
	m_MouseY += m_mouseState.lY;

	//Boundary Code
	/*if (m_MouseX < 0) { m_MouseX = 0; }
	if (m_MouseY < 0) { m_MouseY = 0; }

	if (m_MouseX > m_viewportWidth) { m_MouseX = m_viewportWidth; }
	if (m_MouseY > m_viewportHeight) { m_MouseY = m_viewportHeight; }*/

	return;
}

bool CInput::KeyHeld()
{
	return true;
}

bool CInput::KeyPressed(int KeyPress)
{

	switch (KeyPress) 
	{
	case Key_W:
		if (m_keyboardState[DIK_W] & 0x80) { return true; }
		break;
	case Key_A:
		if (m_keyboardState[DIK_A] & 0x80) { return true; }
		break;
	case Key_S:
		if (m_keyboardState[DIK_S] & 0x80) { return true; }
		break;
	case Key_D:
		if (m_keyboardState[DIK_D] & 0x80) { return true; }
		break;
	case Key_Up:
		if (m_keyboardState[DIK_UP] & 0x80) { return true; }
		break;
	case Key_Down:
		if (m_keyboardState[DIK_DOWN] & 0x80) { return true; }
		break;
	case Key_Left:
		if (m_keyboardState[DIK_LEFT] & 0x80) { return true; }
		break;
	case Key_Right:
		if (m_keyboardState[DIK_RIGHT] & 0x80) { return true; }
		break;
	}

	return false;
}

bool CInput::IsEscapePressed()
{
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}
	return false;
}

void CInput::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_MouseX;
	mouseY = m_MouseY;
	return;
}