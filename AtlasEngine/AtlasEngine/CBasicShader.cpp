//Includes
#include "CShader.h"

//Constructor
CBasicShader::CBasicShader() {
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_sampleState = 0;
}

//Destructor
CBasicShader::~CBasicShader() {
}

//Initialise Shader Function
bool CBasicShader::Initialise(ID3D11Device* device, HWND hwnd) {

	bool result;
	string VSFilepath = "../AtlasEngine/" + m_VSFilename;
	string PSFilepath = "../AtlasEngine/" + m_PSFilename;

	//Initialise Vertex and Pixel Shaders
	result = InitialiseShader(device, hwnd, VSFilepath.c_str(), PSFilepath.c_str());
	if (!result) {
		return false;
	}

	return true;
}

//Shutdown Shader Function
void CBasicShader::Shutdown() {

	//Shutdown Shaders
	ShutdownShader();

	return;
}

//Render Function
bool CBasicShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
															 ID3D11ShaderResourceView* texture[4], D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColour) {

	bool result;

	//Set Shader parameters
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, diffuseColour);
	if (!result) {
		return false;
	}

	//Render Prepped Buffers with Shader
	RenderShader(deviceContext, indexCount);

	return true;
}

//Initialise Shader Function
bool CBasicShader::InitialiseShader(ID3D11Device* device, HWND hwnd, LPCSTR vsFilename, LPCSTR psFilename) {

	//Define Variables
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	//Initialise Variables
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//Complie Vertex Shader Code
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		//Submit Error on Compile Failure
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		//Submit Error Message on Unable to Find File
		else
		{
			MessageBox(hwnd, vsFilename, "Missing Shader File", MB_OK);
		}

		return false;
	}

	//Compile Pixel Shader Code
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		//Submit Error on Compile Failure
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		//Submit Error Message on Unable to Find File
		else
		{
			MessageBox(hwnd, psFilename, "Missing Shader File", MB_OK);
		}

		return false;
	}

	//Create Vertex Shader from Buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	//Create Pixel Shader from Buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	//Setup Layout Data going to Shader (Matching VertexType Structure in Model Class)
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	//Get Count of Elements
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//Create Vertex Layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);

	if (FAILED(result))
	{
		return false;
	}

	//Release Shader Buffers (No Longer Needed)
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	//Setup Description of Matrix Buffer (Vertex Shader)
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//Create Constant Matrix Buffer
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}


	return true;
}

//Shutdown Shader Function
void CBasicShader::ShutdownShader() {

	//Release Sampler State
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	//Release Matrix Constant Buffer
	if (m_matrixBuffer) {
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	//Release Layout
	if (m_layout) {
		m_layout->Release();
		m_layout = 0;
	}

	//Release Pixel Shader
	if (m_pixelShader) {
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	//Release Vertex Shader
	if (m_vertexShader) {
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}

void CBasicShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCSTR shaderFilename) {

	//Define Variables
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	//Get Pointer to Error Message (Text Buffer)
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	//Get length of Message
	bufferSize = errorMessage->GetBufferSize();

	//Open File to Write Error Message
	fout.open("Shader-Error.txt");

	//Write Error Message
	for (i = 0; i < bufferSize; i++) {
		fout << compileErrors[i];
	}

	//Close File
	fout.close();

	//Release Error Message
	errorMessage->Release();
	errorMessage = 0;

	//Display file check Message
	MessageBox(hwnd, "Error Compiling Shader. Check Shader-Text.txt for Message.", shaderFilename, MB_OK);

	return;
}

//Set Shader Parameters Function
bool CBasicShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,
														 ID3D11ShaderResourceView* textures[4], D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColour) {

	//Define Variables
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	//Transpose Matrices for Shader
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	//Lock Constant Buffer
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//Get Pointer to Constant Buffer Data
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	//Copy Matrices into Constant Buffer
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	//Unlock Constant Buffer
	deviceContext->Unmap(m_matrixBuffer, 0);

	//Set Position of Constant Buffer in Vertex Shader
	bufferNumber = 0;

	//Set Constant Buffer with Updated Values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 4, textures);

	return true;
}

//Render Shader Function
void CBasicShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount) {

	//Set Vertex Input Layout
	deviceContext->IASetInputLayout(m_layout);

	//Set Shaders to Render Model
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	//Set Sampler State in Pixel Shader
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	//Render Mesh
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}