#include "..\\_gamedata\\shaders_D3D\\M2\\M2_Material.h"
#include "..\\_gamedata\\shaders_D3D\\CommonTypes.h"

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
	float4 positionVS : SV_POSITION;
	float4 positionWS : POSITION;
	float3 normal     : NORMAL0;
	float2 texCoord0  : TEXCOORD0;
	float2 texCoord1  : TEXCOORD1;
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
Texture2D DiffuseTexture0        : register(t0);
Texture2D DiffuseTexture1        : register(t1);
sampler   DiffuseTexture0Sampler : register(s0);
sampler   DiffuseTexture1Sampler : register(s1);

float4 Test(VertexShaderOutput IN);

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	float4 newVertex = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	if (Material.gIsAnimated && Material.gBonesMaxInfluences > 0u)
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
	OUT.positionVS = mul(ModelViewProjection, newVertex);
	OUT.positionWS = newVertex;
	OUT.normal = IN.normal;
	if (Material.gTextureAnimEnable)
	{
		OUT.texCoord0 = (mul(Material.gTextureAnimMatrix, float4(IN.texCoord0, 1.0f, 1.0f))).xy;
		OUT.texCoord1 = (mul(Material.gTextureAnimMatrix, float4(IN.texCoord1, 1.0f, 1.0f))).xy;
	}
	else
	{
		OUT.texCoord0 = IN.texCoord0;
		OUT.texCoord1 = IN.texCoord1;
	}
	return OUT;
}

PixelShaderOutput PS_main(VertexShaderOutput IN) : SV_TARGET
{
	float4 resultColor = Test(IN);
	
	if (Material.gBlendMode == 0) // GxBlend_Opaque
	{
		resultColor.a = 1.0f;
	}
	else if (Material.gBlendMode == 1) // GxBlend_AlphaKey
	{
		if (resultColor.a <= (224.0f / 255.0f)) discard;
	}
	else 
	{
		if (resultColor.a <= (1.0f / 255.0f)) discard;
	}
	
	PixelShaderOutput OUT;
	OUT.PositionWS = IN.positionWS;
	OUT.Diffuse = resultColor;
	OUT.Specular = float4(1.0f, 1.0f, 1.0f, 1.0f);
	OUT.NormalWS = float4(IN.normal, 1.0f);
	return OUT;
}

float4 Test(VertexShaderOutput IN)
{
	float4 tex0 = DiffuseTexture0.Sample(DiffuseTexture0Sampler, IN.texCoord0);
	float4 tex1 = DiffuseTexture1.Sample(DiffuseTexture1Sampler, IN.texCoord1);

	float4 _in  = float4(0.5f, 0.5f, 0.5f, tex0.a);
	float4 _out = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	_in = tex0;
	
	if (Material.gColorEnable)
	{
		_in.rgb *= (Material.gColor.rgb * Material.gColor.a);
	}

	if (Material.gTextureWeightEnable)
	{
		_in.a *= Material.gTextureWeight;
	}
	
	return _in;

	if (Material.gShader == 0)
	{
		//Combiners_Add	
		_out.rgb = _in.rgb + tex0.rgb;	
		_out.a = _in.a + tex0.a;
	}
	else if (Material.gShader == 1)
	{
		//Combiners_Decal
		_out.rgb = lerp(_in.rgb, tex0.rgb, _in.a);	
		_out.a = _in.a;
	}
	else if (Material.gShader == 2)
	{
		//Combiners_Fade	
		_out.rgb = lerp(tex0.rgb, _in.rgb, _in.a);	
		_out.a = _in.a;
	}
	else if (Material.gShader == 3)
	{
		//Combiners_Mod	
		_out.rgb = _in.rgb * tex0.rgb;	
		_out.a = _in.a * tex0.a;
	}
	else if (Material.gShader == 4)
	{
		//Combiners_Mod2x	
		_out.rgb = _in.rgb * tex0.rgb * 2.0;	
		_out.a = _in.a * tex0.a * 2.0;
	}
	else if (Material.gShader == 5)
	{
		//Combiners_Opaque	
		_out.rgb = _in.rgb * tex0.rgb;	
		_out.a = _in.a;
	}
	else if (Material.gShader == 6)
	{
		//Combiners_Add_Add	
		_out.rgb = (_in.rgb + tex0.rgb) + tex1.rgb;	
		_out.a = (_in.a + tex0.a) + tex1.a;
	}
	else if (Material.gShader == 7)
	{
		//Combiners_Add_Mod	
		_out.rgb = (_in.rgb + tex0.rgb) * tex1.rgb;	
		_out.a = (_in.a + tex0.a) * tex1.a;
	}
	else if (Material.gShader == 8)
	{
		//Combiners_Add_Mod2x
		_out.rgb = (_in.rgb + tex0.rgb) * tex1.rgb * 2.0;	
		_out.a = (_in.a + tex0.a) * tex1.a * 2.0;
	}
	else if (Material.gShader == 9)
	{
		//Combiners_Add_Opaque
		_out.rgb = (_in.rgb + tex0.rgb) * tex1.rgb;	
		_out.a = _in.a + tex0.a;
	}
	else if (Material.gShader == 10)
	{
		//Combiners_Mod_Add
		_out.rgb = (_in.rgb * tex0.rgb) + tex1.rgb;	
		_out.a = (_in.a * tex0.a) + tex1.a;
	}
	else if (Material.gShader == 11)
	{
		//Combiners_Mod_AddNA
		_out.rgb = (_in.rgb * tex0.rgb) + tex1.rgb;	
		_out.a = _in.a * tex0.a;
	}
	else if (Material.gShader == 12)
	{
		//Combiners_Mod_Mod
		_out.rgb = (_in.rgb * tex0.rgb) * tex1.rgb;	
		_out.a = (_in.a * tex0.a) * tex1.a;
	}
	else if (Material.gShader == 13)
	{
		//Combiners_Mod_Mod2x	2	
		_out.rgb = (_in.rgb * tex0.rgb) * tex1.rgb * 2.0;	
		_out.a = (_in.a * tex0.a) * tex1.a * 2.0;
	}
	else if (Material.gShader == 14)
	{
		//Combiners_Mod_Mod2xNA	2	
		_out.rgb = (_in.rgb * tex0.rgb) * tex1.rgb * 2.0;	
		_out.a = _in.a * tex0.a;
	}
	else if (Material.gShader == 15)
	{
		//Combiners_Mod_Opaque	2	
		_out.rgb = (_in.rgb * tex0.rgb) * tex1.rgb;	
		_out.a = _in.a * tex0.a;
	}
	else if (Material.gShader == 16)
	{
		//Combiners_Mod2x_Add	2	// TODO	// TODO
		_out = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (Material.gShader == 17)
	{
		//Combiners_Mod2x_Mod2x		// TODO	// TODO
		_out = float4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (Material.gShader == 18)
	{
		//Combiners_Mod2x_Opaque	// TODO	// TODO
		_out = float4(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else if (Material.gShader == 19)
	{
		//Combiners_Opaque_Add	
		_out.rgb = (_in.rgb * tex0.rgb) + tex1.rgb;	
		_out.a = _in.a + tex1.a;
	}
	else if (Material.gShader == 20)
	{
		//Combiners_Opaque_AddAlpha
		_out.rgb = (_in.rgb * tex0.rgb) + (tex1.rgb * tex1.a);	
		_out.a = _in.a;
	}
	else if (Material.gShader == 21)
	{
		//Combiners_Opaque_AddAlpha_Alpha	
		_out.rgb = (_in.rgb * tex0.rgb) + (tex1.rgb * tex1.a * tex0.a);	
		_out.a = _in.a;
	}
	else if (Material.gShader == 22)
	{
		//Combiners_Opaque_AddNA	
		_out.rgb = (_in.rgb * tex0.rgb) + tex1.rgb;	
		_out.a = _in.a;
	}
	else if (Material.gShader == 23)
	{
		//Combiners_Opaque_Mod	
		_out.rgb = (_in.rgb * tex0.rgb) * tex1.rgb;	
		_out.a = _in.a * tex1.a;
	}
	else if (Material.gShader == 24)
	{
		//Combiners_Opaque_Mod2x	
		_out.rgb = (_in.rgb * tex0.rgb) * tex1.rgb * 2.0;	
		_out.a = _in.a * tex1.a * 2.0;
	}
	else if (Material.gShader == 25)
	{
		//Combiners_Opaque_Mod2xNA	
		_out.rgb = (_in.rgb * tex0.rgb) * tex1.rgb * 2.0;	
		_out.a = _in.a;
	}
	else if (Material.gShader == 26)
	{
		//Combiners_Opaque_Mod2xNA_Alpha	
		_out.rgb = (_in.rgb * tex0.rgb) * lerp(tex1.rgb * 2.0, float3(1.0f, 1.0f, 1.0f), tex0.a);	
		_out.a = _in.a;
	}
	else if (Material.gShader == 27)
	{
		//Combiners_Opaque_Opaque	
		_out.rgb = (_in.rgb * tex0.rgb) * tex1.rgb;	
		_out.a = _in.a;
	}

	return _out;
}