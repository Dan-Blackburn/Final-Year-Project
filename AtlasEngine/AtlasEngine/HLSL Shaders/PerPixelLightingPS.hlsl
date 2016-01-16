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
	float4 ambientSunColour;
	float4 ambientMoonColour;
	float4 diffuseColour;
	float4 specularColour;
	float3 sunlightDirection;
	float sunlightAngle;
	float3 moonlightDirection;
	float specularPower;
};

//Typedefs
struct PixelInputType{
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
float4 PerPixelLightingPS(PixelInputType input) : SV_TARGET
{
	float4 textureColour;
	float4 normalMap;
	float3 textureNormal;
	float3 sunlightDir;
	float3 moonlightDir;
	float sunlightIntensity;
	float moonlightIntensity;
	float3 reflection;
	float4 sunSpecular;
	float4 moonSpecular;
	float4 specularIntensity;

	float4 sunColour = ambientSunColour;
	float4 moonColour = ambientMoonColour;

	sunSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	moonSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sample the texture pixel at this location.
	textureColour = Texture[0].Sample(SampleType, input.tex);

	specularIntensity = Texture[1].Sample(SampleType, input.tex);

	// Sample the pixel in the bump map.
	normalMap = Texture[2].Sample(SampleType, input.tex);

	// Expand the range of the normal value from (0, +1) to (-1, +1).
	normalMap = (normalMap * 2.0f) - 1.0f;

	// Calculate the normal from the data in the bump map.
	textureNormal = (normalMap.x * input.tangent) + (normalMap.y * input.bitangent) + (normalMap.z * input.normal);

	// Normalize the resulting bump normal.
	textureNormal = normalize(textureNormal);

	// Invert the light direction for calculations.
	sunlightDir = -sunlightDirection;
	moonlightDir = -moonlightDirection;

	////////////////////////////////////////////////////////////////////////////////////
	//Texture Normals (No Mesh)

	// Calculate the amount of light on this pixel based on the bump map normal value.
	sunlightIntensity = saturate(dot(input.normal, sunlightDir));
	moonlightIntensity = saturate(dot(input.normal, moonlightDir));

	// Calculate the amount of light on this pixel based on the bump map normal value.
	float sunBumpIntensity = saturate(dot(textureNormal, sunlightDir));
	float moonBumpIntensity = saturate(dot(textureNormal, moonlightDir));
	
	if (sunBumpIntensity > 0.0f)
	{
		sunColour += saturate(sunColour * sunBumpIntensity) / 2;

		sunColour = saturate(sunColour);

		reflection = normalize(2.0f * sunlightIntensity * textureNormal - sunlightDir);

		sunSpecular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
	}

	if (moonBumpIntensity > 0.0f)
	{
		moonColour += saturate(moonColour * moonBumpIntensity) / 2;

		moonColour = saturate(moonColour);

		reflection = normalize(2.0f * moonlightIntensity * textureNormal - moonlightDir);

		moonSpecular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
	}


	//Combine the final bump light with the texture colour
	float4 colour = sunColour * moonColour;
	colour = colour * textureColour;
	colour = saturate(colour);

	if (sunlightAngle < 180.0f)
	{
		//Use specular map pixel to calculate the specular amount on pixel
		sunSpecular = sunSpecular * specularIntensity;

		//Add the specular value last
		sunSpecular = specularColour * sunSpecular;
		colour = colour + sunSpecular;
	}

	if (sunlightAngle > 180.0f) 
	{
		//Use specular map pixel to calculate the specular amount on pixel
		moonSpecular = moonSpecular * specularIntensity;

		//Add the specular value last
		moonSpecular = specularColour * moonSpecular;
		colour = colour + moonSpecular;
	}
	return colour;
}