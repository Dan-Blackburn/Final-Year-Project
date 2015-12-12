#include "CShaderManager.h"
#include "tinyxml2.h"

using namespace tinyxml2;

//Constructors
CShaderManager::CShaderManager()
{
	//Variable Initialisation
	ShaderCount = 0;
	m_Shader = 0;

}

//Copy Constructor
CShaderManager::CShaderManager(const CShaderManager &other)
{

}

//Destructor
CShaderManager::~CShaderManager()
{

}

//Get Shader from List
CShader* CShaderManager::GetShader(std::string shaderName)
{
	for (std::vector<CShader*>::iterator it = m_ShaderList.begin(); it != m_ShaderList.end(); it++)
	{
		m_Shader = *it;

		if (m_Shader->GetShaderName() == shaderName)
		{
			return m_Shader;
		}
	}

	OutputDebugString("Shader not found: ");
	OutputDebugString(shaderName.c_str());
	OutputDebugString("\n");
	return 0;
}

bool CShaderManager::SetShaderType(string ShaderType)
{
	//Sets Shader Type based on Input
	if (ShaderType == "Basic") { m_Shader = new CBasicShader; return true; }
	else if (ShaderType == "Light") { m_Shader = new CLightShader; return true; }
	else
	{ 
		OutputDebugString("Error: Shader Type not recognised: ");
		OutputDebugString(ShaderType.c_str());
		OutputDebugString("\n");
		return false;
	}
}

//Intitialise Shaders
int CShaderManager::InitialiseShaders(ID3D11Device* device, HWND hwnd)
{
	const int ShaderPointerError = 100;
	const int ShaderInitialisationError = 101;
	bool result;

	//XML Variables
	tinyxml2::XMLDocument xmlDoc;
	xmlDoc.LoadFile("Resources/XML/Default.xml");

	XMLElement* shaderCount = xmlDoc.FirstChildElement("Level")->FirstChildElement("Shaders");
	shaderCount->QueryIntAttribute("Number", &ShaderCount);

	//Get Shader Attributes
	XMLElement* ShaderAttributes = xmlDoc.FirstChildElement("Level")->FirstChildElement("Shaders")->FirstChildElement("Shader");

	OutputDebugString("Loading Shaders...\n");
	//Create Shader Objects
	for (int i = 0; i < ShaderCount; i++)
	{
		XMLElement* Attributes = ShaderAttributes->FirstChildElement("Type");
		string shaderType = Attributes->GetText();
		Attributes = Attributes->NextSiblingElement();

		result = SetShaderType(shaderType);

		if (!m_Shader)
		{
			OutputDebugString("Error: Unexpected error when defining Shader Object");
			return ShaderPointerError;
		}

		//---------- XML Entity Attribute ----------//
		//Set Name
		m_Shader->SetShaderName(Attributes->GetText());
		Attributes = Attributes->NextSiblingElement();
		//Set Vertex Shader Filename
		m_Shader->SetVSFilename(Attributes->GetText());
		Attributes = Attributes->NextSiblingElement();
		//Set Pixel Shader Filename
		m_Shader->SetPSFilename(Attributes->GetText());
		Attributes = Attributes->NextSiblingElement();
		//------------------------------------------//

		//Push Model Pointer to List
		m_ShaderList.push_back(m_Shader);

		//Intialise Shader
		result = m_Shader->Initialise(device, hwnd);
		m_Shader = 0;

		if (!result)
		{
			OutputDebugString("Error: Unexpected error when initialising Shader Object");
			return ShaderInitialisationError;
		}

		ShaderAttributes = ShaderAttributes->NextSiblingElement();
	}

	for (std::vector<CShader*>::iterator it = m_ShaderList.begin(); it != m_ShaderList.end(); it++)
	{
		m_Shader = *it;
		m_Shader->Initialise(device, hwnd);
	}

	if (result)
	{
		OutputDebugString("Success!\n");
	}

	return 1;
}

//Render Shaders
int CShaderManager::RenderShaders()
{
	return 1;
}
