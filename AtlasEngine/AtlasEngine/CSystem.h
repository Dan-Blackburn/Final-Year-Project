#pragma once

#ifndef _CSYSTEM_H_
#define _CSYSTEM_H_

//Pre Processor Directives
#define WIN32_LEAN_AND_MEAN

//Include Files
#include <Windows.h>
#include "CInput.h"
#include "CGraphics.h"


//System Class
class CSystem
{
public:
	CSystem();
	CSystem(const CSystem&);
	~CSystem();

	bool Initialise();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitialiseWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hwnd;

	CInput* m_Input;
	CGraphics* m_Graphics;
};

//Prototypes
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Globals
static CSystem* ApplicationHandle = 0;

#endif