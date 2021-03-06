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
struct PixelInputType {
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
float4 SkyboxPS(PixelInputType input) : SV_TARGET
{
	float transferRate = 45.0f;
	// Sample the texture pixel at this location.
	float4 skyTexture = Texture[0].Sample(SampleType, input.tex);
	float4 sunsetTexture = Texture[1].Sample(SampleType, input.tex);
	float4 nightTexture = float4(0.0f, 0.0f, 0.0f, 1.0f);

	if (sunlightAngle >= 45.0f && sunlightAngle <= 135.0f)
	{
		return skyTexture;
	}

	if (sunlightAngle >= 135.0f && sunlightAngle <= 180.0f)
	{
		float amount = (sunlightAngle - 135.0f) / transferRate;
		float4 textureColour = lerp(skyTexture, sunsetTexture, amount);
		return textureColour;
	}

	if (sunlightAngle >= 180.0f && sunlightAngle <= 225.0f)
	{
		float amount = (sunlightAngle - 180.0f) / transferRate;
		float4 textureColour = lerp(sunsetTexture, nightTexture, amount);
		return textureColour;
	}

	if (sunlightAngle >= 225.0f && sunlightAngle <= 315.0f)
	{
		return nightTexture;
	}

	if (sunlightAngle >= 315.0f && sunlightAngle <= 360.0f)
	{
		float amount = (sunlightAngle - 315.0f) / transferRate;
		float4 textureColour = lerp(nightTexture, sunsetTexture, amount);
		return textureColour;
	}

	if (sunlightAngle >= 0.0f && sunlightAngle <= 45.0f)
	{
		float amount = (sunlightAngle - 0.0f) / transferRate;
		float4 textureColour = lerp(sunsetTexture, skyTexture, amount);
		return textureColour;
	}

	return skyTexture;
}