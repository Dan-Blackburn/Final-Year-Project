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

/////////////////////////////////////////////////////////////
//Getters
/////////////////////////////////////////////////////////////

//Returns Vertex Buffer of SubMesh
ID3D11Buffer* CMesh::GetVertexBuffer(SubMesh* subMesh)
{
	return subMesh->VertexBuffer;
}

//Returns Vertex Count of all SubMeshes
int CMesh::GetVertexCount()
{
	return totalVertexCount;
}

//Returns Index Buffer of SubMesh
ID3D11Buffer* CMesh::GetIndexBuffer(SubMesh* subMesh)
{
	return subMesh->IndexBuffer;
}

//Returns Index Count of all SubMeshes
int CMesh::GetIndexCount()
{
	return totalIndexCount;
}

//Returns List of all SubMeshes in Model
std::vector<CMesh::SubMesh*> CMesh::GetSubMeshList()
{
	return SubMeshList;
}

//Returns the Number of SubMeshes in Model
int CMesh::GetSubMeshNum()
{
	return numSubMeshes;
}

//Returns the Texture of SubMesh
ID3D11ShaderResourceView* CMesh::GetTextures(SubMesh* subMesh) {
	return subMesh->Texture[4];
}

/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
//Setters
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////

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

	const aiScene* Scene = importer.ReadFile(mFilePath + fileName, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_ConvertToLeftHanded);

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
	int subMeshTextures = 0;


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

			//Store Position Coordinates (XYZ)
			Vertices->position.x = Scene->mMeshes[i]->mVertices[j].x;
			Vertices->position.y = Scene->mMeshes[i]->mVertices[j].y;
			Vertices->position.z = Scene->mMeshes[i]->mVertices[j].z;

			//Store Texture Coordinates (XY) - Diffuse Only
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

		//Check if Mesh has Texture Coordinates
		if (Scene->mMeshes[i]->HasTextureCoords(0)) 
		{
			//Texture Path Variables
			aiString diffuseFileName;
			aiString specularFileName;
			aiString normalFileName;
			aiString alphaFileName;

			//Flag
			aiString flaggedName;

			//Get Texture Names
			Scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseFileName);
			Scene->mMaterials[i]->GetTexture(aiTextureType_SPECULAR, 0, &specularFileName);
			Scene->mMaterials[i]->GetTexture(aiTextureType_NORMALS, 0, &normalFileName);
			Scene->mMaterials[i]->GetTexture(aiTextureType_OPACITY, 0, &alphaFileName);

			/////////////////////////////////
			//Diffuse Texture Loading
			/////////////////////////////////

			if (diffuseFileName != flaggedName)
			{
				std::string textureFilePath = mFilePath;
				aiString testName;

				//Append the Name of the Texture to the defined Filepath
				textureFilePath = textureFilePath + diffuseFileName.data;

				//Create Shader Resource from File
				if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, textureFilePath.c_str(), NULL, NULL, &subMesh->Texture[Diffuse], NULL))) {
					//Output Error Message 
					OutputDebugString("Unable to Load Texture: ");
					OutputDebugString(diffuseFileName.C_Str());
					OutputDebugString("\n");
					return false;
				}

				subMesh->hasDiffuse = true;
			}

			/////////////////////////////////
			//Specular Texture Loading
			/////////////////////////////////

			if (specularFileName != flaggedName)
			{
				std::string textureFilePath = mFilePath;
				aiString testName;

				//Append the Name of the Texture to the defined Filepath
				textureFilePath = textureFilePath + specularFileName.data;

				//Create Shader Resource from File
				if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, textureFilePath.c_str(), NULL, NULL, &subMesh->Texture[Specular], NULL))) {
					//Output Error Message 
					OutputDebugString("Unable to Load Texture: ");
					OutputDebugString(specularFileName.C_Str());
					OutputDebugString("\n");
					return false;
				}

				subMesh->hasSpecular = true;
			}

			/////////////////////////////////
			//Normal Texture Loading
			/////////////////////////////////

			if (normalFileName != flaggedName)
			{
				std::string textureFilePath = mFilePath;
				aiString testName;

				//Append the Name of the Texture to the defined Filepath
				textureFilePath = textureFilePath + normalFileName.data;

				//Create Shader Resource from File
				if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, textureFilePath.c_str(), NULL, NULL, &subMesh->Texture[Normal], NULL))) {
					//Output Error Message 
					OutputDebugString("Unable to Load Texture: ");
					OutputDebugString(normalFileName.C_Str());
					OutputDebugString("\n");
					return false;
				}

				subMesh->hasNormal = true;
			}

			/////////////////////////////////
			//Alpha Texture Loading
			/////////////////////////////////

			if (alphaFileName != flaggedName)
			{
				std::string textureFilePath = mFilePath;
				aiString testName;

				//Append the Name of the Texture to the defined Filepath
				textureFilePath = textureFilePath + alphaFileName.data;

				//Create Shader Resource from File
				if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, textureFilePath.c_str(), NULL, NULL, &subMesh->Texture[Alpha], NULL))) {
					//Output Error Message 
					OutputDebugString("Unable to Load Texture: ");
					OutputDebugString(alphaFileName.C_Str());
					OutputDebugString("\n");
					return false;
				}

				subMesh->hasAlpha = true;
			}

			if (!subMesh->hasSpecular) { subMesh->Texture[Specular] = subMesh->Texture[Diffuse]; }
			if (!subMesh->hasNormal) { subMesh->Texture[Normal] = subMesh->Texture[Diffuse]; }
			if (!subMesh->hasAlpha) { subMesh->Texture[Alpha] = subMesh->Texture[Diffuse]; }
			
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