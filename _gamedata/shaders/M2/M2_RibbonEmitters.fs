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
uniform vec4 gColor;

void main(void)
{
	vec4 resultColor = texture(gColorMap, VSout.TexCoord);
	resultColor *= gColor;


	setMatID(1.0);
	setPos(VSout.WorldSpacePos);
	setNormal(vec3(0, 0, 0));
	setAlbedo4(resultColor);
	setSpecParams(vec3(0, 0, 0), 0);
};