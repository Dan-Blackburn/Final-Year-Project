//Includes
#include "CGraphics.h"

//Constructor
CGraphics::CGraphics() {
	m_Direct3D = 0;
	m_Camera = 0;
	m_EntityManager = 0;
	m_Shader = 0;
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

	//Create Entity Object
	m_EntityManager = new CEntityManager;

	if (!m_EntityManager) {
		return false;
	}

	//Initialise Model Objects
	result = m_EntityManager->InitialiseEntities(m_Direct3D->GetDevice());

	//Get Pointer to Main Camera
	m_Camera = m_EntityManager->GetCameraEntity();

	m_Camera->SetPosition(0.0f, 5.0f, -15.0f);

	if (result != true) {
		LPCWSTR errorMessage = (LPCWSTR)"Error Code: " + result;
		MessageBox(hwnd, L"Could not Initialise the Model Object.", errorMessage, MB_OK);
		return false;
	}

	//Create Shader Object
	m_Shader = new CShader;
	if (!m_Shader) {
		return false;
	}

	//Intialise Shader Object
	result = m_Shader->Initialise(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not Initialise the Shader Object.", L"Error", MB_OK);
		return false;
	}

	return true;
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

	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	//Clear Buffer to Reset the Viewport
	m_Direct3D->ClearBuffer(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->UpdateViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_EntityManager->PrepareEntities(m_Direct3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_EntityManager->RenderEntities(m_Direct3D->GetDeviceContext(), m_Shader, worldMatrix, viewMatrix, projectionMatrix);

	if (!result)
	{
		return false;
	}

	//Apply Render Scene to Viewport
	m_Direct3D->FrameBuffer();

	return true;
}


//Shutdown and Release Graphics Objects
void CGraphics::Shutdown() {

	//Release Direct3D Objects
	if (m_Direct3D) {
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	//Release Model Objects
	if (m_EntityManager) {
		m_EntityManager->Shutdown();
		delete m_EntityManager;
		m_EntityManager = 0;
	}

	//Release Shader Objects
	if (m_Shader) {
		m_Shader->Shutdown();
		delete m_Shader;
		m_Shader = 0;
	}

	//Release Camera Objects
	if (m_Camera) {
		delete m_Camera;
		m_Camera = 0;
	}

	return;
}

