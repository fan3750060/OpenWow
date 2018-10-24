#include "..\\_gamedata\\shaders_D3D\\Debug\\Base_Material.h"

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
cbuffer Material : register(b2)
{
    Base_Material Mat;
};

Texture2D DiffuseTexture : register(t2);
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
	return float4(1,0,0,1);//DiffuseTexture.Sample(DiffuseSampler, IN.texCoord);
}