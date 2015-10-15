#pragma once

#ifndef _CMODEL_H_
#define _CMODEL_H_

//Include
#include <d3d11.h>
#include <d3dx10math.h>
#include "CMesh.h"

//Model Class
class CModel{
public:
	//Constructors & Destructors
	CModel();
	CModel(const CModel&);
	~CModel();

	//Functions
	bool Initialise(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();

private:
	//Vertex Structure
	struct VertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

	//Functions
	bool InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	//Variables
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	
	//Class Pointers
	CMesh* Mesh;
};

#endif