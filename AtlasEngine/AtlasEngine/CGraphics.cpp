#include "CGraphics.h"

//Constructor
CGraphics::CGraphics() {

}

//Copy Constructor
CGraphics::CGraphics(const CGraphics& other) {

}

//Destructor
CGraphics::~CGraphics() {

}

//Initialise Graphics Objects
bool CGraphics::Initialise(int viewportWidth, int viewportHeight, HWND hwnd) {
	return true;
}

//Shutdown and Release Graphics Objects
void CGraphics::Shutdown() {
	return;
}

bool CGraphics::Frame() {
	return true;
}

bool CGraphics::Render() {
	return true;
}

