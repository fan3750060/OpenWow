#include "..\\_gamedata\\shaders_D3D\\Sky\\Sky_Material.h"

struct VertexShaderInput
{
	float3 position : POSITION;
	float4 color    : COLOR0;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float4 color    : COLOR0;
};


// Uniforms
cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
}
cbuffer Material : register(b2)
{
   Sky_Material Material;
};


VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.position = mul(ModelViewProjection, float4(IN.position, 1.0f));
	OUT.color = IN.color;
	return OUT;
}

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	return float4(IN.color.rgb, 1.0f);
}