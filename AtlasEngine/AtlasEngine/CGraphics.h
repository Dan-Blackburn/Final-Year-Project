#pragma once

#ifndef _CGRAPHICS_H_
#define _CGRAPHICS_H_

//Include
#include "CDirect3D.h"

//Global Variables
const bool FULL_SCREEN = false;
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

};

#endif