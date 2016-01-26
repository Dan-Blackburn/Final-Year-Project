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
	
	inline D3DXMATRIX& getViewMatrix() { return m_viewMatrix; }
	inline D3DXMATRIX& getProjectionMatrix() { return m_projectionMatrix; }
	inline D3DXMATRIX& getRotationMatrix() { return m_rotationMatrix; }

	void Frame(CInput* m_Input, float frameTime);
	void UpdateViewMatrix(D3DXMATRIX&);

	const float toRadians = 0.0174532925f;
	const float m_CameraSpeed = 2.5f;

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_prevMouseX, m_prevMouseY;

	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_rotationMatrix;
	D3DXMATRIX m_projectionMatrix;

	D3DXVECTOR3 mRightVect, mLookVect;

	float m_Sensitivity = 0.2f;
};

#endif