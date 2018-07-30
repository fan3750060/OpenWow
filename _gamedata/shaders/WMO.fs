#version 330
#include "lib/fragDeferredWrite.glsl"

// Input
in struct VSOutput
{
	vec3 WorldSpacePos;
	vec3 Normal;
	vec4 Color;
	vec2 TexCoord0;
	vec2 TexCoord1;
} VSout;
out vec4 color;

// Uniforms
uniform sampler2D gColorMap0;
uniform sampler2D gColorMap1;
uniform sampler2D gColorMap2;
uniform sampler2D gSpecularMap;

uniform bool gHasMOCV;

// Blend
uniform int gBlendMode;

// Ambient color
uniform bool gUseAmbColor;
uniform vec4 gAmbColor;

void main(void)
{
	vec4 tex = texture2D(gColorMap0, VSout.TexCoord0).rgba;
    vec4 tex2 = texture2D(gColorMap1, VSout.TexCoord1).rgba;

	vec4 resultColor = tex;

	if (gHasMOCV)
	{
		// MUL
		//resultColor *= VSout.Color;

		// MULL_ADD
		vec4 newResultColor = resultColor + VSout.Color;
		resultColor *= (newResultColor);

		//resultColor /= 2.0f;
		//resultColor.rgb = mix(resultColor.rgb, VSout.Color.rgb, VSout.Color.a);
		//resultColor.rgb += VSout.Color.rgb;
	}
	
	// Blending
	if (gBlendMode == 0) // GxBlend_Opaque
	{
		resultColor.a = 1.0f;
	}
	else if (gBlendMode == 1) // GxBlend_AlphaKey
	{
		if (resultColor.a < (224.0f / 255.0f)) discard;
	}
	else 
	{
		if (resultColor.a < (1.0f / 255.0f)) discard;
	}
		
	setMatID(1.0);
	setPos(VSout.WorldSpacePos);
	setNormal(normalize(VSout.Normal));
	setAlbedo4(resultColor);
	setSpecParams(texture(gSpecularMap, VSout.TexCoord0).rgb, 1.0);
};