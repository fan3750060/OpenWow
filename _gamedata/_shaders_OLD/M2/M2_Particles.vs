#version 330

// Vertex attrib
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec4 Color;
layout(location = 2) in vec2 TexCoord;


// Input

// Uniforms
uniform mat4 gProjView;
uniform mat4 gWorld;

// Out
out struct VSOutput
{
	vec3 WorldSpacePos;
	vec4 Color;
	vec2 TexCoord;
} VSout;

void main(void)
{
	vec4 worldVertex = gWorld * vec4(VertexPosition, 1.0);
	gl_Position = gProjView * worldVertex;

	VSout.WorldSpacePos = worldVertex.xyz;
	VSout.Color = Color;
	VSout.TexCoord = TexCoord;
};
