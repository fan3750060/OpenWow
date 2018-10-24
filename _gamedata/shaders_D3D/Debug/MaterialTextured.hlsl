#include "..\\_gamedata\\shaders_D3D\\Materials\\MaterialTextured.h"

struct VertexShaderInput
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
}

cbuffer MaterialBase : register(b2)
{
    MaterialTextured Mat;
};

Texture2D DiffuseTexture : register(t0);
sampler DiffuseSampler : register(s0);

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.position = mul(ModelViewProjection, float4(IN.position, 1.0f));
	OUT.texCoord = IN.texCoord;

	return OUT;
}

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	return DiffuseTexture.Sample(DiffuseSampler, IN.texCoord);
}