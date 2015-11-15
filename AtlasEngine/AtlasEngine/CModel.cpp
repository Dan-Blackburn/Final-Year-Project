//Includes
#include "CModel.h"

//Constructor
CModel::CModel() {
	m_Model = new ModelProperties;
	m_Model->Position.x = 0.0f;
	m_Model->Position.y = 0.0f;
	m_Model->Position.z = 0.0f;

	m_Model->Rotation.x = 0.0f;
	m_Model->Rotation.y = 0.0f;
	m_Model->Rotation.z = 0.0f;
}

//Copy Constructor
CModel::CModel(const CModel& other) {
}

//Destructor
CModel::~CModel() {
}

//Setter
void CModel::SetPosition(float x, float y, float z) 
{
	m_Model->Position.x = x;
	m_Model->Position.y = y;
	m_Model->Position.z = z;

	return;
}

void CModel::SetRotation(float x, float y, float z) 
{
	m_Model->Rotation.x = x;
	m_Model->Rotation.y = y;
	m_Model->Rotation.z = z;

	return;
}

//Getters
CModel::ModelProperties* CModel::GetModel() {
	return m_Model;
}

D3DXVECTOR3 CModel::GetPosition() {
	return m_Model->Position;
}

D3DXVECTOR3 CModel::GetRotation() {
	return m_Model->Rotation;
}

//Initialise Function
bool CModel::Initialise(ID3D11Device* device, int currentModel) {

	bool result;

	//Initialise the Verex and Index buffers holding Geometry for Entities
	result = InitialiseBuffers(device, currentModel);
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
bool CModel::InitialiseBuffers(ID3D11Device* device, int currentModel) {

	m_Model->Mesh = new CMesh();
	std::string filename;
	std::string filetype;
	CMesh::eModelType modeltype;


	if (currentModel == 0) {
		//Temp Variables
		filename = "Flat Plane";
		filetype = ".fbx";
		modeltype = CMesh::Terrain;
	}
	else {
		//Temp Variables
		filename = "house";
		filetype = ".fbx";
		modeltype = CMesh::Building;
	}

	//Load Model using Assimp
	bool LoadStatus = m_Model->Mesh->LoadMesh(device, filename, modeltype, filetype);

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
	std::vector<CMesh::SubMesh*> SubMeshList = m_Model->Mesh->GetSubMeshList();

	//Counts through SubMeshes
	for (int currentSubMesh = 0; currentSubMesh < m_Model->Mesh->GetSubMeshNum(); currentSubMesh++) 
	{

		//Get SubMesh from List
		CMesh::SubMesh* subMesh = SubMeshList[currentSubMesh];

		//Get SubMesh's Vertices List
		std::vector<CMesh::VertexType*> verticesList = subMesh->VerticesList;

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
	std::vector<CMesh::SubMesh*> subMeshList = currentModel->Mesh->GetSubMeshList();

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