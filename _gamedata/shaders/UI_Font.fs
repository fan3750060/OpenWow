#version 330

in vec2 OutTextCoord;

uniform sampler2D gFontTexture;
uniform vec3 gFontColor;
uniform vec2 gOffset;

// Out
out vec4 FragColor;

void main(void)
{
	FragColor = vec4(gFontColor, texture(gFontTexture, OutTextCoord).a);
};