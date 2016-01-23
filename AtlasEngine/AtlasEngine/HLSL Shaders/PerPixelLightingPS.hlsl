#define NUM_LIGHTS 6

//Globals
Texture2D Texture[4];
SamplerState SampleType;

//Buffers
cbuffer MatrixBuffer : register(cb0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer LightPositionBuffer : register(b1)
{
	float4 lightPosition[NUM_LIGHTS];
	float4 lightColour[NUM_LIGHTS];
	float4 specularPower[NUM_LIGHTS];
	float4 Brightness[NUM_LIGHTS];
};

cbuffer LightBuffer : register(b2)
{
	float4 ambientSunColour;
	float4 ambientMoonColour;
	float4 diffuseColour;
	float4 specularColour;
	float3 sunlightDirection;
	float sunlightAngle;
	float3 moonlightDirection;
	float padding;
};

//Typedefs
struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float3 viewDirection : TEXCOORD1;
	float4 worldPosition : TEXCOORD2;
};

/////////////////////////////////////
//Pixel Shader
/////////////////////////////////////
float4 PerPixelLightingPS(PixelInputType input) : SV_TARGET
{
	float4 sunColour = ambientSunColour;
	float4 moonColour = ambientMoonColour;

	float4 sunSpecular = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 moonSpecular = float4(0.0f, 0.0f, 0.0f, 1.0f);

	// Sample the texture pixel at this location.
	float4 DiffuseMaterial = Texture[0].Sample(SampleType, input.tex);

	if (DiffuseMaterial.a < 0.1f) { discard; }

	float4 SpecularMaterial = Texture[1].Sample(SampleType, input.tex);

	// Sample the pixel in the bump map.
	float4 normalMap = (Texture[2].Sample(SampleType, input.tex) * 2.0f) - 1.0f;

	// Calculate the normal from the data in the bump map.
	float3 textureNormal = normalize((normalMap.x * input.tangent) + (normalMap.y * input.bitangent) + (normalMap.z * input.normal));

	////////////////////////////////////////////////////////////////////////////////////
	//Texture Normals (No Mesh)
	float3 halfway;
	
	//Sunlight
	halfway = normalize((float3)sunlightDirection + input.viewDirection);
	sunSpecular = ambientSunColour * pow(saturate(dot(textureNormal, halfway)), 50);

	//Moonlight 
	halfway = normalize((float3)moonlightDirection + input.viewDirection);
	moonSpecular = ambientMoonColour * pow(saturate(dot(textureNormal, halfway)), 20);

	float3 StreetlightLight = float3(0.0f, 0.0f, 0.0f);
	float3 StreetlightSpecular = float3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		//Streetlights
		float3 lightDir = normalize((float3)lightPosition[i] - (float3)input.worldPosition);
		float3 lightDist = length((float3)lightPosition[i] - (float3)input.worldPosition);
		float3 DiffuseLight = (float3)lightColour[i] * max(dot(textureNormal, lightDir), 0) / lightDist * Brightness[i].x;
		halfway = normalize((float3)lightDir + input.viewDirection);
		float3 specularLight = DiffuseLight * pow(saturate(dot(textureNormal, halfway)), specularPower[i].x);
		StreetlightLight += DiffuseLight;
		StreetlightSpecular += specularLight;
	}

	////////////////////////////////////////////////////////////////////////////////////

	float3 AmbientLight = saturate((float3)ambientSunColour * (float3)ambientMoonColour);
	float3 DiffuseLight = saturate((float3)AmbientLight + StreetlightLight);

	//Use specular map pixel to calculate the specular amount on pixel
	float3 SpecularLight = StreetlightSpecular + (float3)sunSpecular + (float3)moonSpecular;

	//Add the specular value last
	float4 colour = float4(0.0f, 0.0f, 0.0f, 1.0f);
	colour.rgb = (float3)DiffuseMaterial * DiffuseLight + (float3)SpecularMaterial * SpecularLight;

	return colour;
}