#version 330

// Vertex attrib
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec4 Color;
layout(location = 3) in vec2 TexCoord0;
layout(location = 4) in vec2 TexCoord1;

// Input

// Uniforms
uniform mat4 gProjView;
uniform mat4 gWorld;

// Out
out struct VSOutput
{
	vec3 WorldSpacePos;
	vec3 Normal;
	vec4 Color;
	vec2 TexCoord0;
	vec2 TexCoord1;
} VSout;

void main(void)
{
	gl_Position = gProjView * gWorld * vec4(VertexPosition, 1.0);

	VSout.WorldSpacePos = (gWorld * vec4(VertexPosition, 1.0)).xyz;
	VSout.Normal = (gWorld * vec4(Normal, 0.0)).xyz;
	VSout.Color = Color;
	VSout.TexCoord0 = TexCoord0;
	VSout.TexCoord1 = TexCoord1;
};