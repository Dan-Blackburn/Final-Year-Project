#include "CEntityManager.h"

//Constructor
CEntityManager::CEntityManager() {
	m_ModelEntity = 0;
	m_CameraEntity = 0;
}

//Copy Constructor
CEntityManager::CEntityManager(const CEntityManager &other) {

}

//Destructor
CEntityManager::~CEntityManager() {

}

//Getters & Setters
int CEntityManager::GetEntityCount() {
	return m_EntityCount;
}

void CEntityManager::IncreaseEntityCount(int byValue) {
	m_EntityCount+= byValue;
}

void CEntityManager::DecreaseEntityCount(int byValue) {
	m_EntityCount -= byValue;
}

//Functions
int CEntityManager::InitialiseEntities(ID3D11Device* device) {

	/////////////////////////////////////////////////////////////
	//Model Initialisation
	/////////////////////////////////////////////////////////////

	//Error Code Variables
	int ModelPointerError = 149;
	int ModelInitialisationError = 150;
	bool result;

	//Create Model Entity
	m_ModelEntity = new CModel();

	//Check Pointer is Valid
	if (!m_ModelEntity) {
		return ModelPointerError;
	}

	//Push Model Pointer to List
	m_ModelList.push_back(m_ModelEntity);
	
	//Intialise Model Entity
	result = m_ModelEntity->Initialise(device);

	//Checks if Entity Initialised Correctly
	if (!result) {
		//Debug Dump File
		return ModelInitialisationError;
	}

	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	//Camera Initialisation
	/////////////////////////////////////////////////////////////

	//Error Code Variables
	int CameraPointerError = 249;

	//Create Camera Entity
	m_CameraEntity = new CCamera();

	//Check Pointer is Valid
	if (!m_CameraEntity) {
		return CameraPointerError;
	}

	//Push Camera Pointer to List
	m_CameraList.push_back(m_CameraEntity);

	//Set Position of Camera
	m_CameraEntity->SetPosition(0.0f, 0.0f, -10.0f);

	/////////////////////////////////////////////////////////////

	return true;
}

void CEntityManager::PrepareEntities(ID3D11DeviceContext* deviceContext) {

	//Iterate through Entity List
	for (std::vector<CModel*>::iterator it = m_ModelList.begin(); it != m_ModelList.end(); it++) {

		CModel* currentEntity = *it;

		//Render Current Entity in Iteration
		currentEntity->Render(deviceContext);
	}

}

bool CEntityManager::RenderEntities(ID3D11DeviceContext* deviceContext, CShader* m_Shader, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) {
	bool result;

	for (std::vector<CModel*>::iterator it = m_ModelList.begin(); it != m_ModelList.end(); it++) {
		CModel* currentEntity = *it;

		result = m_Shader->Render(deviceContext, currentEntity->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);

		if (!result) {
			return false;
		}
	}

	return true;
}

CCamera* CEntityManager::GetCameraEntity() {
	return m_CameraEntity;
}

//Shutdown and Release Entities
void CEntityManager::Shutdown() {

	while(m_EntityCount > 0) {
		CModel* temp = m_ModelList.back();
		temp->Shutdown();
		m_ModelList.pop_back();
		DecreaseEntityCount(1);
	}
}