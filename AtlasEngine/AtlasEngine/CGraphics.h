#pragma once

#ifndef _CGRAPHICS_H_
#define _CGRAPHICS_H_

//Include
#include "CDirect3D.h"
#include "CEntityManager.h"
#include "CCamera.h"
#include "CShader.h"


//Global Variables
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

//Graphics Class
class CGraphics {
public:
	CGraphics();
	CGraphics(const CGraphics&);
	~CGraphics();

	bool Initialise(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();
	CDirect3D* m_Direct3D;
	CCamera* m_Camera;
	CEntityManager* m_EntityManager;
	CShader* m_Shader;
};

#endif