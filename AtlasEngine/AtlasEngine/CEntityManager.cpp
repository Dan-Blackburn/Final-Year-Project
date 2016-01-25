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
bool CEntityManager::InitialiseEntities(ID3D11Device* device) 
{
	bool result;

	//XML Variables
	tinyxml2::XMLDocument xmlDoc;
	xmlDoc.LoadFile("Resources/XML/Default.xml");

	/////////////////////////////////////////////////////////////
	//Model Initialisation
	/////////////////////////////////////////////////////////////

	//Get Number of Entities
	XMLElement* ModelEntityCount = xmlDoc.FirstChildElement("Level")->FirstChildElement("ModelEntities");
	ModelEntityCount->QueryIntAttribute("Number", &m_ModelEntityCount);
	XMLElement* ParticleSystemCount = xmlDoc.FirstChildElement("Level")->FirstChildElement("ParticleSystems");
	ParticleSystemCount->QueryIntAttribute("Number", &m_ParticleSystemCount);
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
			return false;
		}

		//---------- XML Entity Attribute ----------//
		XMLElement* Attributes = EntityAttributes->FirstChildElement("Name");
		//Set Name
		m_ModelEntity->SetModelName(Attributes->GetText());
		Attributes = Attributes->NextSiblingElement();
		//Set Model Type
		m_ModelEntity->SetModelType(Attributes->GetText());
		Attributes = Attributes->NextSiblingElement();

		//Check Entity List
		result = CheckEntityList();

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
		if (!result)
		{
			result = m_ModelEntity->Initialise(device, ModelLocation, ModelFiletype);
		}

		if (!result) 
		{
			OutputDebugString("Error: Unexpected error when initialising Model Object");
			return false;
		}
		EntityAttributes = EntityAttributes->NextSiblingElement();
	}
	if (result)
	{
		OutputDebugString("Success!\n");
	}

	/////////////////////////////////////////////////////////////
	//Particle Initialisation
	/////////////////////////////////////////////////////////////

	//Get Light Entity Attributes
	EntityAttributes = xmlDoc.FirstChildElement("Level")->FirstChildElement("ParticleSystems")->FirstChildElement("System");

	//Assign Light Attributes
	OutputDebugString("Adding Particle Systems...\n");
	for (int i = 0; i < m_ParticleSystemCount; i++)
	{
		//Create Particle System
		m_ParticleSystem = new CParticleSystem();
		string meshLocation;
		string meshFiletype;
		float floatValue;
		int intValue;

		//Check Pointer is Valid
		if (!m_ParticleSystem)
		{
			return false;
		}

		//---------- XML Entity Attribute ----------//
		XMLElement* Attributes = EntityAttributes->FirstChildElement("Name");
		//Set System Name
		m_ParticleSystem->SetName(Attributes->GetText());
		Attributes = Attributes->NextSiblingElement();
		//Set System Shader Name
		m_ParticleSystem->SetShaderName(Attributes->GetText());
		Attributes = Attributes->NextSiblingElement();
		//Set Particle Emitter Position
		D3DXVECTOR3 emitterPosition;
		Attributes->QueryFloatAttribute("X", &emitterPosition.x);
		Attributes->QueryFloatAttribute("Y", &emitterPosition.y);
		Attributes->QueryFloatAttribute("Z", &emitterPosition.z);
		m_ParticleSystem->SetEmitterPosition(emitterPosition);
		Attributes = Attributes->NextSiblingElement();
		//Set Size of Particle
		D3DXVECTOR3 particleSize;
		Attributes->QueryFloatAttribute("X", &particleSize.x);
		Attributes->QueryFloatAttribute("Y", &particleSize.y);
		Attributes->QueryFloatAttribute("Z", &particleSize.z);
		m_ParticleSystem->SetParticleSize(particleSize);
		Attributes = Attributes->NextSiblingElement();
		//Set Amount of Particles Per Second
		Attributes->QueryIntAttribute("Number", &intValue);
		m_ParticleSystem->SetParticlesPerSecond(intValue);
		Attributes = Attributes->NextSiblingElement();
		//Set Deviation of Particles when Emitted
		D3DXVECTOR3 particleDeviation;
		Attributes->QueryFloatAttribute("X", &particleDeviation.x);
		Attributes->QueryFloatAttribute("Y", &particleDeviation.y);
		Attributes->QueryFloatAttribute("Z", &particleDeviation.z);
		m_ParticleSystem->SetParticleDeviation(particleDeviation);
		Attributes = Attributes->NextSiblingElement();
		//Set Max Number of Particles
		Attributes->QueryIntAttribute("Number", &intValue);
		m_ParticleSystem->SetMaxParticles(intValue);
		Attributes = Attributes->NextSiblingElement();
		//Set Particle Velocity
		Attributes->QueryFloatAttribute("Number", &floatValue);
		m_ParticleSystem->SetVelocity(floatValue);
		Attributes = Attributes->NextSiblingElement();
		//Set Particle Velocity Variation
		Attributes->QueryFloatAttribute("Number", &floatValue);
		m_ParticleSystem->SetVelocityVariation(floatValue);
		Attributes = Attributes->NextSiblingElement();

		//Set Mesh Location
		meshLocation = Attributes->GetText();
		Attributes = Attributes->NextSiblingElement();
		//Set Mesh Filetype
		meshFiletype = Attributes->GetText();
		Attributes = Attributes->NextSiblingElement();
		//------------------------------------------//

		//Add Particle System to List
		m_ParticleSystemList.push_back(m_ParticleSystem);

		//Initialise Particle System
		result = m_ParticleSystem->Initialise(device, m_ParticleSystem->GetName(), meshFiletype, meshLocation);

		if (!result)
		{
			OutputDebugString("Error: Unexpected error when initialising Particle System");
			return false;
		}

		EntityAttributes = EntityAttributes->NextSiblingElement();
	}

	if (result)
	{
		OutputDebugString("Success!\n");
	}

	/////////////////////////////////////////////////////////////
	//Light Initialisation
	/////////////////////////////////////////////////////////////

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
			return false;
		}

		//---------- XML Entity Attribute ----------//
		XMLElement* Attributes = EntityAttributes->FirstChildElement("Name");
		//Set Light Name
		m_LightEntity->SetLightName(Attributes->GetText());
		Attributes = Attributes->NextSiblingElement();
		//Set Light Type
		m_LightEntity->SetType(Attributes->GetText());
		Attributes = Attributes->NextSiblingElement();
		//Set Light Colour
		float R, G, B, A;
		Attributes->QueryFloatAttribute("R", &R);
		Attributes->QueryFloatAttribute("G", &G);
		Attributes->QueryFloatAttribute("B", &B);
		Attributes->QueryFloatAttribute("A", &A);
		m_LightEntity->SetConstantColour(R, G, B, A);
		Attributes = Attributes->NextSiblingElement();
		if (m_LightEntity->GetType() == CLight::Ambient)
		{
			//Set Light Direction
			float X, Y, Z;
			Attributes->QueryFloatAttribute("X", &X);
			Attributes->QueryFloatAttribute("Y", &Y);
			Attributes->QueryFloatAttribute("Z", &Z);
			m_LightEntity->SetDirection(X, Y, Z);
			Attributes = Attributes->NextSiblingElement();
			//Set Starting Angle
			float angle;
			Attributes->QueryFloatAttribute("Angle", &angle);
			m_LightEntity->SetAngle(angle);
		}
		else if (m_LightEntity->GetType() == CLight::Point)
		{
			//Set Light Position
			float X, Y, Z;
			Attributes->QueryFloatAttribute("X", &X);
			Attributes->QueryFloatAttribute("Y", &Y);
			Attributes->QueryFloatAttribute("Z", &Z);
			m_LightEntity->SetPosition(X, Y, Z);
			Attributes = Attributes->NextSiblingElement();
			//Set Brightness
			float brightness;
			Attributes->QueryFloatAttribute("Value", &brightness);
			D3DXVECTOR4 Brightness = D3DXVECTOR4(brightness, 0.0f, 0.0f, 1.0f);
			m_LightEntity->SetBrightness(Brightness);
			Attributes = Attributes->NextSiblingElement();
		}
		//Set Specular Power
		float specularPower;
		Attributes->QueryFloatAttribute("Value", &specularPower);
		D3DXVECTOR4 SpecularPower = D3DXVECTOR4(specularPower, 0.0f, 0.0f, 1.0f);
		m_LightEntity->SetSpecularPower(SpecularPower);
		//------------------------------------------//

		m_LightList.push_back(m_LightEntity);

		EntityAttributes = EntityAttributes->NextSiblingElement();
	}
	if (result)
	{
		OutputDebugString("Success!\n");
	}

	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	//Camera Initialisation
	/////////////////////////////////////////////////////////////

	//Create Camera Entity
	m_CameraEntity = new CCamera();

	//Check Pointer is Valid
	if (!m_CameraEntity) 
	{
		OutputDebugString("Error: Unexpected error when defining Camera Object");
		return false;
	}

	//Push Camera Pointer to List
	m_CameraList.push_back(m_CameraEntity);

	/////////////////////////////////////////////////////////////

	return true;
}

//Render Entities
bool CEntityManager::RenderEntities(CDirect3D* direct3D, CShaderManager* ShaderManager, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) {
	bool result;
	ID3D11DeviceContext* deviceContext = direct3D->GetDeviceContext();
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

		if (m_ModelEntity->GetName() == "Stars Sphere") { direct3D->EnableAlphaBlending(); }

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
		if (m_ModelEntity->GetName() == "Stars Sphere") { direct3D->DisableAlphaBlending(); }
	}

	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	//Render ALL Entities within Particle List
	/////////////////////////////////////////////////////////////
	
	//Enable Alpha Blending
	direct3D->EnableAlphaBlending();


	for (std::vector<CParticleSystem*>::iterator it = m_ParticleSystemList.begin(); it != m_ParticleSystemList.end(); it++)
	{
		m_ParticleSystem = *it;

		string particleShaderName = m_ParticleSystem->GetShaderName();
		ShaderManager->SetCurrentShader(ShaderManager->GetShader(particleShaderName));

		m_ParticleSystem->Render(deviceContext);
		result = ShaderManager->RenderShader(deviceContext, m_ParticleSystem->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_ParticleSystem->GetTexture(), m_LightList, m_CameraEntity);
		if (!result) { OutputDebugString("Unable to Prepare Buffers\n"); return false; }
	}

	//DisableAlpha Blending
	direct3D->DisableAlphaBlending();

	return true;
}

bool CEntityManager::Frame(ID3D11DeviceContext* deviceContext, float frameTime, float clock, D3DXMATRIX worldMatrix) {
	
	/////////////////////////////////////////////////////////////
	//Frame Function - Entity Manipulation
	for (int i = 0; i < m_ModelEntityCount; i++)
	{
		if (m_ModelList[i]->GetName() == "Stars Sphere")
		{
			m_ModelList[i]->Frame(deviceContext, frameTime);
		}
	}

	for (int i = 0; i < m_ParticleSystemCount; i++)
	{
		m_ParticleSystemList[i]->Frame(deviceContext, frameTime, m_CameraEntity);
	}

	for (int i = 0; i < m_LightEntityCount; i++)
	{
		m_LightList[i]->Frame(frameTime, clock, m_LightList[0]->GetAngle());
	}

	/////////////////////////////////////////////////////////////

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

//Check Entire Entity list, Comparing Names
bool CEntityManager::CheckEntityList()
{
	for (std::vector<CModel*>::iterator it = m_ModelList.begin(); it != m_ModelList.end(); it++)
	{
		if ((m_ModelEntity->GetName() == (*it)->GetName()) && (m_ModelEntity->GetModel()->ModelType == (*it)->GetModel()->ModelType))
		{
			m_ModelEntity->GetModel()->Mesh = (*it)->GetModel()->Mesh;
			return true;
		}
	}

	return false;
}