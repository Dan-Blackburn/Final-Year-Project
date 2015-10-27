#pragma once

#ifndef _CMESHL_H_
#define _CMESHL_H_

//Include
#include <d3d11.h>
#include <d3dx10math.h>
#include <string>
#include <vector>

//Prototypes
struct VertexType;

//Mesh Class
class CMesh{
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
	VertexType* GetVertexData();
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();

	//Vertex Structure
	struct VertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

private:
	//Functions
	std::string MeshFinder(std::string, eModelType, std::string);

	//Variables
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount, m_modelCount;
	int numSubMeshes;

	std::vector<CMesh*> SubMeshList;
	std::vector<VertexType*> VerticesList;
	std::vector<unsigned int*> IndicesList;


	CMesh* SubMesh;
	VertexType* Vertices;
	unsigned int* Indices;
};

#endif