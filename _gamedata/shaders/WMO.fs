#version 330
#include "lib/fragDeferredWrite.glsl"

struct VSOutput
{
	vec3 WorldSpacePos;
	vec2 TexCoord;
	vec3 Normal;
	vec4 Color;
};

// Input
in VSOutput VSout;

// Uniforms
uniform sampler2D gColorMap;
uniform sampler2D gSpecularMap;

uniform bool gHasMOCV;

// Ambient color
uniform bool gUseAmbColor;
uniform vec4 gAmbColor;

void main(void)
{
	vec4 finalColor = texture(gColorMap, VSout.TexCoord);

	if (gHasMOCV)
	{
		finalColor *= VSout.Color;
	}
	
	if (gUseAmbColor)
	{
		finalColor *= gAmbColor;
	}
		
	setMatID(1.0);
	setPos(VSout.WorldSpacePos);
	setNormal(normalize(VSout.Normal));
	setAlbedo4(finalColor);
	setSpecParams(texture(gSpecularMap, VSout.TexCoord).rgb, 1.0);
};