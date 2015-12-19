#include "CEntityManager.h"
#include "tinyxml2.h"

using namespace tinyxml2;

//XML Error Checking
#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

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
int CEntityManager::GetModelEntityCount() {
	return m_ModelEntityCount;
}

int CEntityManager::GetLightEntityCount() {
	return m_LightEntityCount;
}

//Functions
int CEntityManager::InitialiseEntities(ID3D11Device* device) {

	/////////////////////////////////////////////////////////////
	//Model Initialisation
	/////////////////////////////////////////////////////////////

	//Error Code Variables
	int ModelPointerError = 149;
	int ModelInitialisationError = 150;
	int LightPointerError = 151;
	int LightInitialisationError = 152;
	bool result;

	//XML Variables
	tinyxml2::XMLDocument xmlDoc;
	xmlDoc.LoadFile("Resources/XML/Default.xml");

	//Get Number of Entities
	XMLElement* ModelEntityCount = xmlDoc.FirstChildElement("Level")->FirstChildElement("ModelEntities");
	ModelEntityCount->QueryIntAttribute("Number", &m_ModelEntityCount);
	XMLElement* LightEntityCount = xmlDoc.FirstChildElement("Level")->FirstChildElement("LightEntities");
	LightEntityCount->QueryIntAttribute("Number", &m_LightEntityCount);

	//Get Model Entity Attributes
	XMLElement* EntityAttributes = xmlDoc.FirstChildElement("Level")->FirstChildElement("ModelEntities")->FirstChildElement("Entity");

	OutputDebugString("Adding Model Entities...\n");
	//Assign Model Attributes
	for (int i = 0; i < m_ModelEntityCount; i++) 
	{
		//Create Model Entity
		m_ModelEntity = new CModel();

		//Check Pointer is Valid
		if (!m_ModelEntity) {
			OutputDebugString("Error: Unexpected error when defining Model Object");
			return ModelPointerError;
		}

		//---------- XML Entity Attribute ----------//
		XMLElement* Attributes = EntityAttributes->FirstChildElement("Name");
		//Set Name
		m_ModelEntity->SetModelName(Attributes->GetText());
		Attributes = Attributes->NextSiblingElement();
		//Set Model Type
		m_ModelEntity->SetModelType(Attributes->GetText());
		Attributes = Attributes->NextSiblingElement();
		//Set Position
		Attributes->QueryFloatAttribute("X", &m_ModelEntity->GetModel()->Position.x);
		Attributes->QueryFloatAttribute("Y", &m_ModelEntity->GetModel()->Position.y);
		Attributes->QueryFloatAttribute("Z", &m_ModelEntity->GetModel()->Position.z);
		Attributes = Attributes->NextSiblingElement();
		//Set Rotation
		Attributes->QueryFloatAttribute("X", &m_ModelEntity->GetModel()->Rotation.x);
		Attributes->QueryFloatAttribute("Y", &m_ModelEntity->GetModel()->Rotation.y);
		Attributes->QueryFloatAttribute("Z", &m_ModelEntity->GetModel()->Rotation.z);
		Attributes = Attributes->NextSiblingElement();
		//Set Scale
		Attributes->QueryFloatAttribute("X", &m_ModelEntity->GetModel()->Scale.x);
		Attributes->QueryFloatAttribute("Y", &m_ModelEntity->GetModel()->Scale.y);
		Attributes->QueryFloatAttribute("Z", &m_ModelEntity->GetModel()->Scale.z);
		Attributes = Attributes->NextSiblingElement();
		//Set Shader Name
		m_ModelEntity->SetShaderName(Attributes->GetText());
		Attributes = Attributes->NextSiblingElement();
		//Get Model Location
		string ModelLocation = Attributes->GetText();
		Attributes = Attributes->NextSiblingElement();
		//Get Model Filetype
		string ModelFiletype = Attributes->GetText();
		Attributes = Attributes->NextSiblingElement();
		//------------------------------------------//

		//Push Model Pointer to List
		m_ModelList.push_back(m_ModelEntity);

		//Intialise Model Entity
		result = m_ModelEntity->Initialise(device, ModelLocation, ModelFiletype);

		if (!result) 
		{
			OutputDebugString("Error: Unexpected error when initialising Model Object");
			return ModelInitialisationError;
		}
		EntityAttributes = EntityAttributes->NextSiblingElement();
	}
	if (result)
	{
		OutputDebugString("Success!\n");
	}


	//Get Light Entity Attributes
	EntityAttributes = xmlDoc.FirstChildElement("Level")->FirstChildElement("LightEntities")->FirstChildElement("Entity");

	//Assign Light Attributes
	OutputDebugString("Adding Light Entities...\n");
	for (int i = 0; i < m_LightEntityCount; i++)
	{
		//Create Light Entity
		m_LightEntity = new CLight();

		//Check Pointer is Valid
		if (!m_LightEntity) {
			return LightPointerError;
		}

		//---------- XML Entity Attribute ----------//
		XMLElement* Attributes = EntityAttributes->FirstChildElement("Light");
		//Set Light Type
		m_LightEntity->SetType(Attributes->GetText());
		Attributes = Attributes->NextSiblingElement();
		//Set Light Colour
		float R, G, B, A;
		Attributes->QueryFloatAttribute("R", &R);
		Attributes->QueryFloatAttribute("G", &G);
		Attributes->QueryFloatAttribute("B", &B);
		Attributes->QueryFloatAttribute("A", &A);
		m_LightEntity->SetColour(R, G, B, A);
		Attributes = Attributes->NextSiblingElement();
		//Set Light Direction
		float X, Y, Z;
		Attributes->QueryFloatAttribute("X", &X);
		Attributes->QueryFloatAttribute("Y", &Y);
		Attributes->QueryFloatAttribute("Z", &Z);
		m_LightEntity->SetDirection(X, Y, Z);
		Attributes = Attributes->NextSiblingElement();
		//------------------------------------------//

		m_LightList.push_back(m_LightEntity);
	}
	if (result)
	{
		OutputDebugString("Success!\n");
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
	if (!m_CameraEntity) 
	{
		OutputDebugString("Error: Unexpected error when defining Camera Object");
		return CameraPointerError;
	}

	//Push Camera Pointer to List
	m_CameraList.push_back(m_CameraEntity);

	/////////////////////////////////////////////////////////////

	return true;
}

//Render Entities
bool CEntityManager::RenderEntities(ID3D11DeviceContext* deviceContext, CShaderManager* ShaderManager, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) {
	bool result;

	/////////////////////////////////////////////////////////////
	//Render ALL Entities within Model List
	/////////////////////////////////////////////////////////////

	if (ShaderManager->GetShaderCount() == 0)
	{
		OutputDebugString("Error: Unable to detect Shaders\n");
		return false;
	}

	for (std::vector<CModel*>::iterator it = m_ModelList.begin(); it != m_ModelList.end(); it++) 
	{
		m_ModelEntity = *it;

		CMesh* Mesh = m_ModelEntity->GetModel()->Mesh;
		vector<CMesh::SubMesh*> subMeshList = Mesh->GetSubMeshList();

		string modelShaderName = m_ModelEntity->GetShaderName();
		ShaderManager->SetCurrentShader(ShaderManager->GetShader(modelShaderName));

		for (int i = 0; i < Mesh->GetSubMeshNum(); i++) 
		{
			//Prepare Buffers for Rendering
			result = Mesh->PrepareBuffers(deviceContext, i);

			//Check if Buffer Preparation Fails
			if (!result){ OutputDebugString("Unable to Prepare Buffers\n"); return false; }

			m_ModelEntity->UpdateWorldMatrix();

			//Render Diffuse Textures
			result = ShaderManager->RenderShader(deviceContext, Mesh->GetIndexCount(), m_ModelEntity->GetModel()->WorldMatrix, viewMatrix, projectionMatrix, Mesh->GetTextures(subMeshList[i]), m_LightList, m_CameraEntity);

			//Check if Entity fails to Render
			if (!result) { OutputDebugString("Unable to Render Entity\n"); return false; }

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

		//m_ModelEntity->SetRotation(m_ModelEntity->GetRotation().x + 0.2f, 0.0f , 0.0f);
	}

	m_LightEntity = *m_LightList.begin();

	if (m_LightEntity->GetDirection().y > -50.0f)
	{
		m_LightEntity->SetDirection(m_LightEntity->GetDirection().x, m_LightEntity->GetDirection().y - 1.0f, m_LightEntity->GetDirection().z);
	}

	if (m_LightEntity->GetDirection().y < -50.0f)
	{
		m_LightEntity->SetDirection(m_LightEntity->GetDirection().x, 50.0f, m_LightEntity->GetDirection().z);
	}

	return true;
}

//Return Camera Entity
CCamera* CEntityManager::GetCameraEntity() {
	return m_CameraEntity;
}

//Shutdown and Release Entities
void CEntityManager::Shutdown() {

	while(m_ModelList.size() > 0) {
		CModel* temp = m_ModelList.back();
		temp->Shutdown();
		m_ModelList.pop_back();
	}
	while(m_LightList.size() > 0) {
		CLight* temp = m_LightList.back();
		delete temp;
		m_LightList.pop_back();
	}
}