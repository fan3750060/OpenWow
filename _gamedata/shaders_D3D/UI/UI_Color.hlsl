struct UI_Color_Material
{
    float4 Color;
};

struct VertexShaderInput
{
	float3 position : POSITION;
};
struct VertexShaderOutput
{
	float4 position : SV_POSITION;
};

cbuffer PerObject : register(b0)
{
	float4x4 ModelOrtho;
}
cbuffer Material : register(b2)
{
    UI_Color_Material Material;
};

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.position = mul(ModelOrtho, float4(IN.position, 1.0f));
	return OUT;
}

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	return Material.Color;
}