#pragma once

#ifndef _CMESHL_H_
#define _CMESHL_H_

//Include
#include <d3d11.h>
#include <D3DX11tex.h>
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
	static enum eTextureType {Diffuse, Specular, Normal, Alpha};

	//Functions
	bool LoadMesh(ID3D11Device*, std::string, eModelType, std::string, std::string);
	void Shutdown();

	//Getters
	int GetVertexCount();
	int GetIndexCount();
	int GetSubMeshNum();

	ID3D11Buffer* GetVertexBuffer(SubMesh*);
	ID3D11Buffer* GetIndexBuffer(SubMesh*);
	ID3D11ShaderResourceView** GetTextures(SubMesh*);

	std::vector<CMesh::SubMesh*> GetSubMeshList();
	bool PrepareBuffers(ID3D11DeviceContext*, int);

	//Vertex Structure
	struct VertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	//Sub-Mesh Structure
	struct SubMesh {
		std::vector<VertexType*> VerticesList;
		ID3D11Buffer* VertexBuffer;
		ID3D11Buffer* IndexBuffer;
		ID3D11ShaderResourceView* Texture[4];

		int m_vertexCount;
		int m_indexCount;
		int m_facesCount;
		unsigned int* Indices;

		bool hasDiffuse = false;
		bool hasSpecular = false;
		bool hasNormal = false;
		bool hasAlpha = false;
	};

private:
	//Functions
	std::string MeshFinder(eModelType, std::string);

	//Variables
	int m_modelCount;
	int numSubMeshes;
	int totalVertexCount;
	int totalIndexCount;

	std::vector<SubMesh*> SubMeshList;
};

#endif