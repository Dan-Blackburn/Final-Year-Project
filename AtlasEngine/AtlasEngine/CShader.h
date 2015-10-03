#pragma once

#ifndef _CSHADER_H_
#define _CSHADER_H_
//Includes
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

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


private:
	//Structures
	struct MatrixBufferType {
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

};

#endif
