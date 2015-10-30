//Includes
#include "CModel.h"

//Constructor
CModel::CModel() {
	Mesh = 0;
}

//Copy Constructor
CModel::CModel(const CModel& other) {
}

//Destructor
CModel::~CModel() {
}

//Initialise Function
bool CModel::Initialise(ID3D11Device* device) {

	bool result;

	//Initialise the Verex and Index buffers holding Geometry for Triangle
	result = InitialiseBuffers(device);
	if (!result) {
		return false;
	}

	return true;
}

CMesh* CModel::GetMesh() {
	return Mesh;
}

//Shutdown Function for Buffers
void CModel::Shutdown() {

	//Release the Vertex and Index buffers
	ShutdownBuffers();

	return;
}

//Render Function (Called from CGraphics)
void CModel::Render(ID3D11DeviceContext* deviceContext) {
	
	//Add Buffers to Graphics Pipeline
	RenderBuffers(deviceContext);

	return;
}

//Initialise Buffers Function 
bool CModel::InitialiseBuffers(ID3D11Device* device) {

	Mesh = new CMesh();

	//Temp Variables
	std::string filename = "stop_sign";
	std::string filetype = ".obj";
	CMesh::eModelType modeltype = CMesh::Building;

	//Load Model using Assimp
	bool LoadStatus = Mesh->LoadMesh(filename, modeltype, filetype);

	if (!LoadStatus) {
		return false;
		//Submit Error Report Code
	}

	//Define Variables
	CMesh::VertexType* Vertices;
	unsigned int* Indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//Initialise Variables
	int m_vertexCount = Mesh->GetVertexCount();
	int m_indexCount = Mesh->GetIndexCount();

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
	std::vector<CMesh::SubMesh*> SubMeshList = Mesh->GetSubMeshList();

	//Counts through SubMeshes
	for (int currentSubMesh = 0; currentSubMesh < Mesh->GetSubMeshNum(); currentSubMesh++) {

		//Get SubMesh from List
		CMesh::SubMesh* subMesh = SubMeshList[currentSubMesh];

		//Get SubMesh's Vertices List
		std::vector<CMesh::VertexType*> verticesList = subMesh->VerticesList;

		//Iterates through SubMesh's Vertex List
		for (int currentVertex = 0; currentVertex < subMesh->m_vertexCount; currentVertex++) {
			CMesh::VertexType* subMeshVertex = verticesList[currentVertex];

			Vertices[currentVertex].position = subMeshVertex->position;
			Vertices[currentVertex].color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
		}

		//Iterates through SubMesh's Index List
		for (int currentIndex = 0; currentIndex < subMesh->m_indexCount; currentIndex++) {
			unsigned int* subMeshIndex = &subMesh->Indices[currentIndex];
			Indices[currentIndex] = *subMeshIndex;
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

//Render Buffers Function
void CModel::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	unsigned int stride;
	unsigned int offset;

	//Get List of SubMeshs
	std::vector<CMesh::SubMesh*> subMeshList = Mesh->GetSubMeshList();

	//Iterate through SubMeshes
	for (int subMeshCount = 0; subMeshCount < Mesh->GetSubMeshNum(); subMeshCount++) {
		CMesh::SubMesh* currentMesh = subMeshList[subMeshCount];

		ID3D11Buffer* VertexBuffer = currentMesh->VertexBuffer;
		ID3D11Buffer* IndexBuffer = currentMesh->IndexBuffer;

		//Set Vertex Buffer
		stride = sizeof(CMesh::VertexType);
		offset = 0;

		//Activate Vertex Buffer in Input Assembler
		deviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

		//Activate Index Buffer in Input Assembler
		deviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	return;
}


//Shutdown Buffers Function (Release Index and Vertex Buffer)
void CModel::ShutdownBuffers() {

	//Get List of SubMeshs
	std::vector<CMesh::SubMesh*> subMeshList = Mesh->GetSubMeshList();

	//Iterate through SubMeshes
	for (int subMeshCount = 0; subMeshCount < Mesh->GetSubMeshNum(); subMeshCount++) {
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