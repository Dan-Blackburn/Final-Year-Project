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
		/*mMatrices = new D3DXMATRIX[mMesh->getNumNodes()];
		D3DXMatrixScaling(scaleMatrix, mScale.x)
		D3DXMatrixRotationX(rotation)

		mMatrices0 = scale * rotz * royx * roty * translation;
		mMatrices1 = mMatrices[0] * mMesh->GetNode[1]->Transformation;*/
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

	/////////////////////////////////////////////////////////////
	//Render ALL Entities within Model List
	/////////////////////////////////////////////////////////////

	for (std::vector<CModel*>::iterator it = m_ModelList.begin(); it != m_ModelList.end(); it++) 
	{
		CModel* currentEntity = *it;

		CMesh* Mesh = currentEntity->GetModel()->Mesh;
		vector<CMesh::SubMesh*> subMeshList = Mesh->GetSubMeshList();

		for (int i = 0; i < Mesh->GetSubMeshNum(); i++) 
		{
			//Prepare Buffers for Rendering
			result = Mesh->PrepareBuffers(deviceContext, i);

			//Check if Buffer Preparation Fails
			if (!result)
			{
				OutputDebugString("Unable to Prepare Buffers\n");
				return false;
			}

			currentEntity->Update();
			//Render Diffuse Textures
			result = m_Shader->Render(deviceContext, Mesh->GetIndexCount(), currentEntity->GetModel()->WorldMatrix, viewMatrix, projectionMatrix, Mesh->GetTextures(subMeshList[i]));

			//Check if Entity fails to Render
			if (!result) 
			{
				OutputDebugString("Unable to Render Entity\n");
				return false;
			}

		}
	}

	/////////////////////////////////////////////////////////////

	return true;
}

bool CEntityManager::Frame() {
	
	/////////////////////////////////////////////////////////////
	//Frame Function - Entity Manipulation
	/////////////////////////////////////////////////////////////

	//Iterate through "UNORDERED" Model List - (Testing Only)
	for (std::vector<CModel*>::iterator it = m_ModelList.begin(); it != m_ModelList.end(); it++) 
	{
		m_ModelEntity = *it;

		m_ModelEntity->SetPosition(0.0f, 0.0f, 200.0f);
		m_ModelEntity->SetRotation(0.0f, m_ModelEntity->GetRotation().y + 0.1f, 0.0f);
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