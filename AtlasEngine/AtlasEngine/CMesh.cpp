//Includes
#include "CMesh.h"
#include <Importer.hpp>
#include <Scene.h>
#include <PostProcess.h>

//Constructor
CMesh::CMesh() {
	totalVertexCount = 0;
	totalIndexCount = 0;
}

//Copy Constructor
CMesh::CMesh(const CMesh& other) {
}

//Destructor
CMesh::~CMesh() {
}

//Load Mesh from File
bool CMesh::LoadMesh(std::string mFilename, eModelType modelType, std::string mFileType) {

	/////////////////////////////////
	//Load Mesh through Assimp
	/////////////////////////////////

	//Mesh Location Handler
	std::string mFilePath = MeshFinder(mFilename, modelType, mFileType);

	//Create Instance of Importer
	Assimp::Importer importer;

	const aiScene* Scene = importer.ReadFile(mFilePath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	//Error Handler if Failed
	if (!Scene) {
		//Error Message?
		return false;
	}

	/////////////////////////////////
	//Process Scene to Mesh
	/////////////////////////////////

	numSubMeshes = Scene->mNumMeshes;
	int masterIndex = 0;

	//Store Sub-Meshes of Mesh
	for (int i = 0; i < numSubMeshes; i++) {
		SubMesh* subMesh = new SubMesh;

		//Store Vertex & Index Count for Loops
		subMesh->m_vertexCount = Scene->mMeshes[i]->mNumVertices;
		subMesh->m_indexCount = Scene->mMeshes[i]->mNumFaces * 3;
		totalVertexCount += subMesh->m_vertexCount;
		totalIndexCount += subMesh->m_indexCount;

		//Store Vertices of Sub-Mesh
		for (int j = 0; j < subMesh->m_vertexCount; j++) {
			VertexType* Vertices = new VertexType;

			//Store Positions (XYZ)
			Vertices->position.x = Scene->mMeshes[i]->mVertices[j].x;
			Vertices->position.y = Scene->mMeshes[i]->mVertices[j].y;
			Vertices->position.z = Scene->mMeshes[i]->mVertices[j].z;

			//Store Colour (RGBA)
			if (Scene->mMeshes[i]->HasVertexColors(j)) {
				Vertices->color.x = Scene->mMeshes[i]->mColors[j]->r;
				Vertices->color.y = Scene->mMeshes[i]->mColors[j]->g;
				Vertices->color.z = Scene->mMeshes[i]->mColors[j]->b;
				Vertices->color.w = Scene->mMeshes[i]->mColors[j]->a;
			}

			subMesh->VerticesList.push_back(Vertices);
		}

		subMesh->Indices = new unsigned int[subMesh->m_indexCount];

		for (int j = 0; j < subMesh->m_indexCount; j++) {
			subMesh->Indices[j] = j;
		}

		SubMeshList.push_back(subMesh);
	}

	return true;
}

//Render Models
bool CMesh::RenderModels(ID3D11DeviceContext* deviceContext) {

	//Call Model List Render Function?

	return true;
}

//Shutdown Mesh and Release Models
void CMesh::Shutdown() {

	//Checks for Models
	if (m_modelCount) {

		//Release Models if Existing
		for (int i = 0; i < m_modelCount; i++) {

		}

	}
}

//Processes the Filename, Model Type and File Type, Generating a Path to the Mesh
std::string CMesh::MeshFinder(std::string mFilename, eModelType modelType, std::string mFileType) {

	std::string directory = "Resources/Models/";

	//Process Filename and Type
	switch (modelType) {
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
		directory = directory + "Terrain/";
		break;
	case Vehicle:
		directory = directory + "Vehicles/";
		break;
	}

	return directory + mFilename + mFileType;

}

ID3D11Buffer* CMesh::GetVertexBuffer(SubMesh* subMesh) {
	return subMesh->VertexBuffer;
}

int CMesh::GetVertexCount() {
	return totalVertexCount;
}

ID3D11Buffer* CMesh::GetIndexBuffer(SubMesh* subMesh) {
	return subMesh->IndexBuffer;
}

int CMesh::GetIndexCount() {
	return totalIndexCount;
}

std::vector<CMesh::SubMesh*> CMesh::GetSubMeshList() {
	return SubMeshList;
}

int CMesh::GetSubMeshNum() {
	return numSubMeshes;
}