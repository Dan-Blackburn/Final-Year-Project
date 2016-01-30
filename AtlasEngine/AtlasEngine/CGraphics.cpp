//Includes
#include "CGraphics.h"
#include <iostream>

//Constructor
CGraphics::CGraphics() {
	m_Direct3D = 0;
	m_Camera = 0;
	m_EntityManager = 0;
	m_ShaderManager = 0;
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

	/////////////////////////////////////////////////////////////
	//Direct3D Partition
	/////////////////////////////////////////////////////////////

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
		MessageBox(hwnd, "Could not initialise Direct3D", "Error", MB_OK);
		return false;
	}

	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	//Entity Manager Partition
	/////////////////////////////////////////////////////////////

	//Create Entity Object
	m_EntityManager = new CEntityManager;
	if (!m_EntityManager) {
		return false;
	}

	//Initialise Model Objects
	result = m_EntityManager->InitialiseEntities(m_Direct3D->GetDevice());

	if (!result) {
		return false;
	}

	//Get Pointer to Main Camera
	m_Camera = m_EntityManager->GetCameraEntity();

	if (!m_Camera)
	{
		return false;
	}

	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	//Shader Partition
	/////////////////////////////////////////////////////////////

	//Create Shader Object
	m_ShaderManager = new CShaderManager;
	if (!m_ShaderManager) 
	{
		return false;
	}

	//Intialise Shader Object
	result = m_ShaderManager->InitialiseShaders(m_Direct3D->GetDevice(), hwnd);
	if (!result) 
	{
		MessageBox(hwnd, "Could not Initialise the Shader Object.", "Error", MB_OK);
		return false;
	}

	/////////////////////////////////////////////////////////////

	return true;
}

//Calculate Frame
bool CGraphics::Frame(CInput* m_Input, float frameTime, float clock) {

	D3DXMATRIX worldMatrix;

	//Camera Frame Function
	m_Camera->Frame(m_Input, frameTime);

	//Update World Matrix
	m_Direct3D->UpdateWorldMatrix(worldMatrix);

	//Entity Frame Function, Setting Position of the Entitys within the World
	m_EntityManager->Frame(m_Direct3D->GetDeviceContext(), frameTime, clock, worldMatrix);

	//Render the Graphics Scene
	bool result = Render();

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

	//Get Matrices from Camera and Direct3D Objects
	m_Camera->UpdateViewMatrix(viewMatrix);
	m_Direct3D->UpdateWorldMatrix(worldMatrix);
	m_Direct3D->UpdateProjectionMatrix(projectionMatrix);

	// Render the model using the color shader
	result = m_EntityManager->RenderEntities(m_Direct3D, m_ShaderManager, worldMatrix, viewMatrix, projectionMatrix);

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
	if (m_ShaderManager) {
		//m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}

	//Release Camera Objects
	if (m_Camera) {
		delete m_Camera;
		m_Camera = 0;
	}

	return;
}

