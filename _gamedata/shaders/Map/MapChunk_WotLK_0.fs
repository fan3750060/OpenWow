#version 330
#include "../lib/fragDeferredWrite.glsl"

// In put
in struct
{
	vec3 WorldSpacePos;
	vec3 Normal;
	vec4 VertexColorMCCV;
	vec2 TexCoordDetail;
	vec2 TexCoordAlpha;
} VSInput;

// Diffuses
uniform sampler2D gColorMap[4];

// Speculars textures
uniform sampler2D gSpecularMap[4];

// Alpha Textures + Shadow Texture
uniform sampler2D gBlend;

// Shadow Params
uniform bool gShadowMapExists;
uniform vec3 gShadowColor;

void main()
{
	vec3 resultColor = texture(gColorMap[0], VSInput.TexCoordDetail).rgb;
	vec4 resultSpecular = texture(gSpecularMap[0], VSInput.TexCoordDetail);
		
	if (gShadowMapExists)
	{
		float alphaCurrent = texture(gBlend, VSInput.TexCoordAlpha).a;
		resultColor = resultColor * (1.0 - alphaCurrent) + gShadowColor * alphaCurrent;
	}

	//
	setMatID(1.0);
	setPos(VSInput.WorldSpacePos);
	setNormal(VSInput.Normal);
	setAlbedo(resultColor.rgb);
	setSpecParams(resultSpecular.rgb, 1.0);
};
