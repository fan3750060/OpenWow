#include "..\\_gamedata\\shaders_D3D\\Material\\MaterialDebug.h"

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
    MaterialDebug Material;
};

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;

	OUT.position = mul(ModelViewProjection, float4(IN.position, 1.0f));
	
	return OUT;
}

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	return Material.DiffuseColor;
}