#include "CLight.h"

//Constructors & Destructors

//Frame
void CLight::Frame(float frameTime, float clock)
{
	float rotationRate = 0.0002f * frameTime;
	m_Angle+= rotationRate * 100;

	float colourRate = rotationRate * 0.75f;

	float radian = m_Angle * (D3DX_PI / 180);

	Direction.x = -cos(radian);
	Direction.y = -sin(radian);
	
	if (m_LightName == "Sun")
	{
		LightColour.x = sin(radian);
		LightColour.y = sin(radian);
		LightColour.z = sin(radian);
	}

	if (m_LightName == "Moon")
	{
		if (-sin(radian) > 0.4)
		{
			LightColour.x = -sin(radian);
			LightColour.y = -sin(radian);
			LightColour.z = -sin(radian);
		}

		else
		{
			LightColour.x = 0.4f;
			LightColour.y = 0.4f;
			LightColour.z = 0.4f;
		}
	}

	if (m_Angle > 360.0f)
	{
		m_Angle = 0.0f;
	}
}