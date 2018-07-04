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



// Layers
uniform bool gIsNortrend;
uniform bool gIsMCCVExists;
uniform int gLayersCount;

// Diffuses
uniform sampler2D gColorMap[4];

// Alpha Textures + Shadow Texture
uniform sampler2D gBlend;

// Speculars textures
uniform sampler2D gSpecularMap[4];

uniform int gIsLowRes;

// Shadow Params
uniform bool gShadowMapExists;
uniform vec3 gShadowColor;

void main()
{
	vec3 layersColor = vec3(0);
	vec4 layersSpecular = vec4(0);
	vec3 resultColor = vec3(0);
	vec4 resultSpecular = vec4(0);

	if (gIsNortrend)
	{
		float alphaSumma = 0.0;
		for(int i = 1; i < gLayersCount; i++)
		{
			float alphaCurrent = texture(gBlend, VSInput.TexCoordAlpha)[i - 1];
			alphaSumma += alphaCurrent;
			layersColor += texture(gColorMap[i], VSInput.TexCoordDetail).rgb * alphaCurrent;
			layersSpecular += texture(gSpecularMap[i], VSInput.TexCoordDetail) * alphaCurrent;
		}

		resultColor = texture(gColorMap[0], VSInput.TexCoordDetail).rgb * (1.0 - alphaSumma) + layersColor;
		resultSpecular = texture(gSpecularMap[0], VSInput.TexCoordDetail) * (1.0 - alphaSumma) + layersSpecular;
	}
	else
	{
		layersColor = texture(gColorMap[0], VSInput.TexCoordDetail).rgb;
		layersSpecular = texture(gSpecularMap[0], VSInput.TexCoordDetail);
		for(int i = 1; i < gLayersCount; i++)
		{
			float alphaCurrent = texture(gBlend, VSInput.TexCoordAlpha)[i - 1];
			layersColor *= (1.0 - alphaCurrent);
			layersColor += texture(gColorMap[i], VSInput.TexCoordDetail).rgb * alphaCurrent;
			layersSpecular *= (1.0 - alphaCurrent);
			layersSpecular += texture(gSpecularMap[i], VSInput.TexCoordDetail) * alphaCurrent;
		}

		resultColor = layersColor;
		resultSpecular = layersSpecular;
	}
			
	if (gShadowMapExists)
	{
		float alphaCurrent = texture(gBlend, VSInput.TexCoordAlpha).a;
		resultColor = resultColor * (1.0 - alphaCurrent) + gShadowColor * alphaCurrent;
	}

	if (gIsMCCVExists)
	{
		resultColor *= (VSInput.VertexColorMCCV.rgb * 2.0f);
	}

	if (gIsLowRes == 2) // Low
	{
		resultColor *= vec3(1.0f, 0.7f, 0.7f);
	}
	else if (gIsLowRes == 1) // Default
	{
		resultColor *= vec3(0.8f, 1.0f, 0.8f);
	}

	//
	setMatID(1.0);
	setPos(VSInput.WorldSpacePos);
	setNormal(VSInput.Normal);
	setAlbedo(resultColor.rgb);
	setSpecParams(resultSpecular.rgb, 1.0);
};
