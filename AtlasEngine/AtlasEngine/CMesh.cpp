//Includes
#include "CMesh.h"
#include <Importer.hpp>
#include <Scene.h>
#include <PostProcess.h>

//Constructor
CMesh::CMesh() {
	totalIndexCount = 0;
	totalVertexCount = 0;
}

//Copy Constructor
CMesh::CMesh(const CMesh& other) {
}

//Destructor
CMesh::~CMesh() {
}

//Load Mesh from File
bool CMesh::LoadMesh(ID3D11Device* device, std::string mFileName, eModelType modelType, std::string mFileType) 
{

	/////////////////////////////////
	//Load Mesh through Assimp
	/////////////////////////////////

	//Mesh Location Handler
	std::string mFilePath = MeshFinder(modelType);
	std::string fileName = mFileName + mFileType;

	//Create Instance of Importer
	Assimp::Importer importer;

	const aiScene* Scene = importer.ReadFile(mFilePath + fileName, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	//Error Handler if Failed
	if (!Scene) 
	{
		//Error Message?
		return false;
	}

	/////////////////////////////////
	//Process Scene to Mesh
	/////////////////////////////////

	numSubMeshes = Scene->mNumMeshes;


	//Store Sub-Meshes of Mesh
	for (int i = 0; i < numSubMeshes; i++) 
	{
		SubMesh* subMesh = new SubMesh;

		//Store Vertex & Index Count for Loops
		subMesh->m_vertexCount = Scene->mMeshes[i]->mNumVertices;
		subMesh->m_indexCount = Scene->mMeshes[i]->mNumFaces * 3;
		subMesh->m_facesCount = Scene->mMeshes[i]->mNumFaces;
		totalVertexCount += subMesh->m_vertexCount;
		totalIndexCount += subMesh->m_indexCount;

		//Store Vertices of Sub-Mesh
		for (int j = 0; j < subMesh->m_vertexCount; j++) 
		{
			VertexType* Vertices = new VertexType;

			//Store Positions (XYZ)
			Vertices->position.x = Scene->mMeshes[i]->mVertices[j].x;
			Vertices->position.y = Scene->mMeshes[i]->mVertices[j].y;
			Vertices->position.z = Scene->mMeshes[i]->mVertices[j].z;

			Vertices->texture.x = Scene->mMeshes[i]->mTextureCoords[Diffuse][j].x;
			Vertices->texture.y = Scene->mMeshes[i]->mTextureCoords[Diffuse][j].y;

			subMesh->VerticesList.push_back(Vertices);
		}

		//Create Index
		subMesh->Indices = new unsigned int[subMesh->m_indexCount];

		int indexCount = 0;
		
		//Store 3 Point Indices
		for (unsigned int j = 0; j < subMesh->m_facesCount; j++) 
		{
			if (Scene->mMeshes[i]->mFaces[j].mNumIndices == 3) 
			{
				subMesh->Indices[indexCount] = Scene->mMeshes[i]->mFaces[j].mIndices[0];
				indexCount++;
				subMesh->Indices[indexCount] = Scene->mMeshes[i]->mFaces[j].mIndices[1];
				indexCount++;
				subMesh->Indices[indexCount] = Scene->mMeshes[i]->mFaces[j].mIndices[2];
				indexCount++;
			}
		}

		//Store Materials as Shader Resources
		for (int i = 0; i < Scene->mNumMaterials; i++) 
		{
			//Texture Path Variables
			aiString textureName;
			std::string textureFilePath = mFilePath;

			//Get the Name of the Texture
			Scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &textureName);
			
			//Append the Name of the Texture to the defined Filepath
			textureFilePath = textureFilePath + textureName.data;

			//Create Shader Resource from File
			if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, textureFilePath.c_str(), NULL, NULL, &subMesh->Texture[Diffuse], NULL))) {
				//Output Error Message 
				OutputDebugString("Unable to Load Texture: ");
				OutputDebugString(textureName.C_Str());
				OutputDebugString("\n");
				return false;
			}
		}

		//Add Submesh to the List of Submeshes 
		SubMeshList.push_back(subMesh);
	}

	return true;
}

//Shutdown Mesh and Release Models
void CMesh::Shutdown() 
{

	//Checks for Models
	if (m_modelCount) 
	{

		//Release Models if Existing
		for (int i = 0; i < m_modelCount; i++) 
		{

		}

	}
}

//Processes the Filename, Model Type and File Type, Generating a Path to the Mesh
std::string CMesh::MeshFinder(eModelType modelType) 
{

	std::string directory = "Resources/Models/";

	//Process Filename and Type
	switch (modelType)
	{
	case Object:
		directory = directory + "Objects/";
		break;
	case Water:
		directory = directory + "Water/";
		break;
	case Building:
		directory = directory + "Buildings/";
		break;
	case Terrain:
		directory = directory + "Terrains/";
		break;
	case Vehicle:
		directory = directory + "Vehicles/";
		break;
	}

	return directory;

}


//Render Buffers Function
bool CMesh::PrepareBuffers(ID3D11DeviceContext* deviceContext, int indexCount) 
{
	unsigned int stride;
	unsigned int offset;

	//Get List of SubMeshs
	std::vector<CMesh::SubMesh*> subMeshList = this->GetSubMeshList();

	//Iterate through SubMeshes
	CMesh::SubMesh* currentMesh = subMeshList.at(indexCount);

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

	return true;

}

ID3D11Buffer* CMesh::GetVertexBuffer(SubMesh* subMesh) 
{
	return subMesh->VertexBuffer;
}

int CMesh::GetVertexCount() 
{
	return totalVertexCount;
}

ID3D11Buffer* CMesh::GetIndexBuffer(SubMesh* subMesh) 
{
	return subMesh->IndexBuffer;
}

int CMesh::GetIndexCount() 
{
	return totalIndexCount;
}

std::vector<CMesh::SubMesh*> CMesh::GetSubMeshList() 
{
	return SubMeshList;
}

int CMesh::GetSubMeshNum() 
{
	return numSubMeshes;
}

ID3D11ShaderResourceView* CMesh::GetTexture(SubMesh* subMesh, int textureType) {
	return subMesh->Texture[textureType];
}