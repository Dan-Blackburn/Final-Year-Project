#pragma once

#ifndef _CMODEL_H_
#define _CMODEL_H_

//Include
#include <d3d11.h>
#include <d3dx10math.h>
#include "CMesh.h"

//Model Class
class CModel{
private:
	struct ModelProperties;
public:
	//Constructors & Destructors
	CModel();
	CModel(const CModel&);
	~CModel();

	//Functions
	bool Initialise(ID3D11Device*, std::string, std::string);
	void Frame(ID3D11DeviceContext* deviceContext, float frameTime);
	void UpdateWorldMatrix();
	void Shutdown();
	
	//Setter
	inline void SetModelName(std::string modelName) { m_Model->ModelName = modelName; }
	inline void SetShaderName(std::string shaderName) { m_Model->ShaderName = shaderName; }
	inline void SetModel(ModelProperties* newModel) { m_Model = newModel; }
	void SetModelType(std::string);
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void SetScale(float, float, float);
	void SetSkin(ID3D11ShaderResourceView*);

	//Getters
	inline std::string GetName() { return m_Model->ModelName; }
	inline ModelProperties* GetModel() { return m_Model; }
	inline D3DXVECTOR3 GetPosition() { return m_Model->Position; };
	inline D3DXVECTOR3 GetRotation() { return m_Model->Rotation; };

	inline std::string GetShaderName() { return m_Model->ShaderName; }

	//Constant Variables
	const float toRadians = 0.0174532925f;

private:
	struct ModelProperties {
		std::string ModelName;
		std::string ShaderName;
		CMesh::eModelType ModelType;
		CMesh* Mesh;
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Rotation;
		D3DXVECTOR3 Scale;
		ID3D11ShaderResourceView* Texture;

		D3DXMATRIX WorldMatrix;
	};

	//Functions
	bool InitialiseBuffers(ID3D11Device*, std::string, std::string);
	void ShutdownBuffers();

	//Class Pointers
	ModelProperties* m_Model;

};

#endif