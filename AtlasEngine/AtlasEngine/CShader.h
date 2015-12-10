#pragma once

#ifndef _CSHADER_H_
#define _CSHADER_H_
//Includes
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include <iostream>

//Namespaces
using namespace std;

//Shader Class
class CShader {

public:
	//Constructors & Destructors
	CShader();
	CShader(const CShader&);
	~CShader();

	//Functions
	bool Initialise(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*[4]);

	inline std::string GetShaderName() { return m_ShaderName; }
	inline std::string GetVSFilename() { return m_VSFilename; }
	inline std::string GetPSFilename() { return m_PSFilename; }

	inline void SetShaderName(std::string shaderName) { m_ShaderName = shaderName; }
	inline void SetVSFilename(std::string vsFilename) { m_VSFilename = vsFilename; }
	inline void SetPSFilename(std::string psFilename) { m_PSFilename = psFilename; }

private:
	//Structures
	struct MatrixBufferType {
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	//Functions
	bool InitialiseShader(ID3D11Device*, HWND, LPCSTR, LPCSTR);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, LPCSTR);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*[4]);
	void RenderShader(ID3D11DeviceContext*, int);

	//Variables
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	std::string m_ShaderName;
	std::string m_VSFilename;
	std::string m_PSFilename;

	ID3D11SamplerState* m_sampleState;
};

#endif
