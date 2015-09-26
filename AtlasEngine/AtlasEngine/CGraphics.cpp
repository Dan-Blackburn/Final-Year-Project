//Includes
#include "CGraphics.h"

//Constructor
CGraphics::CGraphics() {
	m_Direct3D = 0;
}

//Copy Constructor
CGraphics::CGraphics(const CGraphics& other) {

}

//Destructor
CGraphics::~CGraphics() {

}

//Initialise Graphics Objects
bool CGraphics::Initialise(int viewportWidth, int viewportHeight, HWND hwnd) {

	bool result;

	//Create Direct3D Object
	m_Direct3D = new CDirect3D();

	//Checks if Object Exists
	if (!m_Direct3D) {
		return false;
	}

	//Initialise Direct3D Object
	result = m_Direct3D->Initialise(viewportWidth, viewportHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	//Checks Direct3D Object was Initialised
	if (!result) {
		MessageBox(hwnd, L"Could not initialise Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}

//Shutdown and Release Graphics Objects
void CGraphics::Shutdown() {

	//Shutdown Direct3D Objects
	if (m_Direct3D) {
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}

//Calculate Frame
bool CGraphics::Frame() {

	bool result;

	//Render the Graphics Scene
	result = Render();

	//Check if Scene was Rendered;
	if (!result) {
		return false;
	}

	return true;
}

//Render Object to Viewport
bool CGraphics::Render() {

	//Clear Buffer to Reset the Viewport
	m_Direct3D->ClearBuffer(0.5f, 0.5f, 0.5f, 1.0f);

	//Apply Render Scene to Viewport
	m_Direct3D->FrameBuffer();

	return true;
}

