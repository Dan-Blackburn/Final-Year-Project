#pragma once

#ifndef _CMESHL_H_
#define _CMESHL_H_

//Include
#include <d3d11.h>
#include <d3dx10math.h>
#include <string>
#include <vector>


//Mesh Class
class CMesh{

	//Prototypes
	struct VertexType;
	struct SubMesh;

public:
	//Constructors & Destructors
	CMesh();
	CMesh(const CMesh&);
	~CMesh();

	//Temp Variables?
	static enum eModelType { Building, Terrain, Water, Vehicle, Object };

	//Functions
	bool LoadMesh(std::string, eModelType, std::string);
	bool RenderModels(ID3D11DeviceContext*);
	void Shutdown();

	int GetVertexCount();
	int GetIndexCount();
	int GetSubMeshNum();

	ID3D11Buffer* GetVertexBuffer(SubMesh*);
	ID3D11Buffer* GetIndexBuffer(SubMesh*);

	std::vector<CMesh::SubMesh*> GetSubMeshList();
	bool RenderBuffers(ID3D11DeviceContext*, int);

	//Vertex Structure
	struct VertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

	//Sub-Mesh Structure
	struct SubMesh {
		std::vector<VertexType*> VerticesList;
		ID3D11Buffer* VertexBuffer;
		ID3D11Buffer* IndexBuffer;

		int m_vertexCount;
		int m_indexCount;
		int m_facesCount;
		unsigned int* Indices;
	};

private:
	//Functions
	std::string MeshFinder(std::string, eModelType, std::string);

	//Variables
	int m_modelCount;
	int numSubMeshes;
	int totalVertexCount;
	int totalIndexCount;

	std::vector<SubMesh*> SubMeshList;
};

#endif