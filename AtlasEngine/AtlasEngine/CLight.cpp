#include "CLight.h"

//Constructors & Destructors

//Frame Function (Called each frame to manipulate entities)
void CLight::Frame(float frameTime, float clock, float sunAngle)
{
	//Global Variables
	float Sunrise = 0.0f;
	float Sunset = 180.0f;

	float rotationRate = 0.02f * frameTime;
	m_Angle += rotationRate;

	float radian = m_Angle * (D3DX_PI / 180);

	switch (LightType)
	{
	case Ambient:
		Direction.x = cos(radian);
		Direction.y = sin(radian);

		if (-sin(radian) > ConstantColour.x) { LightColour.x = -sin(radian); }
		else { LightColour.x = ConstantColour.x; }

		if (-sin(radian) > ConstantColour.y) { LightColour.y = -sin(radian); }
		else { LightColour.y = ConstantColour.y; }

		if (-sin(radian) > ConstantColour.z) { LightColour.z = -sin(radian); }
		else { LightColour.z = ConstantColour.z; }
	break;

	case Point:
		if (sunAngle < Sunset)
		{
			m_CurrentBrightness = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			m_CurrentBrightness = Brightness;
		}
	break;
	}

	if (m_Angle > 360.0f) { m_Angle = 0.0f; }

}