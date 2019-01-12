#include "..\\_gamedata\\shaders_D3D\\Liquid\\Liquid_Material.h"

struct VertexShaderInput
{
	float3 position : POSITION;
	float3 texCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float3 texCoord : TEXCOORD0;
};


// Uniforms
cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
}
cbuffer Material : register(b2)
{
    Liquid_Material Material;
};

// Textures and samples
Texture2D DiffuseTexture        : register(t0);
sampler   DiffuseTextureSampler : register(s0);

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.position = mul(ModelViewProjection, float4(IN.position, 1.0f));
	OUT.texCoord = IN.texCoord;
	return OUT;
}

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	float alpha = DiffuseTexture.Sample(DiffuseTextureSampler, IN.texCoord).w;

	float4 resultColor = float4(Material.gColorLight, (1.0 - IN.texCoord.z) * Material.gShallowAlpha) + float4(Material.gColorDark, IN.texCoord.z * Material.gDeepAlpha);
	resultColor *= (1.0 - alpha);
	resultColor += float4(1.0f, 1.0f, 1.0f, 1.0f) * alpha;

	return resultColor;
}