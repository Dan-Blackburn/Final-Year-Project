#include "CSystem.h"
#include "Console.h"
#include <iostream>

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	CSystem* System;
	bool result;

	RedirectIOToConsole();

	cout << "Console Window: " << endl;
 
	// Create the System object.
	System = new CSystem;
	if (!System)
	{
		return 0;
	}

	// Initialize and run the System object.
	result = System->Initialise();
	if (result)
	{
		System->Run();
	}

	// Shutdown and release the System object.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}