//Includes
#include "CCamera.h"

//Constructor
CCamera::CCamera() {

	//Initialise all Position and Rotation Values
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

}

//Copy Constructor
CCamera::CCamera(const CCamera& other) {
}

//Destructor
CCamera::~CCamera() {
}

//Getter Functions
D3DXVECTOR3 CCamera::GetPosition() {
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR3 CCamera::GetRotation() {
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

//Setter Functions
void CCamera::SetPosition(float x, float y, float z) {

	//Sidenote: Might split these into SetPositionX SetPositionY SetPositionZ
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;

	return;
}

void CCamera::SetRotation(float x, float y, float z) {

	//Sidenote: Might split these into SetRotationX SetRotationY SetRotationZ
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;

	return;
}

void CCamera::Render() {

	//Position and Rotation Variables
	D3DXVECTOR3 Up, Position, LookAt;
	float Yaw, Pitch, Roll;
	D3DXMATRIX rotationMatrix;

	//Setup Vector pointing Upwards
	Up.x = 0.0f;
	Up.y = 1.0f;
	Up.z = 0.0f;

	//Setup Camera Position in World
	Position.x = m_positionX;
	Position.y = m_positionY;
	Position.z = m_positionZ;

	//Setup where Camera is looking by Default
	LookAt.x = 0.0f;
	LookAt.y = 0.0f;
	LookAt.z = 1.0f;

	//Set Yaw Pitch and Roll Rotations in Radians
	Pitch = m_rotationX * toRadians;
	Yaw = m_rotationY * toRadians;
	Roll = m_rotationZ * toRadians;

	//Create Rotation Matrix from Yaw Pitch and Roll values
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, Yaw, Pitch, Roll);

	//Transform LookAt and Up vector by Rotation Matrix
	D3DXVec3TransformCoord(&LookAt, &LookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&Up, &Up, &rotationMatrix);

	//Translate rotated Camera Position to Location of Viewer
	LookAt = Position + LookAt;

	//Create View Matrix from Vectors
	D3DXMatrixLookAtLH(&m_viewMatrix, &Position, &LookAt, &Up);

	return;
}

void CCamera::UpdateViewMatrix(D3DXMATRIX& viewMatrix) {
	viewMatrix = m_viewMatrix;
	return;
}


