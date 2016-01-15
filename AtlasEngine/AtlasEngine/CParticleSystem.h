#pragma once
#define _CPARTICLESYSTEM_H_

//Includes
#include <d3d11.h>
#include <d3dx10math.h>
#include <D3DX11tex.h>
#include <iostream>
#include <vector>

#include "CCamera.h"

//Particle System Class
class CParticleSystem
{
public:
	//Constructors & Destructor
	CParticleSystem();
	CParticleSystem(const CParticleSystem&);
	~CParticleSystem();

	//Getters & Setters
	inline std::string GetName() { return m_Name; }
	inline std::string GetShaderName() { return m_ShaderName; }
	inline ID3D11ShaderResourceView** GetTexture() { return m_Texture; }
	inline int GetIndexCount() { return m_IndexCount; }
	inline void SetName(std::string name) { m_Name = name; }
	inline void SetShaderName(std::string shaderName) { m_ShaderName = shaderName; }
	void SetEmitterPosition(D3DXVECTOR3 position);
	inline void SetParticlesPerSecond(int particlesPerSecond) { m_ParticlesPerSecond = particlesPerSecond; }
	inline void SetParticleDeviation(D3DXVECTOR3 particleDeviation) { m_ParticleDeviation = particleDeviation; }
	inline void SetMaxParticles(int maxParticles) { m_MaxParticles = maxParticles; }
	inline void SetVelocity(float velocity) { m_ParticleVelocity = velocity; }
	inline void SetVelocityVariation(float velocityVariation) { m_ParticleVelocityVariation = velocityVariation; }
	inline void SetParticleSize(D3DXVECTOR3 particleSize) { m_ParticleSize = particleSize; }

	//Functions
	bool Initialise(ID3D11Device*, std::string, std::string, std::string);
	bool Frame(ID3D11DeviceContext*, float frameTime, CCamera*);
	inline void Render(ID3D11DeviceContext* deviceContext) { PrepareBuffers(deviceContext); }
	void Shutdown();

private:
	//Functions
	bool InitialiseParticleSystem();
	void ShutdownParticleSystem();

	bool InitialiseBuffers(ID3D11Device*);
	void ShutdownBuffers();

	void EmitParticles(float);
	void UpdateParticles(float);
	void KillParticles();

	bool UpdateBuffers(ID3D11DeviceContext*, CCamera*);
	void PrepareBuffers(ID3D11DeviceContext*);

	std::string TextureFinder(std::string, std::string, std::string);

	//Structures
	struct ParticleType
	{
		D3DXVECTOR3 position;
		float velocity;
		bool active;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR4 colour;
	};

	//Variables
	std::string m_Name;
	std::string m_ShaderName;
	D3DXVECTOR3 m_ParticleDeviation;
	D3DXVECTOR3 m_EmitterPosition;
	D3DXVECTOR3 m_ParticleSize;
	float m_ParticleVelocity, m_ParticleVelocityVariation;
	float m_ParticlesPerSecond;
	int m_MaxParticles;

	int m_CurrentParticleCount;
	float m_AccumulatedTime;

	ParticleType* m_ParticleList;

	VertexType* m_Vertices;

	int m_VertexCount, m_IndexCount;
	ID3D11Buffer *m_VertexBuffer, *m_IndexBuffer;
	ID3D11ShaderResourceView* m_Texture[4];
};

