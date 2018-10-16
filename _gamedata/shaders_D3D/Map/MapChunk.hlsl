struct VertexShaderInput
{
	float3 position       : POSITION;
	float3 normal         : NORMAL0;
	float2 texCoordDetail : TEXCOORD0;
	float2 texCoordAlpha  : TEXCOORD1;
};

struct VertexShaderOutput
{
	float3 position       : POSITION;
	float3 normal         : NORMAL0;
	float2 texCoordDetail : TEXCOORD0;
	float2 texCoordAlpha  : TEXCOORD1;
};


// Uniforms
cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
}

// Textures and samples
Texture2D ColorMap0 : register(t0);
Texture2D ColorMap1 : register(t1);
Texture2D ColorMap2 : register(t2);
Texture2D ColorMap3 : register(t3);
sampler   ColorMapSampler : register(s0);

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;

	OUT.position = mul(ModelViewProjection, float4(IN.position, 1.0f));
	OUT.normal = IN.normal;
	OUT.texCoordDetail = IN.texCoordDetail;
	OUT.texCoordAlpha = IN.texCoordAlpha;
	
	return OUT;
}

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	for (int i = 0; i < 4; i++)
	{

	}

	return ColorMap0.Sample(ColorMapSampler, IN.texCoordDetail);
}