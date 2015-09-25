#include "CInput.h"

//Constructor
CInput::CInput(){

}

//Copy Constructor
CInput::CInput(const CInput&  other) {

}

//Destructor
CInput::~CInput() {

}

//Initialises all Keys
void CInput::Initialise() {

	int numOfKeys = 256;

	//Loops through each Key and Initialises
	for (int i = 0; i < numOfKeys; i++) {
		m_Keys[i] = false;
	}

	return;
}

//Handles Key being Released
void CInput::KeyReleased(unsigned int input) {
	m_Keys[input] = true;
	return;
}

//Handles Key being Pressed
void CInput::KeyPressed(unsigned int input) {
	m_Keys[input] = true;
	return;
}

//Checks state of chosen Key
bool CInput::IsKeyDown(unsigned int key) {
	return m_Keys[key];
}