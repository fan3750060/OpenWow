#version 330

struct VSOutput
{
	vec3 WorldSpacePos;
};

// Vertex attrib
layout(location = 0) in vec3 VertexPosition;

// Uniforms
uniform mat4 gProjection;
uniform mat4 gView;

// Out
out VSOutput VSout;

void main(void)
{
	mat4 PV = gProjection * gView;
	gl_Position = PV * vec4(VertexPosition, 1.0);

	VSout.WorldSpacePos = (vec4(VertexPosition, 1.0)).xyz;
};