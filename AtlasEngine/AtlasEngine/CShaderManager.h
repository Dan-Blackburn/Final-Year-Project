#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include "CShader.h"
#include <vector>

class CShaderManager
{
public:
	CShaderManager();
	CShaderManager(const CShaderManager&);
	~CShaderManager();

	//Getters & Setters
	int GetShaderCount() { return ShaderCount; }
	CShader* GetShader(string);

	bool SetShaderType(string);
	inline void SetShaderCount(int newShaderNum) { ShaderCount = newShaderNum; }

	//Functions
	int InitialiseShaders(ID3D11Device*, HWND);
	int RenderShaders();
	bool Shutdown();

private:
	//Variables
	int ShaderCount;
	CShader* m_Shader;
	vector<CShader*> m_ShaderList;

};