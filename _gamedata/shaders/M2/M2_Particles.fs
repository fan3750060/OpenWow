#version 330
#include "../lib/fragDeferredWrite.glsl"

// Input
in struct VSOutput
{
	vec3 WorldSpacePos;
	vec4 Color;
	vec2 TexCoord;
} VSout;

// Uniforms
uniform sampler2D gColorMap;
uniform vec4 gColor;

void main(void)
{
	vec4 resultColor = texture(gColorMap, VSout.TexCoord);
	resultColor *= VSout.Color;

	setMatID(1.0);
	setPos(VSout.WorldSpacePos);
	setNormal(vec3(0, 0, 0));
	setAlbedo4(resultColor);
	setSpecParams(vec3(0, 0, 0), 0);
};