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
	CLight() { Direction.x = 1.0f; Direction.y = 1.0f; Direction.z = 1.0f; LightColour.x = 1.0f; LightColour.y = 1.0f; LightColour.z = 1.0f; SpecularPower = 1.0f; }
	CLight(const CLight& other) {};
	~CLight() { };

	//Setters & Getters
	inline void SetPosition(float x, float y, float z) { Direction.x = x; Direction.y = y; Direction.z = z; }
	inline void SetColour(float r, float g, float b) { LightColour.x = r; LightColour.y = g; LightColour.z = b; }
	inline void SetType(std::string Type) { if (Type == "Ambient") LightType = Ambient; else if (Type == "Specular") LightType = Specular; else if (Type == "Diffuse") LightType = Diffuse; else { OutputDebugString("Error: Light type not found"); } }
	inline void SetPower(float power) { SpecularPower = power; }

	inline D3DXVECTOR3 GetDirection() { return Direction; }
	inline D3DXVECTOR4 GetColour() { return LightColour; }
	inline float GetSpecularPower() { return SpecularPower; }

	//Public Variables
	static enum eLightType { Ambient, Diffuse, Specular };

private:
	//Orientation
	D3DXVECTOR3 Direction;

	D3DXVECTOR4 LightColour;
	float SpecularPower;
	eLightType LightType;
};