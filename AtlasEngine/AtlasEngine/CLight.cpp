#include "CLight.h"

//Constructors & Destructors

//Frame
void CLight::Frame(float frameTime, float clock)
{
	float rotationRate = 0.02f * frameTime;
	m_Angle += rotationRate;

	float radian = m_Angle * (D3DX_PI / 180);

	Direction.x = -cos(radian);
	Direction.y = -sin(radian);
	
	if (-sin(radian) > ConstantColour.x)
	{
		LightColour.x = -sin(radian);
	}

	else
	{
		LightColour.x = ConstantColour.x;
	}

	if (-sin(radian) > ConstantColour.y)
	{
		LightColour.y = -sin(radian);
	}

	else
	{
		LightColour.y = ConstantColour.y;
	}

	if (-sin(radian) > ConstantColour.z)
	{
		LightColour.z = -sin(radian);
	}

	else
	{
		LightColour.z = ConstantColour.z;
	}

	if (m_Angle > 360.0f)
	{
		m_Angle = 0.0f;
	}
}