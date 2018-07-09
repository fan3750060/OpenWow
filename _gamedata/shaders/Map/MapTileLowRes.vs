#version 330

// Vertex attrib
layout(location = 0) in vec3 VertexPosition;

// Uniforms
uniform mat4 gProjView;

// Out
out struct VSOutput
{
	vec3 WorldSpacePos;
} VSout;

void main(void)
{
	gl_Position = gProjView * vec4(VertexPosition, 1.0);

	VSout.WorldSpacePos = (vec4(VertexPosition, 1.0)).xyz;
};