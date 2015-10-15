//Globals
CBuffer MatrixBuffer{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
}

//Typedefs
struct VertexInputType{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
}

struct PixelInputType{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
}

///////////////////////////////////////
//Vertex Shader
///////////////////////////////////////
PixelInputType TextureVertexShader(VertexInputType input){
	PixelInputType output;

	//Change Position Vector (to be 4 units) for Matrix Calculations
	input.position.w = 1.0f;

	//Calculate Position of Vertex against World, View and Projection Matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(input.position, viewMatrix);
	output.position = mul(input.position, projectionMatrix);

	//Store Texture Coordinate for Pixel Shader
	output.tex = input.tex;

	return output;
}

