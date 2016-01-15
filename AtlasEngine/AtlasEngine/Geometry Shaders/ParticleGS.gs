//Typedefs
struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float3 viewDirection : TEXCOORD1;
};

/////////////////////////////////////
//Geometry Shader
/////////////////////////////////////
[maxvertexcount(4)]
void ParticleGS(point VertexInputType input[1], inout TriangleStream<PixelInputType> triStream)
{
	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 right = float3(1.0f, 0.0f, 0.0f);

	float halfWidth = 0.5f * input[0].position.x;
	float halfHeight = 0.5f * input[0].position.y;

	float4 vertex[4];
	vertex[0] = float4(input[0].position + halfWidth * right - halfHeight * up, 1.0f);
	vertex[1] = float4(input[0].position + halfWidth * right + halfHeight * up, 1.0f);
	vertex[2] = float4(input[0].position - halfWidth * right - halfHeight * up, 1.0f);
	vertex[3] = float4(input[0].position - halfWidth * right + halfHeight * up, 1.0f);

	PixelInputType output;

	for(int i = 0; i < 4; i++)
	{
		output.position = float4(vertex[0], vertex[1], vertex[2], 1.0f);
		output.tex = input.tex;
	}

	triStream.Append(output);
}