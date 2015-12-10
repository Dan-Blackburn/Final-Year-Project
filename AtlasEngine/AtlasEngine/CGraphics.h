#pragma once

#ifndef _CGRAPHICS_H_
#define _CGRAPHICS_H_

//Include
#include "CDirect3D.h"
#include "CEntityManager.h"
#include "CCamera.h"
#include "CShaderManager.h"


//Global Variables
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 10000.0f;
const float SCREEN_NEAR = 0.1f;

//Graphics Class
class CGraphics {
public:
	CGraphics();
	CGraphics(const CGraphics&);
	~CGraphics();

	bool Initialise(int, int, HWND);
	void Shutdown();
	bool Frame(CInput* m_Input);

private:
	bool Render();
	CDirect3D* m_Direct3D;
	CCamera* m_Camera;
	CEntityManager* m_EntityManager;
	CShaderManager* m_ShaderManager;
};

#endif