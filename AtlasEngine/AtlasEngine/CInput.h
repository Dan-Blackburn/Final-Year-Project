#pragma once
#ifndef _CInput_H_
#define _CInput_H_

//Input Class handles the users Input from the Keyboard
class CInput
{
public:
	CInput();
	CInput(const CInput&);
	~CInput();

	void Initialise();

	void KeyPressed(unsigned int);
	void KeyReleased(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_Keys[256];
};

#endif