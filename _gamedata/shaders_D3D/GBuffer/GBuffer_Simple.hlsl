#include "..\\_gamedata\\shaders_D3D\\CommonTypes.h"

struct GBuffer_Material
{
    float4 DiffuseColor;
};

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

// Uniforms
cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
}
cbuffer Material : register(b2)
{
    GBuffer_Material Material;
};

Texture2D PositionWS          : register(t0); 
Texture2D DiffuseTextureVS    : register(t1);
Texture2D SpecularTextureVS   : register(t2);
Texture2D NormalTexture       : register(t3);
Texture2D DepthStencilTexture : register(t4);

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.position = mul(ModelViewProjection, float4(IN.position, 1.0f));
	OUT.texCoord = IN.texCoord;
	return OUT;
}

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	return DiffuseTextureVS.Load(int3(IN.position.x, IN.position.y, 0));
}