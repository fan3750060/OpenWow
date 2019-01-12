#include "..\\_gamedata\\shaders_D3D\\M2\\M2_Material.h"

struct VertexShaderInput
{
	float3 position  : POSITION;
	float4 boneWeight: BLENDWEIGHT0;
	uint4  boneIndex : BLENDINDICES0;
	float3 normal    : NORMAL0;
	float2 texCoord0 : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
};

struct VertexShaderOutput
{
	float4 position  : SV_POSITION;
	float3 normal    : NORMAL0;
	float2 texCoord0 : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
};

// Uniforms
cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
}
cbuffer Material : register(b1)
{
    M2_Material Material;
};

// Textures and samples
Texture2D DiffuseTexture        : register(t0);
sampler   DiffuseTextureSampler : register(s0);

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	float4 newVertex = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	if (Material.gIsAnimated == 1 && Material.gBonesMaxInfluences > 0u)
	{	
		for (uint i = 0; i < Material.gBonesMaxInfluences; i++)
		{
			if (IN.boneWeight[i] > 0.0f)
			{
				newVertex += mul(Material.Bones[IN.boneIndex[i]], float4(IN.position, 1.0f) * IN.boneWeight[i]);
			}
		}
	}
	else
	{
		newVertex = float4(IN.position, 1.0f);
	}


	VertexShaderOutput OUT;
	OUT.position = mul(ModelViewProjection, newVertex);
	OUT.normal = IN.normal;
	OUT.texCoord0 = IN.texCoord0;
	OUT.texCoord1 = IN.texCoord1;
	return OUT;
}

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	return DiffuseTexture.Sample(DiffuseTextureSampler, IN.texCoord0);
}