//Includes
#include "CShader.h"

//Constructor
CLightShader::CLightShader() {
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_cameraBuffer = 0;
	m_lightBuffer = 0;
	m_sampleState = 0;
}

//Destructor
CLightShader::~CLightShader() {
}

//Initialise Shader Function
bool CLightShader::Initialise(ID3D11Device* device, HWND hwnd) {

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
void CLightShader::Shutdown() {
	
	//Shutdown Shaders
	ShutdownShader();

	return;
}

//Render Function
bool CLightShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, 
														ID3D11ShaderResourceView* texture[4],  vector<CLight*> lightList, CCamera* mainCamera) 
{
	bool result;

	//Set Shader parameters
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightList, mainCamera);
	if (!result) {
		return false;
	}

	//Render Prepped Buffers with Shader
	RenderShader(deviceContext, indexCount);

	return true;
}

//Initialise Shader Function
bool CLightShader::InitialiseShader(ID3D11Device* device, HWND hwnd, LPCSTR vsFilename, LPCSTR psFilename) {
	
	//Implement multiple "Shader" classes.
	//Define Variables
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC lightPositionBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	//Initialise Variables
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//Complie Vertex Shader Code
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, this->m_VSName.c_str(), "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
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
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, this->m_PSName.c_str(), "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
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

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "BITANGENT";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;

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

	//Setup Description of Camera Buffer (Vertex Shader)
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	//Create Constant Camera Buffer
	result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Setup Description of Light Position Buffer (Pixel Shader)
	lightPositionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightPositionBufferDesc.ByteWidth = sizeof(LightPositionBuffer);
	lightPositionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightPositionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightPositionBufferDesc.MiscFlags = 0;
	lightPositionBufferDesc.StructureByteStride = 0;

	//Create Constant Light Position Buffer
	result = device->CreateBuffer(&lightPositionBufferDesc, NULL, &m_lightPositionBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Setup the description of the light dynamic constant buffer that is in the pixel shader.
	//Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}


	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
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
void CLightShader::ShutdownShader() {

	//Release Sampler State
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	//Release Matrix Constant Buffer
	if (m_matrixBuffer) 
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	//Release Light Position Constant Buffer
	if (m_lightPositionBuffer)
	{
		m_lightPositionBuffer->Release();
		m_lightPositionBuffer = 0;
	}

	//Release Camera Constant Buffer
	if (m_cameraBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	//Release Light Constant Buffer
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	//Release Layout
	if (m_layout) 
	{
		m_layout->Release();
		m_layout = 0;
	}

	//Release Pixel Shader
	if (m_pixelShader) 
	{
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

void CLightShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCSTR shaderFilename) {
	
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
bool CLightShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, 
											ID3D11ShaderResourceView* textures[4], vector<CLight*> lightList, CCamera* mainCamera) {

	//Define Variables
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	CameraBufferType* dataPtr1;
	LightBufferType* dataPtr2;
	LightPositionBuffer* dataPtr3;
	unsigned int bufferNumber;

	//Transpose Matrices for Shader
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	//Vertex Shader Code Below

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

	//Lock Camera Constant Buffer
	result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//Get Pointer to data in Constant Buffer
	dataPtr1 = (CameraBufferType*)mappedResource.pData;

	dataPtr1->cameraPosition = mainCamera->GetPosition();
	dataPtr1->padding = 0.0f;

	//Unlock Constant Buffer
	deviceContext->Unmap(m_cameraBuffer, 0);

	//Increase Buffer number
	bufferNumber++;

	//Set Camera Constant Buffer in Vertex Shader
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);
	//////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////
	//Pixel Shader Code Below:

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 4, textures);

	//Lock Light Constant Buffer
	result = deviceContext->Map(m_lightPositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//Get Pointer to data in Constant Buffer
	dataPtr3 = (LightPositionBuffer*)mappedResource.pData;

	int index = 0;
	for (std::vector<CLight*>::iterator it = lightList.begin(); it != lightList.end(); it++)
	{
		if ((*it)->GetType() == CLight::Point)
		{
			dataPtr3->lightPosition[index] = (*it)->GetPosition();
			dataPtr3->lightColour[index] = (*it)->GetConstantColour();
			dataPtr3->specularPower[index] = (*it)->GetSpecularPower();
			dataPtr3->Brightness[index] = (*it)->GetBrightness();
			index++;
		}
	}

	//Unlock Constant Buffer
	deviceContext->Unmap(m_lightPositionBuffer, 0);


	//Lock Light Contant Buffer
	result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//Get Pointer to data in Constant Buffer
	dataPtr2 = (LightBufferType*)mappedResource.pData;

	CLight* sunLight = lightList[0];
	CLight* moonLight = lightList[1];

	//Copy Lighting Variables into Constant Buffer
	dataPtr2->ambientSunColour = sunLight->GetColour();
	dataPtr2->ambientMoonColour = moonLight->GetColour();
	dataPtr2->diffuseColour = D3DXVECTOR4{1.0f, 1.0f, 1.0f, 1.0f};
	dataPtr2->specularColour = D3DXVECTOR4{ 1.0f, 1.0f, 0.8f, 1.0f };
	dataPtr2->sunlightDirection = sunLight->GetDirection();
	dataPtr2->sunlightAngle = sunLight->GetAngle();
	dataPtr2->moonlightDirection = moonLight->GetDirection();
	dataPtr2->padding = 0.0f;

	//Unlock Constant Buffer
	deviceContext->Unmap(m_lightBuffer, 0);

	//Set position of Constant Buffers in Pixel Shader
	bufferNumber = 0;

	//Set the Light Constant Buffer in the Pixel Shader with the updated Values
	deviceContext->PSSetConstantBuffers(bufferNumber, 2, &m_matrixBuffer);
	bufferNumber++;
	deviceContext->PSSetConstantBuffers(bufferNumber, 2, &m_lightPositionBuffer);
	bufferNumber++;
	deviceContext->PSSetConstantBuffers(bufferNumber, 2, &m_lightBuffer);

	//////////////////////////////////////////////////////////////////////////////////////////////

	return true;
}

//Render Shader Function
void CLightShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount) {

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