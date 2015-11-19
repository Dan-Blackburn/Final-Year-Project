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

	struct ModelProperties;

	//Constructors & Destructors
	CModel();
	CModel(const CModel&);
	~CModel();

	//Functions
	bool Initialise(ID3D11Device*, int);
	void Shutdown();
	
	//Setter
	void SetModelName(std::string);
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void SetSkin(ID3D11ShaderResourceView*);

	//Getters
	ModelProperties* GetModel();
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

private:
	struct ModelProperties {
		std::string ModelName;
		CMesh* Mesh;
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Rotation;
		ID3D11ShaderResourceView* Texture;
	};

	//Functions
	bool InitialiseBuffers(ID3D11Device*, int);
	void ShutdownBuffers();

	//Class Pointers
	ModelProperties* m_Model;

};

#endif