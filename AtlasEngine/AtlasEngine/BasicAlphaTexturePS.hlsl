//Globals
Texture2D Texture[4];
SamplerState SampleType;

//Typedefs
struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

/////////////////////////////////////
//Pixel Shader
/////////////////////////////////////
float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColour;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColour = Texture[0].Sample(SampleType, input.tex);
	if (textureColour.a < 0.1f)
	{
		discard;
	}

    return textureColour;
}