#pragma once

#include "CModel.h"
#include "CCamera.h"
#include "CShader.h"
#include <vector>

class CEntityManager {
public:
	//Constructors & Destructors
	CEntityManager();
	CEntityManager(const CEntityManager&);
	~CEntityManager();

	//Getters & Setters
	CCamera* GetCameraEntity();
	int GetEntityCount();
	void IncreaseEntityCount(int);
	void DecreaseEntityCount(int);

	//Functions
	int InitialiseEntities(ID3D11Device*);
	bool RenderEntities(ID3D11DeviceContext*, CShader*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void Shutdown();

private:
	//Entity Variables
	int m_EntityCount;

	//Entity Pointers
	CModel* m_ModelEntity;
	CCamera* m_CameraEntity;

	std::vector<CModel*> m_ModelList;
	std::vector<CCamera*> m_CameraList;

};