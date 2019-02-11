#include "..\\_gamedata\\shaders_D3D\\Sky\\Sky_Material.h"
#include "..\\_gamedata\\shaders_D3D\\CommonTypes.h"

struct VertexShaderInput
{
	float3 position : POSITION;
	float4 color    : COLOR0;
};

struct VertexShaderOutput
{
	float4 positionVS : SV_POSITION;
	float4 positionWS : POSITION;
	float4 color    : COLOR0;
};


// Uniforms
cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
}
cbuffer Material : register(b2)
{
   Sky_Material Material;
};


VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.positionVS = mul(ModelViewProjection, float4(IN.position, 1.0f));
	OUT.positionWS = float4(IN.position, 1.0f);
	OUT.color = IN.color;
	return OUT;
}

PixelShaderOutput PS_main(VertexShaderOutput IN) : SV_TARGET
{
	PixelShaderOutput OUT;
	OUT.PositionWS = IN.positionWS;
	OUT.Diffuse = float4(IN.color.rgb, 1.0f);
	return OUT;
}