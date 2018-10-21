#version 330

in vec2 OutTextCoord;

uniform sampler2D gTexture;

out vec4 FragColor;

void main(void)
{
	vec4 resultColor = texture(gTexture, OutTextCoord);

	/*if 
	(
		resultColor.r == 0.0f && 
		resultColor.g == 0.0f &&
		resultColor.b == 0.0f
	) 
	discard;*/

	FragColor = resultColor;
}
