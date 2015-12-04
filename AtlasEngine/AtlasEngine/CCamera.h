#pragma once

#ifndef _CCAMERA_H_
#define _CCAMERA_H_

//Includes
#include <d3dx10math.h>
#include "CInput.h"

//Camera Class
class CCamera
{
public:
	CCamera();
	CCamera(const CCamera&);
	~CCamera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void MoveX(float);
	void MoveY(float);
	void MoveZ(float);

	void RotateX(float);
	void RotateY(float);
	void RotateZ(float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Frame(CInput* m_Input);
	void Render();
	void UpdateViewMatrix(D3DXMATRIX&);
	void UpdateWorldMatrix(D3DXMATRIX&);

	const float toRadians = 0.0174532925f;

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_prevMouseX, m_prevMouseY;

	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_worldMatrix;

	float m_Sensitivity = 0.2f;
};

#endif