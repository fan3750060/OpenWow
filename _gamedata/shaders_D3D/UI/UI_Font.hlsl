#include "..\\_gamedata\\shaders_D3D\\UI\\UI_Font_Material.h"

struct VertexShaderInput
{
	float2 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

cbuffer PerObject : register(b0)
{
	float4x4 ModelOrtho;
}
cbuffer Material : register(b2)
{
    UI_Font_Material Material;
};

Texture2D DiffuseTexture : register(t0);
sampler DiffuseTextureSampler : register(s0);

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.position = mul(ModelOrtho, (float4(IN.position, 0.0f, 1.0f) + float4(Material.Offset, 0.0f, 0.0f)));
	OUT.texCoord = IN.texCoord;
	return OUT;
}

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	return float4(Material.Color.rgb, Material.Color.a * DiffuseTexture.Sample(DiffuseTextureSampler, IN.texCoord).a);
}