#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include "CLight.h"
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

	inline void SetCurrentShader(CShader* shader) { m_Shader = shader; }
	bool SetShaderType(string);
	inline void SetShaderCount(int newShaderNum) { ShaderCount = newShaderNum; }

	//Functions
	int InitialiseShaders(ID3D11Device*, HWND);
	bool RenderShader(ID3D11DeviceContext*, int, D3DMATRIX, D3DMATRIX, D3DMATRIX, ID3D11ShaderResourceView*[4], vector<CLight*>, CCamera* mainCamera);
	bool Shutdown();

private:
	//Variables
	int ShaderCount;
	CShader* m_Shader;
	vector<CShader*> m_ShaderList;

};