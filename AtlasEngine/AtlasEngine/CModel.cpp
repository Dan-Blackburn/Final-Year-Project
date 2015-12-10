//Includes
#include "CModel.h"
#include <string>

using namespace std;

//Constructor
CModel::CModel() {
	m_Model = new ModelProperties;
	m_Model->Position.x = 0.0f;
	m_Model->Position.y = 0.0f;
	m_Model->Position.z = 0.0f;

	m_Model->Rotation.x = 0.0f;
	m_Model->Rotation.y = 0.0f;
	m_Model->Rotation.z = 0.0f;

	m_Model->Scale.x = 1.0f;
	m_Model->Scale.y = 1.0f;
	m_Model->Scale.z = 1.0f;

	UpdateWorldMatrix();
}

//Copy Constructor
CModel::CModel(const CModel& other) {
}

//Destructor
CModel::~CModel() {
}

/////////////////////////////////////////////////////////////
//Setters
/////////////////////////////////////////////////////////////

//Set new Position for Model
void CModel::SetPosition(float x, float y, float z) 
{
	m_Model->Position.x = x;
	m_Model->Position.y = y;
	m_Model->Position.z = z;

	return;
}

//Set new Rotation for Model
void CModel::SetRotation(float x, float y, float z) 
{
	m_Model->Rotation.x = x;
	m_Model->Rotation.y = y;
	m_Model->Rotation.z = z;

	return;
}

//Set new Scale for Model
void CModel::SetScale(float x, float y, float z)
{
	m_Model->Scale.x = x;
	m_Model->Scale.y = y;
	m_Model->Scale.z = z;

	return;
}

//Set new Model Type for Model
void CModel::SetModelType(string newType)
{
	if (newType == "Terrain") { m_Model->ModelType = CMesh::Terrain; return; }
	if (newType == "Water") { m_Model->ModelType = CMesh::Water; return; }
	if (newType == "Building") { m_Model->ModelType = CMesh::Building; return; }
	if (newType == "Object") { m_Model->ModelType = CMesh::Object; return; }
	if (newType == "Vehicle") { m_Model->ModelType = CMesh::Vehicle; return; }
	
	//Report when no type has been applied
	OutputDebugString("Unable to apply Model Type to: ");
	OutputDebugString(this->m_Model->ModelName.c_str());
	OutputDebugString("\n");
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
//Getters
/////////////////////////////////////////////////////////////

//Returns Model
CModel::ModelProperties* CModel::GetModel() {
	return m_Model;
}

//Returns Position of Model
D3DXVECTOR3 CModel::GetPosition() {
	return m_Model->Position;
}

//Returns Rotation of Model
D3DXVECTOR3 CModel::GetRotation() {
	return m_Model->Rotation;
}

/////////////////////////////////////////////////////////////

void CModel::UpdateWorldMatrix() {
	
	float Pitch = m_Model->Rotation.x * toRadians;
	float Yaw = m_Model->Rotation.y * toRadians;
	float Roll = m_Model->Rotation.z * toRadians;

	//Temp Matrices
	D3DXMATRIX translation, rotationX, rotationY, rotationZ, scale;

	D3DXMatrixTranslation(&translation, m_Model->Position.x, m_Model->Position.y, m_Model->Position.z);
	D3DXMatrixRotationX(&rotationX, Pitch);
	D3DXMatrixRotationY(&rotationY, Yaw);
	D3DXMatrixRotationZ(&rotationZ, Roll);
	D3DXMatrixScaling(&scale, m_Model->Scale.x, m_Model->Scale.y, m_Model->Scale.z);

	m_Model->WorldMatrix = scale * rotationZ * rotationX * rotationY * translation;

}


//Initialise Function
bool CModel::Initialise(ID3D11Device* device, string ModelLocation, string ModelFiletype) {

	bool result;

	//Initialise the Verex and Index buffers holding Geometry for Entities
	result = InitialiseBuffers(device, ModelLocation, ModelFiletype);
	if (!result) {
		return false;
	}

	return true;
}

//Shutdown Function for Buffers
void CModel::Shutdown() {

	//Release the Vertex and Index buffers
	ShutdownBuffers();

	return;
}

//Initialise Buffers Function 
bool CModel::InitialiseBuffers(ID3D11Device* device, string ModelLocation, string ModelFiletype) {

	m_Model->Mesh = new CMesh();

	//Load Model using Assimp
	bool LoadStatus = m_Model->Mesh->LoadMesh(device, this->m_Model->ModelName, this->m_Model->ModelType, ModelFiletype, ModelLocation);

	if (!LoadStatus) {
		OutputDebugString("Unable to Load Mesh");
		return false;
	}

	//Define Variables
	CMesh::VertexType* Vertices;
	unsigned int* Indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//Initialise Variables
	int m_vertexCount = m_Model->Mesh->GetVertexCount();
	int m_indexCount = m_Model->Mesh->GetIndexCount();

	//Vertex Array
	Vertices = new CMesh::VertexType[m_vertexCount];
	if (!Vertices) {
		return false;
	}

	//Index Array
	Indices = new unsigned int[m_indexCount];
	if (!Indices) {
		return false;
	}

	//Gets List of Sub-Meshes
	vector<CMesh::SubMesh*> SubMeshList = m_Model->Mesh->GetSubMeshList();

	//Counts through SubMeshes
	for (int currentSubMesh = 0; currentSubMesh < m_Model->Mesh->GetSubMeshNum(); currentSubMesh++) 
	{

		//Get SubMesh from List
		CMesh::SubMesh* subMesh = SubMeshList[currentSubMesh];

		//Get SubMesh's Vertices List
		vector<CMesh::VertexType*> verticesList = subMesh->VerticesList;

		//Iterates through SubMesh's Vertex List
		for (int currentVertex = 0; currentVertex < subMesh->m_vertexCount; currentVertex++) {
			CMesh::VertexType* subMeshVertex = verticesList[currentVertex];

			Vertices[currentVertex].position = subMeshVertex->position;
			Vertices[currentVertex].texture = subMeshVertex->texture;

			//Iterates through SubMesh's Index List
			for (int currentIndex = 0; currentIndex < subMesh->m_indexCount; currentIndex++) {
				unsigned int* subMeshIndex = &subMesh->Indices[currentIndex];
				Indices[currentIndex] = *subMeshIndex;
			}
		}

		//Set descriptions of Static Vetex Buffer
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(CMesh::VertexType) * subMesh->m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		//Give Substructure a Pointer to Vertex Data
		vertexData.pSysMem = Vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		//Create Vertex Buffer
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &subMesh->VertexBuffer);

		if (FAILED(result))
		{
			return false;
		}

		//Set descriptions for Static Index Buffer
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned int) * subMesh->m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		//Give Substructure a Pointer to Index Data
		indexData.pSysMem = Indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		//Create Index Buffer
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &subMesh->IndexBuffer);
		if (FAILED(result))
		{
			return false;
		}

	}
	//Release Arrays now Buffers are Created
	delete[] Vertices;
	Vertices = 0;

	delete[] Indices;
	Indices = 0;

	return true;
}

//Shutdown Buffers Function (Release Index and Vertex Buffer)
void CModel::ShutdownBuffers() {

	ModelProperties* currentModel = GetModel();

	//Get List of SubMeshs
	vector<CMesh::SubMesh*> subMeshList = currentModel->Mesh->GetSubMeshList();

	//Iterate through SubMeshes
	for (int subMeshCount = 0; subMeshCount < currentModel->Mesh->GetSubMeshNum(); subMeshCount++) {
		CMesh::SubMesh* currentMesh = subMeshList[subMeshCount];

		ID3D11Buffer* VertexBuffer = currentMesh->VertexBuffer;
		ID3D11Buffer* IndexBuffer = currentMesh->IndexBuffer;

		//Release Buffers
		if (IndexBuffer) {
			IndexBuffer->Release();
			IndexBuffer = 0;
		}

		if (VertexBuffer) {
			VertexBuffer->Release();
			VertexBuffer = 0;
		}

	}

	return;
}