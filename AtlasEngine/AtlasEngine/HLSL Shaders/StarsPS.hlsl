#define NUM_LIGHTS 6

//Globals
Texture2D Texture[4];
SamplerState SampleType;

//Buffers
cbuffer MatrixBuffer : register(b0)
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
struct PixelInputType {
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float3 viewDirection : TEXCOORD1;
	float4 worldPosition : TEXCOORD2;
};

float4 StarsPS(PixelInputType input) : SV_TARGET
{
	float transferRate = 45.0f;

	float4 starTexture = Texture[0].Sample(SampleType, input.tex);
	float4 blackTexture = float4(0.0f, 0.0f, 0.0f, 0.0f);

	if (sunlightAngle < 180.0f) { discard; }

	if (sunlightAngle >= 180.0f && sunlightAngle <= 225.0f)
	{
		float amount = (sunlightAngle - 180.0f) / transferRate;
		float4 textureColour = lerp(blackTexture, starTexture, amount);
		return textureColour;
	}

	if (sunlightAngle >= 225.0f && sunlightAngle <= 315.0f)
	{
		return starTexture;
	}

	if (sunlightAngle >= 315.0f && sunlightAngle <= 360.0f)
	{
		float amount = (sunlightAngle - 315.0f) / transferRate;
		float4 textureColour = lerp(starTexture, blackTexture, amount);
		return textureColour;
	}

	return blackTexture;
}