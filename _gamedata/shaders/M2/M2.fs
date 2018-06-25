#version 330
#include "../lib/fragDeferredWrite.glsl"

// Input
in struct VSOutput
{
	vec3 position;
	vec3 normal;
	vec2 tc0;
	vec2 tc1;
} VSout;

uniform int gShader;

// Textures
uniform sampler2D gDiffuseTexture0;
uniform sampler2D gDiffuseTexture1;
uniform sampler2D gSpecularTexture;

uniform int gBlendMode;

// Colors
uniform bool gColorEnable;
uniform vec4 gColor;

// Texture weight (alpha)
uniform bool gTextureWeightEnable;
uniform float gTextureWeight;


vec4 Test()
{
	vec4 _in = gColor;
	vec4 tex0 = texture(gDiffuseTexture0, VSout.tc0);
	vec4 tex1 = texture(gDiffuseTexture1, VSout.tc1);
	vec4 _out = vec4(0.0f);
	
	if (gTextureWeightEnable)
	{
		tex0.a *= gTextureWeight;
		tex1.a *= gTextureWeight;
	}

	if (gShader == 0)
	{
		//Combiners_Add	
		_out.rgb = _in.rgb + tex0.rgb;	
		_out.a = _in.a + tex0.a;
	}
	else if (gShader == 1)
	{
		//Combiners_Decal
		_out.rgb = mix(_in.rgb, tex0.rgb, _in.a);	
		_out.a = _in.a;
	}
	else if (gShader == 2)
	{
		//Combiners_Fade	
		_out.rgb = mix(tex0.rgb, _in.rgb, _in.a);	
		_out.a = _in.a;
	}
	else if (gShader == 3)
	{
		//Combiners_Mod	
		_out.rgb = _in.rgb * tex0.rgb;	
		_out.a = _in.a * tex0.a;
	}
	else if (gShader == 4)
	{
		//Combiners_Mod2x	
		_out.rgb = _in.rgb * tex0.rgb * 2.0;	
		_out.a = _in.a * tex0.a * 2.0;
	}
	else if (gShader == 5)
	{
		//Combiners_Opaque	
		_out.rgb = _in.rgb * tex0.rgb;	
		_out.a = _in.a;
	}
	else if (gShader == 6)
	{
		//Combiners_Add_Add	
		_out.rgb = (_in.rgb + tex0.rgb) + tex1.rgb;	
		_out.a = (_in.a + tex0.a) + tex1.a;
	}
	else if (gShader == 7)
	{
		//Combiners_Add_Mod	
		_out.rgb = (_in.rgb + tex0.rgb) * tex1.rgb;	
		_out.a = (_in.a + tex0.a) * tex1.a;
	}
	else if (gShader == 8)
	{
		//Combiners_Add_Mod2x
		_out.rgb = (_in.rgb + tex0.rgb) * tex1.rgb * 2.0;	
		_out.a = (_in.a + tex0.a) * tex1.a * 2.0;
	}
	else if (gShader == 9)
	{
		//Combiners_Add_Opaque
		_out.rgb = (_in.rgb + tex0.rgb) * tex1.rgb;	
		_out.a = _in.a + tex0.a;
	}
	else if (gShader == 10)
	{
		//Combiners_Mod_Add
		_out.rgb = (_in.rgb * tex0.rgb) + tex1.rgb;	
		_out.a = (_in.a * tex0.a) + tex1.a;
	}
	else if (gShader == 11)
	{
		//Combiners_Mod_AddNA
		_out.rgb = (_in.rgb * tex0.rgb) + tex1.rgb;	
		_out.a = _in.a * tex0.a;
	}
	else if (gShader == 12)
	{
		//Combiners_Mod_Mod
		_out.rgb = (_in.rgb * tex0.rgb) * tex1.rgb;	
		_out.a = (_in.a * tex0.a) * tex1.a;
	}
	else if (gShader == 13)
	{
		//Combiners_Mod_Mod2x	2	
		_out.rgb = (_in.rgb * tex0.rgb) * tex1.rgb * 2.0;	
		_out.a = (_in.a * tex0.a) * tex1.a * 2.0;
	}
	else if (gShader == 14)
	{
		//Combiners_Mod_Mod2xNA	2	
		_out.rgb = (_in.rgb * tex0.rgb) * tex1.rgb * 2.0;	
		_out.a = _in.a * tex0.a;
	}
	else if (gShader == 15)
	{
		//Combiners_Mod_Opaque	2	
		_out.rgb = (_in.rgb * tex0.rgb) * tex1.rgb;	
		_out.a = _in.a * tex0.a;
	}
	else if (gShader == 16)
	{
		//Combiners_Mod2x_Add	2	// TODO	// TODO
		_out = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (gShader == 17)
	{
		//Combiners_Mod2x_Mod2x		// TODO	// TODO
		_out = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (gShader == 18)
	{
		//Combiners_Mod2x_Opaque	// TODO	// TODO
		_out = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else if (gShader == 19)
	{
		//Combiners_Opaque_Add	
		_out.rgb = (_in.rgb * tex0.rgb) + tex1.rgb;	
		_out.a = _in.a + tex1.a;
	}
	else if (gShader == 20)
	{
		//Combiners_Opaque_AddAlpha
		_out.rgb = (_in.rgb * tex0.rgb) + (tex1.rgb * tex1.a);	
		_out.a = _in.a;
	}
	else if (gShader == 21)
	{
		//Combiners_Opaque_AddAlpha_Alpha	
		_out.rgb = (_in.rgb * tex0.rgb) + (tex1.rgb * tex1.a * tex0.a);	
		_out.a = _in.a;
	}
	else if (gShader == 22)
	{
		//Combiners_Opaque_AddNA	
		_out.rgb = (_in.rgb * tex0.rgb) + tex1.rgb;	
		_out.a = _in.a;
	}
	else if (gShader == 23)
	{
		//Combiners_Opaque_Mod	
		_out.rgb = (_in.rgb * tex0.rgb) * tex1.rgb;	
		_out.a = _in.a * tex1.a;
	}
	else if (gShader == 24)
	{
		//Combiners_Opaque_Mod2x	
		_out.rgb = (_in.rgb * tex0.rgb) * tex1.rgb * 2.0;	
		_out.a = _in.a * tex1.a * 2.0;
	}
	else if (gShader == 25)
	{
		//Combiners_Opaque_Mod2xNA	
		_out.rgb = (_in.rgb * tex0.rgb) * tex1.rgb * 2.0;	
		_out.a = _in.a;
	}
	else if (gShader == 26)
	{
		//Combiners_Opaque_Mod2xNA_Alpha	
		_out.rgb = (_in.rgb * tex0.rgb) * mix(tex1.rgb * 2.0, vec3(1.0), tex0.a);	
		_out.a = _in.a;
	}
	else if (gShader == 27)
	{
		//Combiners_Opaque_Opaque	
		_out.rgb = (_in.rgb * tex0.rgb) * tex1.rgb;	
		_out.a = _in.a;
	}

	return _out;
}

void main(void)
{
	vec4 resultColor = Test();
		
	if (gBlendMode == 0) // GxBlend_Opaque
	{
		resultColor.a = 1.0f;
	}
	else if(gBlendMode == 1) // GxBlend_AlphaKey
	{
		if (resultColor.a < (224.0f / 255.0f)) discard;
	}
	else 
	{
		if (resultColor.a < (1.0f / 255.0f)) discard;
	}
	
	setMatID(1.0);
	setPos(VSout.position);
	setNormal(normalize(VSout.normal));
	setAlbedo4(resultColor);
	setSpecParams(texture(gSpecularTexture, VSout.tc0).rgb, 1.0);	
};