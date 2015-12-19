//Globals
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer
{
	float3 cameraPosition;
	float padding;
};

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

///////////////////////////////////////
//Vertex Shader
///////////////////////////////////////
PixelInputType BuildingLightingVS(VertexInputType input)
{
	PixelInputType output;
	float3 worldPosition;

	//Change Position Vector (to be 4 units) for Matrix Calculations
	input.position.w = 1.0f;

	//Calculate Position of Vertex against World, View and Projection Matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//Store Texture Coordinate for Pixel Shader
	output.tex = input.tex;

	//Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	//Normalize the normal vector.
	output.normal = normalize(output.normal);

	//Calculate the Position of Vertex in World
	worldPosition = mul(input.position, worldMatrix);

	//Determine Viewing Direction
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	//Normalise Viewing Direction Vector
	output.viewDirection = normalize(output.viewDirection);

	//Calculate the tangent vector against the world matrix only and then normalize the final value.
	output.tangent = mul(input.tangent, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);

	//Calculate the binormal vector against the world matrix only and then normalize the final value.
	output.bitangent = mul(input.bitangent, (float3x3)worldMatrix);
	output.bitangent = normalize(output.bitangent);

	return output;
}

