struct UI_Texture_Material
{
    float4 Color;
};

struct VertexShaderInput
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal   : NORMAL0;
};
struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal   : NORMAL0;
};

cbuffer PerObject : register(b0)
{
	float4x4 ModelOrtho;
}
cbuffer Material : register(b2)
{
    UI_Texture_Material Material;
};

Texture2D DiffuseTexture : register(t0);
sampler DiffuseTextureSampler : register(s0);

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.position = mul(ModelOrtho, float4(IN.position, 1.0f));
	OUT.texCoord = IN.texCoord;
	OUT.normal = IN.normal;
	return OUT;
}

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	return DiffuseTexture.Sample(DiffuseTextureSampler, IN.texCoord) * Material.Color;
}