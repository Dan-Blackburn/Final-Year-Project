//Includes
#include "CModel.h"

//Constructor
CModel::CModel() {

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

	Mesh = new CMesh;

	//Temp Variables
	std::string filename = "building";
	std::string filetype = ".obj";
	CMesh::eModelType modeltype = CMesh::Building;

	//Load Model using Assimp
	bool LoadStatus = Mesh->LoadMesh(filename, modeltype, filetype);



	if (!LoadStatus) {
		//Submit Error Report Code
	}

	//Define Variables
	/*VertexType* vertices;
	unsigned long* indices;*/
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	/*Initialise Variables
	m_vertexCount = 3;
	m_indexCount = 3;*/

	/*Vertex Array
	vertices = new VertexType[m_vertexCount];
	if (!vertices) {
		return false;
	}

	//Index Array
	indices = new unsigned long[m_indexCount];
	if (!indices) {
		return false;
	}

	//Load Arrays with Data
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  //Bottom left
	vertices[0].color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  //Top middle
	vertices[1].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  //Bottom right
	vertices[2].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	//Load the index array with data
	indices[0] = 0;  //Bottom left
	indices[1] = 1;  //Top middle
	indices[2] = 2;  //Bottom right

	//Set descriptions of Static Vetex Buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(CMesh::VertexType) * Mesh->GetVertexCount();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give Substructure a Pointer to Vertex Data
	vertexData.pSysMem = Mesh->GetVertexData();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create Vertex Buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Set descriptions for Static Index Buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Give Substructure a Pointer to Index Data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create Index Buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Release Arrays now Buffers are Created
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
	*/
	return true;
}

//Render Buffers Function
void CModel::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	unsigned int stride;
	unsigned int offset;

	//Set Vertex Buffer
	stride = sizeof(CMesh::VertexType);
	offset = 0;

	//Activate Vertex Buffer in Input Assembler
	deviceContext->IASetVertexBuffers(0, 1, Mesh->GetVertexBuffer(), &stride, &offset);

	//Activate Index Buffer in Input Assembler
	deviceContext->IASetIndexBuffer(Mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


//Shutdown Buffers Function (Release Index and Vertex Buffer)
void CModel::ShutdownBuffers() {

	//Release Buffers
	if (m_indexBuffer) {
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer) {
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}