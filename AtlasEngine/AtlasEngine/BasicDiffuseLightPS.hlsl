//Globals
Texture2D Texture[4];
SamplerState SampleType;

//Buffers
cbuffer LightBuffer
{
	float4 diffuseColour;
	float3 lightDirection;
	float padding;
};

//Typedefs
struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

/////////////////////////////////////
//Pixel Shader
/////////////////////////////////////
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColour;
	float3 lightDir;
	float lightIntensity;
	float4 colour;
	float4 normal;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColour = Texture[0].Sample(SampleType, input.tex);
	normal = Texture[3].Sample(SampleType, input.tex);

	//Invert the Light Direction for Calculations
	lightDir = -lightDirection;

	//Calculate the amount of Light on this Pixel
	lightIntensity = saturate(dot(input.normal, lightDir));

	//Determine the final amount of diffuse colour based on the diffuse color combined with the light intensity.
	colour = saturate(diffuseColour * lightIntensity);

	//Multiply the texture pixel by the final diffuse colour to get the final pixel colour result.
	colour = colour * textureColour;

    return colour;
}