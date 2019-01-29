struct Debug_Material
{
    float4 DiffuseColor;
};

// Uniforms
cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
}
cbuffer Material : register(b2)
{
    Debug_Material Material;
};

float4 VS_main(float3 position)
{
	return mul(ModelViewProjection, float4(position, 1.0f));
}

float4 PS_main(float4 pos) : SV_TARGET
{
	return Material.DiffuseColor;
}