//Includes
#include "CParticleSystem.h"

//Constructor
CParticleSystem::CParticleSystem()
{
	m_ParticleList = 0;
	m_Vertices = 0;
	m_VertexBuffer = 0;
	m_IndexBuffer = 0;
}

//Copy Constructor
CParticleSystem::CParticleSystem(const CParticleSystem &other) {}

//Destructor
CParticleSystem::~CParticleSystem()
{

}

//Getters & Setters

//Set's Position of Particles being Emitted
void CParticleSystem::SetEmitterPosition(D3DXVECTOR3 position)
{
	m_EmitterPosition = position;
}

//Initialise Particle System
bool CParticleSystem::Initialise(ID3D11Device* device, std::string mFileName, std::string mFileType, std::string textureLocation)
{
	bool result;

	//Texture Location
	std::string textureRelativePath = TextureFinder(textureLocation, mFileName, mFileType);

	if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, textureRelativePath.c_str(), NULL, NULL, &m_Texture[0], NULL)))
	{
		//Output Error Message 
		OutputDebugString("Unable to Load Texture: ");
		OutputDebugString(textureRelativePath.c_str());
		OutputDebugString("\n");
		return false;
	}

	//Initialise Particles in System
	result = InitialiseParticleSystem();
	if (!result)
	{
		return false;
	}

	//Initialise Buffers
	result = InitialiseBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

//Shutdown Particle System
void CParticleSystem::Shutdown()
{
	//Release Buffers
	ShutdownBuffers();

	//Release Particle System
	ShutdownParticleSystem();
}

//Frame Particle System
bool CParticleSystem::Frame(ID3D11DeviceContext* deviceContext, float frameTime, CCamera* mainCamera)
{
	bool result;

	//Release previous Particles
	KillParticles();

	//Emit new Particles
	EmitParticles(frameTime);

	//Update positions of Particles
	UpdateParticles(frameTime);

	//Update Buffers (Dynamic)
	result = UpdateBuffers(deviceContext, mainCamera);

	if (!result)
	{
		return false;
	}

	return true;
}


//Initialise Particle System
bool CParticleSystem::InitialiseParticleSystem()
{
	//Create Particle List
	m_ParticleList = new ParticleType[m_MaxParticles];
	if (!m_ParticleList)
	{
		return false;
	}

	//Set all Particles in list to Inactive
	for (int i = 0; i < m_MaxParticles; i++)
	{
		m_ParticleList[i].position = m_EmitterPosition;
		m_ParticleList[i].active = false;
	}

	m_CurrentParticleCount = 0;
	m_AccumulatedTime = 0.0f;

	return true;
}

//Shutdown Particle System
void CParticleSystem::ShutdownParticleSystem()
{
	//Release Particle List
	if (m_ParticleList)
	{
		delete[] m_ParticleList;
		m_ParticleList = 0;
	}
}

//Initialise Particle System Buffers
bool CParticleSystem::InitialiseBuffers(ID3D11Device* device)
{
	HRESULT result;

	unsigned long* indices;
	int i;

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	//Set max number of Vertices in Vertex array
	m_VertexCount = m_MaxParticles * 6;

	//Set max number of Indices in Index array
	m_IndexCount = m_VertexCount;

	//Create Vertex array for rendering Particles
	m_Vertices = new VertexType[m_VertexCount];
	if (!m_Vertices)
	{
		return false;
	}

	//Create Index array
	indices = new unsigned long[m_IndexCount];
	if (!indices)
	{
		return false;
	}

	//Initialise Vertex array
	memset(m_Vertices, 0, (sizeof(VertexType)* m_VertexCount));

	//Initialise Index Array
	for (i = 0; i < m_IndexCount; i++)
	{
		indices[i] = i;
	}

	//Set up Vertex Buffer description
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Assign Pointer
	vertexData.pSysMem = m_Vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create Vertex Buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Set up Index Buffer description
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Release Index Array
	delete[] indices;
	indices = 0;

	return true;
}

//Shutdown Particle System Buffers
void CParticleSystem::ShutdownBuffers()
{
	//Release Vertex Buffer
	if (m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = 0;
	}

	//Release Index Buffer
	if (m_IndexBuffer)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = 0;
	}
}

//Emit particles per Frame
void CParticleSystem::EmitParticles(float frameTime)
{
	bool emit = false;
	D3DXVECTOR3 position, colour;
	float velocity;

	//Increment Frame Time
	m_AccumulatedTime += frameTime;

	//Check if next Particle needs emitting
	if (m_AccumulatedTime > (1000.0f / m_ParticlesPerSecond))
	{
		//Reset Total Time
		m_AccumulatedTime = 0.0f;
		emit = true;
	}

	//Emit Particles when true
	if ((emit == true) && (m_CurrentParticleCount < (m_MaxParticles - 1)))
	{
		m_CurrentParticleCount++;

		D3DXVECTOR3 randomLowVector = { (m_EmitterPosition.x - m_ParticleDeviation.x), (m_EmitterPosition.y - m_ParticleDeviation.y), (m_EmitterPosition.z - m_ParticleDeviation.z) };
		D3DXVECTOR3 randomHighVector = { (m_EmitterPosition.x + m_ParticleDeviation.x), (m_EmitterPosition.y + m_ParticleDeviation.y), (m_EmitterPosition.z + m_ParticleDeviation.z) };
		//Generate Particle Properties
		position.x = randomLowVector.x + static_cast <float>(rand()) /(static_cast <float> (RAND_MAX / (randomHighVector.x - randomLowVector.x)));
		position.y = randomLowVector.y + static_cast <float>(rand()) /(static_cast <float> (RAND_MAX / (randomHighVector.x)));// -(m_EmitterPosition.y - m_ParticleDeviation.y));
		position.z = randomLowVector.z + static_cast <float>(rand()) /(static_cast <float> (RAND_MAX / (randomHighVector.x - randomLowVector.z)));

		//Generate Velocity
		velocity = m_ParticleVelocity * m_ParticleVelocityVariation;

		//Z Depth Sorting
		int index = 0;
		bool furthestInactive = false;

		while (!furthestInactive)
		{
			if ((m_ParticleList[index].active == false) || (m_ParticleList[index].position.z < position.z))
			{
				furthestInactive = true;
			}

			else
			{
				index++;
			}
		}

		//Implement new Particle
		int i = m_CurrentParticleCount;
		int j = i - 1;

		while (i != index)
		{
			m_ParticleList[i].position.x = m_ParticleList[j].position.x;
			m_ParticleList[i].position.y = m_ParticleList[j].position.y;
			m_ParticleList[i].position.z = m_ParticleList[j].position.z;
			m_ParticleList[i].velocity = m_ParticleList[j].velocity;
			m_ParticleList[i].active = m_ParticleList[j].active;
			i--;
			j--;
		}

		//Insert Particle with proper depth order
		m_ParticleList[index].position.x = position.x;
		m_ParticleList[index].position.y = position.y;
		m_ParticleList[index].position.z = position.z;
		m_ParticleList[index].velocity = velocity;
		m_ParticleList[index].active = true;

	}

}

//Update Particle Properties
void CParticleSystem::UpdateParticles(float frameTime)
{
	//Updates vary depending on System
	for (int i = 0; i < m_CurrentParticleCount; i++)
	{
		m_ParticleList[i].position.y = m_ParticleList[i].position.y - (m_ParticleList[i].velocity * frameTime);
	}
}

//Kill Particles (Kills on Life Time Expiration)
void CParticleSystem::KillParticles()
{
	//Kill Particles Below Certain Position
	for (int i = 0; i < m_MaxParticles; i++)
	{
		//Check height of each Particle
		if (m_ParticleList[i].active == true && m_ParticleList[i].position.y < -1.0f)
		{
			m_ParticleList[i].active = false;
			m_CurrentParticleCount--;

			//Dynamically Erase Particle from List
			for (int j = i; j < m_MaxParticles; j++)
			{
				m_ParticleList[j].position.x = m_ParticleList[j + 1].position.x;
				m_ParticleList[j].position.y = m_ParticleList[j + 1].position.y;
				m_ParticleList[j].position.z = m_ParticleList[j + 1].position.z;

				m_ParticleList[j].velocity = m_ParticleList[j + 1].velocity;
				m_ParticleList[j].active = m_ParticleList[j + 1].active;
			}
		}
	}
}

//Dynamically Update Buffers
bool CParticleSystem::UpdateBuffers(ID3D11DeviceContext* deviceContext, CCamera* mainCamera)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	//Initialise Vertex Array
	memset(m_Vertices, 0, (sizeof(VertexType) * m_VertexCount));

	int index = 0;
	//Build Vertex Array
	for (int i = 0; i < m_CurrentParticleCount; i++)
	{
		//Quad Construction
		//Bottom Left
		m_Vertices[index].position =	D3DXVECTOR3(m_ParticleList[i].position.x - m_ParticleSize.x,
													m_ParticleList[i].position.y - m_ParticleSize.y,
													m_ParticleList[i].position.z);

		m_Vertices[index].texture = D3DXVECTOR2(0.0f, 1.0f);
		index++;

		//Top Left
		m_Vertices[index].position =	D3DXVECTOR3(m_ParticleList[i].position.x - m_ParticleSize.x,
													m_ParticleList[i].position.y + m_ParticleSize.y,
													m_ParticleList[i].position.z);

		m_Vertices[index].texture = D3DXVECTOR2(0.0f, 0.0f);
		index++;

		//Bottom Right
		m_Vertices[index].position =	D3DXVECTOR3(m_ParticleList[i].position.x + m_ParticleSize.x,
													m_ParticleList[i].position.y - m_ParticleSize.y,
													m_ParticleList[i].position.z);

		m_Vertices[index].texture = D3DXVECTOR2(1.0f, 1.0f);
		index++;

		//Bottom Right
		m_Vertices[index].position =	D3DXVECTOR3(m_ParticleList[i].position.x + m_ParticleSize.x,
													m_ParticleList[i].position.y - m_ParticleSize.y,
													m_ParticleList[i].position.z);

		m_Vertices[index].texture = D3DXVECTOR2(1.0f, 1.0f);
		index++;

		//Top Left
		m_Vertices[index].position =	D3DXVECTOR3(m_ParticleList[i].position.x - m_ParticleSize.x,
													m_ParticleList[i].position.y + m_ParticleSize.y,
													m_ParticleList[i].position.z);

		m_Vertices[index].texture = D3DXVECTOR2(0.0f, 0.0f);
		index++;

		//Top Right
		m_Vertices[index].position =	D3DXVECTOR3(m_ParticleList[i].position.x + m_ParticleSize.x,
													m_ParticleList[i].position.y + m_ParticleSize.y,
													m_ParticleList[i].position.z);

		m_Vertices[index].texture = D3DXVECTOR2(1.0f, 0.0f);
		index++;
	}

	//Lock Vertex Buffer
	result = deviceContext->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//Get pointer to Data in Vertex Buffer
	verticesPtr = (VertexType*)mappedResource.pData;

	//Copy to Vertex Buffer
	memcpy(verticesPtr, (void*)m_Vertices, (sizeof(VertexType) * m_VertexCount));

	//Unlock Vertex Buffer
	deviceContext->Unmap(m_VertexBuffer, 0);

	return true;
}

//Render Particle Buffers
void CParticleSystem::PrepareBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//Texture Finder
std::string CParticleSystem::TextureFinder(std::string textureLocation, std::string textureFilename , std::string textureFiletype)
{
	//Concrete Filepath
	std::string absPath = "Resources/Models/Particles/";

	//Texture Filepath Creation
	std::string finalPath = absPath + textureLocation + textureFilename + textureFiletype;

	return finalPath;
}
