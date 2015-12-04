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
	m_positionX += x;

	return;
}

void CCamera::MoveY(float y)
{
	m_positionY += y;

	return;
}

void CCamera::MoveZ(float z)
{
	m_positionZ += z;

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
	if (m_Input->KeyPressed(m_Input->Key_W)) { this->MoveZ(3.0f); }		//Move Forward
	if (m_Input->KeyPressed(m_Input->Key_A)) { this->MoveX(-3.0f); }	//Move Left
	if (m_Input->KeyPressed(m_Input->Key_S)) { this->MoveZ(-3.0f); }	//Move Down
	if (m_Input->KeyPressed(m_Input->Key_D)) { this->MoveX(3.0f); }		//Move Right
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

	//Generate View Matrix
	this->Render();

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

	//Update Camera World Matrix
	this->UpdateWorldMatrix(m_worldMatrix);

	return;
}

void CCamera::UpdateViewMatrix(D3DXMATRIX& viewMatrix) 
{
	viewMatrix = m_viewMatrix;
	return;
}

void CCamera::UpdateWorldMatrix(D3DXMATRIX& worldMatrix)
{
	float Pitch = this->m_rotationX * toRadians;
	float Yaw = this->m_rotationY * toRadians;
	float Roll = this->m_rotationZ * toRadians;

	//Temp Matrices
	D3DXMATRIX translation, rotationX, rotationY, rotationZ, scale;

	D3DXMatrixTranslation(&translation, this->m_positionX, this->m_positionY, this->m_positionZ);
	D3DXMatrixRotationX(&rotationX, Pitch);
	D3DXMatrixRotationY(&rotationY, Yaw);
	D3DXMatrixRotationZ(&rotationZ, Roll);
	D3DXMatrixIdentity(&scale);

	this->m_worldMatrix = scale * rotationZ * rotationX * rotationY * translation;
}


