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

cbuffer LightBuffer : register(cb1)
{
	float4 ambientColour;
	float4 diffuseColour;
	float4 specularColour;
	float3 lightDirection;
	float specularPower;
	float padding;
};

//Typedefs
struct PixelInputType {
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float3 viewDirection : TEXCOORD1;
};

/////////////////////////////////////
//Pixel Shader
/////////////////////////////////////
float4 ParticlePS(PixelInputType input) : SV_TARGET
{
	// Sample the texture pixel at this location.
	float4 textureColour = Texture[0].Sample(SampleType, input.tex);

	if (textureColour.a < 0.1f)
	{
		discard;
	}

	textureColour = textureColour * textureColour.a;

	return textureColour;
}