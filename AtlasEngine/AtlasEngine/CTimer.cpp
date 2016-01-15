//Includes
#include "CTimer.h"

//Constructor
CTimer::CTimer()
{
	m_Time = 6.0f;
}

//Copy Constructor
CTimer::CTimer(const CTimer &other)
{

}

//Destructor
CTimer::~CTimer()
{

}

//Initialise Timer
bool CTimer::Initialise()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
	if (m_Frequency == 0)
	{
		return false;
	}
	
	m_TicksPerMS = (float)(m_Frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_StartTime);

	return true;
}

//Time Frame
void CTimer::Frame()
{
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)& currentTime);

	timeDifference = (float)(currentTime - m_StartTime);
	m_FrameTime = timeDifference / m_TicksPerMS;
	m_StartTime = currentTime;

	m_Time += m_FrameTime / 1000.0f;

	if (m_Time > 24.0f)
	{
		m_Time = 0.0f;
	}

	return;
}