#include "CSystem.h"

//Constructor
CSystem::CSystem() {
	m_Input = 0;
	m_Graphics = 0;
	m_Timer = 0;
}

//Copy Constructor
CSystem::CSystem(const CSystem& other) {

}

//Destructor
CSystem::~CSystem() {

}

//Initialise Application
bool CSystem::Initialise() {

	//Viewport Dimensions
	int viewportWidth = 0;
	int viewportHeight = 0;
	bool result;

	//Initialise Windows API
	InitialiseWindows(viewportWidth, viewportHeight);

	//Define Input Object
	m_Input = new CInput;
	if (!m_Input) {
		return false;
	}

	//Initialise Input Object
	result = m_Input->Initialise(m_hInstance, m_hwnd, viewportWidth, viewportHeight);

	//Input Error Check
	if (!result)
	{
		MessageBox(m_hwnd, "Could not initialize the Input Object.", "Error", MB_OK);
		return false;
	}

	//Define Graphics Object
	m_Graphics = new CGraphics;
	if (!m_Graphics) {
		result = false;
	}

	//Initialise Graphics Object
	result = m_Graphics->Initialise(viewportWidth, viewportHeight, m_hwnd);

	//Graphics Error Check
	if (!result)
	{
		MessageBox(m_hwnd, "Could not initialize the Graphics Object.", "Error", MB_OK);
		return false;
	}

	//Define Graphics Object
	m_Timer = new CTimer;
	if (!m_Timer)
	{
		return false;
	}

	//Initialise the Timer Object
	result = m_Timer->Initialise();
	if (!result)
	{
		MessageBox(m_hwnd, "Could not initialise the Timer Object.", "Error", MB_OK);
		return false;
	}

	return true;
}

//Shutdown Application and Cleanup Assets
void CSystem::Shutdown() {

	//Release Graphics Objects
	if (!m_Graphics) {
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	//Release Input Objects
	if (!m_Input) {
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	if (!m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	//Shutdown the Viewport
	ShutdownWindows();

	return;
}

//Looping Function (Gameplay)
void CSystem::Run() {
	MSG msg;
	bool playing, result;

	//Initialise Message Structure
	ZeroMemory(&msg, sizeof(MSG));

	playing = true;
	//Loop Application Until User Quit
	while (playing) {

		//Windows Message Handler
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//Check for Windows Signal to Exit
		if (msg.message == WM_QUIT) {
			playing = false;
		}
		else {
			//Continue Processing Frames
			result = Frame();
			if (!result) {
				playing = false;
			}
		}

		//Check if the Escape Key has been Pressed
		if (m_Input->IsEscapePressed() == true)
		{
			playing = false;
		}

	}
	
	return;
}

//Engine Processing Function
bool CSystem::Frame() {
	bool result;
	int mouseX, mouseY;

	m_Timer->Frame();

	//Input Frame processing
	result = m_Input->Frame();

	//Input Frame Error Check
	if (!result)
	{
		return false;
	}

	//Process Frame for Graphics Object
	result = m_Graphics->Frame(m_Input, m_Timer->GetTime(), m_Timer->GetClock());
	if (!result) 
	{
		return false;
	}
	
	return true;
}

//Message Handling Function (System Messages)
LRESULT CALLBACK CSystem::MessageHandler(HWND hwnd, UINT keyMsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, keyMsg, wparam, lparam);
}

//Initialises Viewport Window for Rendering
void CSystem::InitialiseWindows(int& viewportWidth, int& viewportHeight) {

	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//External Point to this Object
	ApplicationHandle = this;

	//Get Instance of this Application
	m_hInstance = GetModuleHandle(NULL);

	//Application Name
	m_applicationName = "AtlasEngine";

	//Setup Windows Class (Default Settings)
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//Register Windows Class
	RegisterClassEx(&wc);

	//Determine Resolution of Client's Screen
	viewportWidth = GetSystemMetrics(SM_CXSCREEN);
	viewportHeight = GetSystemMetrics(SM_CYSCREEN);

	//Screen Settings (Orientation)
	if (FULL_SCREEN) {

		//Set Screen to maximum size of the Users Desktop
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)viewportWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)viewportHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//Change Display Settings to Fullscreen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//Set Window Position to Top Left Corner
		posX = posY = 0;
	}
	else {

		//Set Screen to Resolution Below
		viewportWidth = 1280;
		viewportHeight = 720;

		//Place Window in Middle of Screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - viewportWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - viewportHeight) / 2;
	}

	//Create Window in Screen Settings
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_OVERLAPPEDWINDOW,
		posX, posY, viewportWidth, viewportHeight, NULL, NULL, m_hInstance, NULL);

	//Set Window as Main Focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//Hide Cursor
	ShowCursor(false);

	return;
}

//Shutdown and Release Window
void CSystem::ShutdownWindows() {
	
	//Show Mouse Cursor
	ShowCursor(true);

	//Fix Display Setting (Leaving Fullscreen Mode)
	if (FULL_SCREEN) {
		ChangeDisplaySettings(NULL, 0);
	}

	//Release Window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//Release Application Instance
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;

	//Release Pointer to Application Instance
	ApplicationHandle = NULL;

	return;
}

//Function to Receive Windows Messages
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	
	//Checks Windows Message
	switch (umessage) {
		//Checks if Window is being Destroyed
		case WM_DESTROY:
			PostQuitMessage(0);
			return false;
			break;
		//Checks if Window is being closed
		case WM_CLOSE:
			PostQuitMessage(0);
			return false;
			break;
		//Other Messages passed to Message Handler
		default: 
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}