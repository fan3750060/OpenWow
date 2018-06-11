#version 330
#include "../lib/fragDeferredWrite.glsl"

struct VSOutput
{
	vec3 WorldSpacePos;
	vec2 TexCoord;
};

// Input
in VSOutput VSout;

// Uniforms
uniform sampler2D gColorMap;

void main(void)
{
	setMatID(1.0);
	setPos(VSout.WorldSpacePos);
	setNormal(vec3(0, 0, 0));
	setAlbedo4(texture(gColorMap, VSout.TexCoord));
	setSpecParams(vec3(0, 0, 0), 0);
};