#include "..\\_gamedata\\shaders_D3D\\M2\\M2_Material.h"

struct VertexShaderInput
{
	float3 position  : POSITION;
	float3 normal    : NORMAL0;
	float2 texCoord0 : TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 position  : SV_POSITION;
	float3 normal    : NORMAL0;
	float2 texCoord0 : TEXCOORD0;
};


// Uniforms
cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
}
cbuffer Material : register(b2)
{
    M2_Material Material;
};

// Textures and samples
Texture2D DiffuseTexture        : register(t0);
sampler   DiffuseTextureSampler : register(s0);

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;

	OUT.position = mul(ModelViewProjection, float4(IN.position, 1.0f));
	OUT.normal = IN.normal;
	OUT.texCoord0 = IN.texCoord0;
	return OUT;
}

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	return DiffuseTexture.Sample(DiffuseTextureSampler, IN.texCoord0);
}