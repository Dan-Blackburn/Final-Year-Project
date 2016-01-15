#pragma once

#include "CDirect3D.h"
#include "CModel.h"
#include "CCamera.h"
#include "CShaderManager.h"
#include "CParticleSystem.h"
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
	bool InitialiseEntities(ID3D11Device*);
	bool Frame(ID3D11DeviceContext*, float, float, D3DXMATRIX);
	bool RenderEntities(CDirect3D*, CShaderManager*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void Shutdown();

private:
	//Entity Variables
	int m_ModelEntityCount;
	int m_ParticleSystemCount;
	int m_LightEntityCount;

	//Entity Pointers
	CModel* m_ModelEntity;
	CParticleSystem* m_ParticleSystem;
	CLight* m_LightEntity;
	CCamera* m_CameraEntity;

	std::vector<CModel*> m_ModelList;
	std::vector<CParticleSystem*> m_ParticleSystemList;
	std::vector<CLight*> m_LightList;
	std::vector<CCamera*> m_CameraList;

};