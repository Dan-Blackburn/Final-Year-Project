#pragma once

#ifndef _CMESHL_H_
#define _CMESHL_H_

//Include
#include <d3d11.h>
#include <d3dx10math.h>

//Mesh Class
class CMesh {
public:
	//Constructors & Destructors
	CMesh();
	CMesh(const CMesh&);
	~CMesh();

	//Functions
	bool LoadMesh();
	bool RenderModels(ID3D11DeviceContext*);
	void Shutdown();

private:
	//Vertex Structure
	struct VertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

	//Variables
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	int m_modelCount;
};

#endif