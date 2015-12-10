#pragma once

#include "CModel.h"
#include "CCamera.h"
#include "CShaderManager.h"
#include "CLight.h"
#include <vector>

class CEntityManager {
public:
	//Constructors & Destructors
	CEntityManager();
	CEntityManager(const CEntityManager&);
	~CEntityManager();

	//Getters & Setters
	CCamera* GetCameraEntity();
	int GetModelEntityCount();
	int GetLightEntityCount();

	//Functions
	int InitialiseEntities(ID3D11Device*);
	bool Frame();
	bool RenderEntities(ID3D11DeviceContext*, CShaderManager*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void Shutdown();

private:
	//Entity Variables
	int m_ModelEntityCount;
	int m_LightEntityCount;

	//Entity Pointers
	CModel* m_ModelEntity;
	CLight* m_LightEntity;
	CCamera* m_CameraEntity;

	std::vector<CModel*> m_ModelList;
	std::vector<CLight*> m_LightList;
	std::vector<CCamera*> m_CameraList;

};