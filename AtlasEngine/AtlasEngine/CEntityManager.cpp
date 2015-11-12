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

	for (int i = 0; i < 2; i++) {

		//Create Model Entity
		m_ModelEntity = new CModel();

		//Check Pointer is Valid
		if (!m_ModelEntity) {
			return ModelPointerError;
		}

		//Push Model Pointer to List
		m_ModelList.push_back(m_ModelEntity);
		IncreaseEntityCount(1);

		//Intialise Model Entity
		result = m_ModelEntity->Initialise(device, i);

		//Checks if Entity Initialised Correctly
		if (!result) {
			//Return Error Code
			return ModelInitialisationError;
		}

		else {
			OutputDebugString("Model Entity Added\n");
		}
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
		//Return Error Code
		return CameraPointerError;
	}

	else {
		OutputDebugString("Camera Entity Added\n");
	}

	//Push Camera Pointer to List
	m_CameraList.push_back(m_CameraEntity);

	/////////////////////////////////////////////////////////////

	return true;
}

//Render Entities
bool CEntityManager::RenderEntities(ID3D11DeviceContext* deviceContext, CShader* m_Shader, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) {
	bool result;

	for (std::vector<CModel*>::iterator it = m_ModelList.begin(); it != m_ModelList.end(); it++) 
	{
		CModel* currentEntity = *it;

		CMesh* Mesh = currentEntity->GetModel()->Mesh;
		vector<CMesh::SubMesh*> subMeshList = Mesh->GetSubMeshList();

		//Frame Function?
		D3DXVECTOR3 coords = currentEntity->GetModel()->Rotation;
		currentEntity->SetRotation(coords.x, coords.y + 0.01f, coords.z);

		for (int i = 0; i < Mesh->GetSubMeshNum(); i++) 
		{
			result = Mesh->PrepareBuffers(deviceContext, i);

			//Render Diffuse Only
			result = m_Shader->Render(deviceContext, Mesh->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, Mesh->GetTexture(subMeshList[i], CMesh::Diffuse));
		}

		if (!result) {
			return false;
		}
	}

	return true;
}

//Return Camera Entity
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