#include "..\\_gamedata\\shaders_D3D\\CommonTypes.h"

struct Debug_Material
{
    float4 DiffuseColor;
};

struct VertexShaderInput
{
	float3 position       : POSITION;
};
struct VertexShaderOutput
{
	float4 position       : SV_POSITION;
};

// Uniforms
cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
}
cbuffer Material : register(b2)
{
    Debug_Material Material;
};

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.position = mul(ModelViewProjection, float4(IN.position, 1.0f));
	return OUT;
}

PixelShaderOutput PS_main(VertexShaderOutput IN) : SV_TARGET
{
	PixelShaderOutput OUT;
	OUT.Diffuse = Material.DiffuseColor;
	return OUT;
}