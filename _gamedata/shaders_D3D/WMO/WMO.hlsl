#include "..\\_gamedata\\shaders_D3D\\WMO\\WMO_Material.h"

struct VertexShaderInput
{
	float3 position  : POSITION;
	float3 normal    : NORMAL0;
	float4 color     : COLOR0;
	float2 texCoord0 : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
};

struct VertexShaderOutput
{
	float4 position  : SV_POSITION;
	float3 normal    : NORMAL0;
	float4 color     : COLOR0;
	float2 texCoord0 : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
};


// Uniforms
cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
}
cbuffer Material : register(b2)
{
    WMO_Material Material;
};

// Textures and samples
Texture2D DiffuseTexture        : register(t0);
sampler   DiffuseTextureSampler : register(s0);

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;

	OUT.position = mul(ModelViewProjection, float4(IN.position, 1.0f));
	OUT.normal = IN.normal;
	OUT.color = IN.color;
	OUT.texCoord0 = IN.texCoord0;
	OUT.texCoord1 = IN.texCoord1;
	
	return OUT;
}

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	return DiffuseTexture.Sample(DiffuseTextureSampler, IN.texCoord0);
}