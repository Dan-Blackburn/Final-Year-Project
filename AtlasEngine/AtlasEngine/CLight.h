#pragma once
#include <d3d11.h>
#include <D3DX11tex.h>
#include <d3dx10math.h>
#include <iostream>

//Prototypes
enum eLightType;

class CLight
{
public:
	CLight() { Position.x = 1.0f; Position.y = 1.0f; Position.z = 1.0f; Position.w = 1.0f; Direction.x = 1.0f; Direction.y = 1.0f; Direction.z = 1.0f; LightColour.x = 1.0f; LightColour.y = 1.0f; LightColour.z = 1.0f; m_Angle = 90.0f; }
	CLight(const CLight& other) {};
	~CLight() { };

	//Setters & Getters
	inline void SetLightName(std::string lightName) { m_LightName = lightName; }
	inline void SetDirection(float x, float y, float z) { Direction.x = x; Direction.y = y; Direction.z = z; }
	inline void SetPosition(float x, float y, float z) { Position.x = x; Position.y = y; Position.z = z; }
	inline void SetConstantColour(float r, float g, float b, float a) { ConstantColour.x = r; ConstantColour.y = g; ConstantColour.z = b; ConstantColour.w = a; }
	inline void SetAngle(float angle) { m_Angle = angle; }
	inline void SetType(std::string Type) { if (Type == "Ambient") LightType = Ambient; else if (Type == "Point") LightType = Point; else if (Type == "Spotlight") LightType = Spotlight; else { OutputDebugString("Error: Light type not found"); } }
	inline void SetBrightness(D3DXVECTOR4 brightness) { Brightness = brightness; }
	inline void SetSpecularPower(D3DXVECTOR4 power) { SpecularPower = power; }

	inline std::string GetLightName() { return m_LightName; }
	inline D3DXVECTOR3 GetDirection() { return Direction; }
	inline D3DXVECTOR4 GetPosition() { return Position; }
	inline D3DXVECTOR4 GetColour() { return LightColour; }
	inline D3DXVECTOR4 GetConstantColour() { return ConstantColour; }
	inline float GetAngle() { return m_Angle; }
	inline int GetType() { return LightType; }
	inline D3DXVECTOR4 GetBrightness() { return m_CurrentBrightness; }
	inline D3DXVECTOR4 GetSpecularPower() { return SpecularPower; }

	void Frame(float frameTime, float clock, float sunAngle);

	//Public Variables
	enum eLightType { Ambient, Point, Spotlight };

private:
	//Orientation
	D3DXVECTOR3 Direction;
	D3DXVECTOR4 Position;
	std::string m_LightName;
	D3DXVECTOR4 LightColour;
	D3DXVECTOR4 ConstantColour;
	D3DXVECTOR4 Brightness;
	D3DXVECTOR4 m_CurrentBrightness;
	D3DXVECTOR4 SpecularPower;
	eLightType LightType;
	float m_Angle;
};