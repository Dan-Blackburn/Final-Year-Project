//Includes
#include "CMesh.h"
#include <Importer.hpp>
#include <Scene.h>
#include <PostProcess.h>

//Constructor
CMesh::CMesh() {

}

//Copy Constructor
CMesh::CMesh(const CMesh& other) {
	m_indexBuffer = 0;
	m_vertexBuffer = 0;
	m_indexCount = 0;
	m_vertexBuffer = 0;
}

//Destructor
CMesh::~CMesh() {
}

//Load Mesh from File
bool CMesh::LoadMesh() {

	//Mesh Location Information


	//Create Instance of Importer
	Assimp::Importer importer;

	//const aiScene* scene = importer.ReadFile();

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