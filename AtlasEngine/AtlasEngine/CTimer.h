#pragma once

#ifndef _CTIMER_H_
#define _CTIMER_H_

//Includes
#include <windows.h>

//Timer Class
class CTimer
{
public:
	//Constructors & Destructor
	CTimer();
	CTimer(const CTimer&);
	~CTimer();

	bool Initialise();
	void Frame();

	//Getters & Setters
	inline float GetTime() { return m_FrameTime; };
	inline float GetClock() { return m_Time; }

private:
	INT64 m_Frequency;
	float m_TicksPerMS;
	INT64 m_StartTime;
	float m_FrameTime;

	float m_Time;
};

#endif