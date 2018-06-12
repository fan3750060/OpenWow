#version 330
#include "../lib/fragDeferredRead.glsl"

struct BaseLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct DirectionalLight
{
	BaseLight Base;
	vec3 Direction;
};

// Light type
uniform DirectionalLight gDirectionalLight;

// Specular
uniform float gSpecularPower; // Shrinesses

// Screen
uniform vec3 gCameraPosition;
uniform vec2 gScreenSize;

// OUT
out vec4 FragColor;

// Local
vec2 pixelXY;
vec3 WorldPos;
vec3 Normal;
vec4 Color;

vec3 CalcLightInternal(BaseLight Light, vec3 LightDirection)
{
	float DiffuseFactor = max(dot(Normal, -LightDirection), 0.0f);
	vec3 DiffuseColor = vec3(Light.diffuse) * DiffuseFactor;
	
	vec3 SpecularColor = vec3(0, 0, 0);

	if (DiffuseFactor > 0)
	{
		vec3 VertexToEye = normalize(gCameraPosition - WorldPos);
		vec3 LightReflect = normalize(reflect(LightDirection, Normal));

		float SpecularFactor = dot(VertexToEye, LightReflect);
		SpecularFactor = pow(SpecularFactor, gSpecularPower);

		if (SpecularFactor > 0)
		{
			SpecularColor = vec3(getSpecParams(pixelXY).rgb) * SpecularFactor;
		}
	}

	return Light.ambient + DiffuseColor + SpecularColor;
}

vec3 CalcDirectionalLight()
{
	return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction);
}

// -----------------------------------------------------------------------------------------------

void main(void)
{
	pixelXY = gl_FragCoord.xy / gScreenSize;
	
	WorldPos = getPos(pixelXY);
	Normal = getNormal(pixelXY);
	Color = getAlbedo4(pixelXY);
	//Color.a = 1.0f;
	
	if(getMatID(pixelXY) >= 0)
	{
		FragColor =  Color * vec4(CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction), 1.0f);
	}
	else
	{
		FragColor = Color;
	}
}
