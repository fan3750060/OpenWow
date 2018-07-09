#version 330

// Vertex attrib
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec4 Color;

// Input

// Uniforms
uniform mat4 gProjView;
uniform mat4 gWorld;

// Out
out struct VSOutput
{
	vec3 WorldSpacePos;
	vec3 Color;
} VSout;

void main(void)
{
	gl_Position = gProjView * gWorld * vec4(VertexPosition, 1.0);

	VSout.WorldSpacePos = (gWorld * vec4(VertexPosition, 1.0)).xyz;
	VSout.Color = Color.rgb;
};