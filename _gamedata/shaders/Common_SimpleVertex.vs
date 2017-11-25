#version 330

layout(location = 0) in vec2 VertexPosition;

void main(void)
{
	gl_Position = vec4(VertexPosition, 0.0, 1.0);
}
