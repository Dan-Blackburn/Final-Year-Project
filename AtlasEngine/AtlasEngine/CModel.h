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

private:
	//Functions
	bool InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	//Class Pointers
	CMesh* Mesh;
};

#endif