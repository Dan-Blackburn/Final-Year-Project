#pragma once

#ifndef _CSHADER_H_
#define _CSHADER_H_
//Includes
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include <iostream>
#include <vector>
#include "CLight.h"
#include "CCamera.h"

//Namespaces
using namespace std;

//Shader Class
class CShader {

public:
	//Functions
	virtual bool Initialise(ID3D11Device*, HWND) = 0;
	virtual bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*[4], vector<CLight*>, CCamera*) = 0;
	virtual void Shutdown() = 0;

	virtual std::string GetShaderName() = 0;
	virtual std::string GetVSName() = 0;
	virtual std::string GetPSName() = 0;
	virtual std::string GetVSFilename() = 0;
	virtual std::string GetPSFilename() = 0;

	virtual void SetShaderName(std::string shaderName) = 0;
	virtual void SetVSName(std::string newVSName) = 0;
	virtual void SetPSName(std::string newPSName) = 0;
	virtual void SetVSFilename(std::string vsFilename) = 0;
	virtual void SetPSFilename(std::string psFilename) = 0;

private:
	//Functions
	virtual bool InitialiseShader(ID3D11Device*, HWND, LPCSTR, LPCSTR) = 0;
	virtual void ShutdownShader() = 0;
	virtual void OutputShaderErrorMessage(ID3D10Blob*, HWND, LPCSTR) = 0;

	virtual bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*[4], vector<CLight*>, CCamera*) = 0;
	virtual void RenderShader(ID3D11DeviceContext*, int) = 0;
};

class CLightShader: public CShader 
{
public:
	//Constructors & Destructors
	CLightShader();
	~CLightShader();

	//Functions
	bool Initialise(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*[4], vector<CLight*> lightList, CCamera* mainCamera);

	inline std::string GetShaderName() { return m_ShaderName; }
	inline std::string GetVSName() { return m_VSName; }
	inline std::string GetPSName() { return m_PSName; }
	inline std::string GetVSFilename() { return m_VSFilename; }
	inline std::string GetPSFilename() { return m_PSFilename; }

	inline void SetShaderName(std::string shaderName) { m_ShaderName = shaderName; }
	inline void SetVSName(std::string newVSName) { m_VSName = newVSName; }
	inline void SetPSName(std::string newPSName) { m_PSName = newPSName; }
	inline void SetVSFilename(std::string vsFilename) { m_VSFilename = vsFilename; }
	inline void SetPSFilename(std::string psFilename) { m_PSFilename = psFilename; }

private:
	//Structures

	//Matrix Buffer Structure
	struct MatrixBufferType 
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	//Camera Buffer Structure
	struct CameraBufferType
	{
		D3DXVECTOR3 cameraPosition;
		float padding;
	};

	//Light Buffer Structure
	struct LightBufferType 
	{
		D3DXVECTOR4 ambientColour = {0.0f, 0.0f, 0.0f, 0.0f};
		D3DXVECTOR4 diffuseColour = { 0.0f, 0.0f, 0.0f, 0.0f};
		D3DXVECTOR4 specularColour = { 0.0f, 0.0f, 0.0f, 0.0f };
		D3DXVECTOR3 lightDirection = { 0.0f, 0.0f, 0.0f };
		float specularPower;
	};

	//Functions
	bool InitialiseShader(ID3D11Device*, HWND, LPCSTR, LPCSTR);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, LPCSTR);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*[4], vector<CLight*> lightList, CCamera* mainCamera);
	void RenderShader(ID3D11DeviceContext*, int);

	//Variables
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_lightBuffer;

	std::string m_ShaderName;
	std::string m_VSName;
	std::string m_PSName;
	std::string m_VSFilename;
	std::string m_PSFilename;

	ID3D11SamplerState* m_sampleState;
};

#endif
