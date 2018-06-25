#version 330

struct VSOutput
{
	vec3 WorldSpacePos;
	vec2 TexCoord0;
	vec2 TexCoord1;
	vec3 Normal;
	vec4 Color;
};

// Vertex attrib
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 TexCoord0;
layout(location = 2) in vec2 TexCoord1;
layout(location = 3) in vec3 Normal;
layout(location = 4) in vec4 Color;

// Input

// Uniforms
uniform mat4 gProjection;
uniform mat4 gView;
uniform mat4 gWorld;

// Out
out VSOutput VSout;

void main(void)
{
	mat4 PVW = gProjection * gView * gWorld;
	gl_Position = PVW * vec4(VertexPosition, 1.0);

	VSout.WorldSpacePos = (gWorld * vec4(VertexPosition, 1.0)).xyz;
	VSout.TexCoord0 = TexCoord0;
	VSout.TexCoord1 = TexCoord1;
	VSout.Normal = (gWorld * vec4(Normal, 0.0)).xyz;
	VSout.Color = Color;
};