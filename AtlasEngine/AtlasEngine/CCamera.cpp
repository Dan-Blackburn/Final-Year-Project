//Includes
#define NOMINMAX
#include "CCamera.h"

#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))

//Constructor
CCamera::CCamera() {

	//Initialise all Position and Rotation Values
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_prevMouseX = 0.0f;
	m_prevMouseY = 0.0f;
	
	D3DXMatrixIdentity(&m_viewMatrix);
	D3DXMatrixIdentity(&m_projectionMatrix);
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

	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;

	return;
}

void CCamera::SetRotation(float x, float y, float z) {

	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;

	return;
}

void CCamera::MoveX(float x)
{
	return;
}

void CCamera::MoveY(float y)
{
	float pitch = m_rotationX * (D3DX_PI / 180);
	float yaw = m_rotationY * (D3DX_PI / 180);

	m_positionX+= y * sinf(yaw) * cosf(pitch);
	m_positionY+= y * -sinf(pitch);
	m_positionZ+= y * cosf(yaw) * cosf(pitch);

	return;
}

void CCamera::MoveZ(float z)
{
	float pitch = m_rotationX * (D3DX_PI / 180);
	float yaw = m_rotationY * (D3DX_PI / 180);

	m_positionX+= z * sinf(yaw) * cosf(pitch);
	m_positionY+= z * -sinf(pitch);
	m_positionZ+= z * cosf(yaw) * cosf(pitch);

	return;
}

void CCamera::RotateX(float x)
{
	m_rotationX += x;

	if (m_rotationX > 89) { m_rotationX = 89; }
	if (m_rotationX < -89) { m_rotationX = -89; }
}

void CCamera::RotateY(float y)
{
	m_rotationY += y;

	while (m_rotationY > 180) { m_rotationY -= 360; }
	while (m_rotationY < -180) { m_rotationY += 360; }
}

void CCamera::RotateZ(float z)
{
	m_rotationY += z;
}

void CCamera::Frame(CInput* m_Input) 
{	
	//----- Key Movement -----//
	if (m_Input->KeyPressed(m_Input->Key_W)) { this->MoveZ(m_CameraSpeed); }	//Move Forward
	if (m_Input->KeyPressed(m_Input->Key_A)) { this->MoveX(-m_CameraSpeed); }	//Move Left
	if (m_Input->KeyPressed(m_Input->Key_S)) { this->MoveZ(-m_CameraSpeed); }	//Move Down
	if (m_Input->KeyPressed(m_Input->Key_D)) { this->MoveX(m_CameraSpeed); }	//Move Right
	if (m_Input->KeyPressed(m_Input->Key_Up)) { this->RotateX(-1.0f		* m_Sensitivity); }	//Turn Upward
	if (m_Input->KeyPressed(m_Input->Key_Down)) { this->RotateX(1.0f	* m_Sensitivity); }	//Turn Downward
	if (m_Input->KeyPressed(m_Input->Key_Left)) { this->RotateY(-1.0f	* m_Sensitivity); }	//Turn Left
	if (m_Input->KeyPressed(m_Input->Key_Right)) { this->RotateY(1.0f	* m_Sensitivity); }	//Turn Right

	//----- Mouse Movement -----//
	int mouseXPos, mouseYPos;
	m_Input->GetMouseLocation(mouseXPos, mouseYPos);

	float MovementX = mouseXPos - m_prevMouseX;
	float MovementY = mouseYPos - m_prevMouseY;

	m_prevMouseX = mouseXPos;
	m_prevMouseY = mouseYPos;

	this->RotateX(MovementY * m_Sensitivity);
	this->RotateY(MovementX * m_Sensitivity);

	return;
}

void CCamera::UpdateViewMatrix(D3DXMATRIX& viewMatrix)
{
	//Position and Rotation Variables
	D3DXVECTOR3 Up, Position, LookAt, Right;
	float Yaw, Pitch, Roll;

	D3DXMATRIX yawMatrix;
	D3DXMATRIX pitchMatrix;
	D3DXMATRIX rollMatrix;
	D3DXVECTOR3 Direction;

	//Setup Vector pointing Upwards
	Up.x = 0.0f; Up.y = 1.0f; Up.z = 0.0f;
	LookAt.x = 0.0f; LookAt.y = 0.0f; LookAt.z = 1.0f;
	Right.x = 1.0f; Right.y = 0.0f; Right.z = 0.0f;

	//Setup Camera Position in World
	Position.x = m_positionX;
	Position.y = m_positionY;
	Position.z = m_positionZ;

	//Set Yaw Pitch and Roll Rotations in Radians
	Pitch = m_rotationX * toRadians;
	Yaw = m_rotationY * toRadians;
	Roll = m_rotationZ * toRadians;

	//Direction Vector
	D3DXVec3Normalize(&Direction, &(LookAt - Right));

	D3DXMatrixRotationAxis(&yawMatrix, &Up, Yaw);
	D3DXMatrixRotationAxis(&pitchMatrix, &Right, Pitch);
	D3DXMatrixRotationAxis(&rollMatrix, &LookAt, Roll);

	//Move LookAt Vector
	D3DXVec3TransformCoord(&LookAt, &LookAt, &pitchMatrix);
	D3DXVec3TransformCoord(&LookAt, &LookAt, &yawMatrix);
	D3DXVec3Normalize(&LookAt, &LookAt);

	//Define Right Vector
	D3DXVec3Cross(&Right, &Up, &LookAt);
	D3DXVec3Normalize(&Right, &Right);

	//Define Up Vector
	D3DXVec3Cross(&Up, &LookAt, &Right);

	D3DXMatrixIdentity(&m_viewMatrix);

	m_viewMatrix._11 = Right.x;
	m_viewMatrix._21 = Right.y;
	m_viewMatrix._31 = Right.z;

	m_viewMatrix._12 = Up.x;
	m_viewMatrix._22 = Up.y;
	m_viewMatrix._32 = Up.z;

	m_viewMatrix._13 = LookAt.x;
	m_viewMatrix._23 = LookAt.y;
	m_viewMatrix._33 = LookAt.z; 

	m_viewMatrix._41 = -D3DXVec3Dot(&Position, &Right);
	m_viewMatrix._42 = -D3DXVec3Dot(&Position, &Up);
	m_viewMatrix._43 = -D3DXVec3Dot(&Position, &LookAt);

	mRightVect = Right;
	mLookVect = LookAt;
	viewMatrix = m_viewMatrix;

	return;
}

