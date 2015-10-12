#pragma once

#ifndef _CCAMERA_H_
#define _CCAMERA_H_

//Includes
#include <d3dx10math.h>

//Camera Class
class CCamera
{
public:
	CCamera();
	CCamera(const CCamera&);
	~CCamera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void UpdateViewMatrix(D3DXMATRIX&);

	const float toRadians = 0.0174532925f;

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
};

#endif