#version 330

// Vertex attrib
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec4 VertexColorMCCV;
layout(location = 3) in vec2 TexCoordDetail;
layout(location = 4) in vec2 TexCoordAlpha;

// Output
out struct
{
	vec3 WorldSpacePos;
	vec3 Normal;
	vec4 VertexColorMCCV;
	vec2 TexCoordDetail;
	vec2 TexCoordAlpha;
} VSInput;

// Uniforms
uniform mat4 gProjView;

void main(void)
{
	gl_Position = gProjView * vec4(VertexPosition, 1.0);

	VSInput.WorldSpacePos   = VertexPosition;
	VSInput.Normal          = vec3(-Normal.y, Normal.z, -Normal.x) / 127.0f; // Map chunk specific
	VSInput.VertexColorMCCV = VertexColorMCCV;
	VSInput.TexCoordDetail  = TexCoordDetail;
	VSInput.TexCoordAlpha   = TexCoordAlpha;
};