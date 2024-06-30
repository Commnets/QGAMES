// Used to pass parameters from the application buffer to the Vertex Shader!
struct VertexShaderInput
{
    float4 Position : POSITION0;
	float4 Color : COLOR0;
	float2 TextureCoords : TEXCOORD0;
};

// Used to pass parameters from the vertext shader output buffer to the Fragment / Pixel Shader!
struct VertexShaderOutput
{
    float4 Position : POSITION0;
	float4 Color : COLOR0;
	float2 TextureCoords : TEXCOORD0;
};

// The simpliest operation possible to convert the vertext position into a 2D position
// Using the conversion matrix passed also as parameter...
VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.Position = input.Position;
	output.Color = input.Color;
	output.TextureCoords = input.TextureCoords;
	
    return output;
}

// The the shader is just to assign the color....
// So the color is fixed...it doesn't depend on any texture behind...
float4 PixelShaderFunction(VertexShaderOutput input, sampler2D atlas : register(s0)) : COLOR0
{
    return tex2D(atlas, input.TextureCoords) * input.Color;
}