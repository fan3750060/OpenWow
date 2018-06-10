#include "stdafx.h"

// Includes

// General
#include "Wmo_Part_Light.h"

WMO_Part_Light::WMO_Part_Light(const WMO_LightDef& _proto)
{
	lightDef = _proto;

	lightDef.pos = vec3(lightDef.pos.x, lightDef.pos.z, -lightDef.pos.y);

	fcolor = fromARGB(lightDef.color);
	fcolor *= lightDef.intensity;
	fcolor.w = 1.0f;

	//Log::Warn("Atten = %f, %f", lightDef.attenStart, lightDef.attenEnd);
}

void WMO_Part_Light::setup(uint32 light)
{
	float LightAmbient[] = {0, 0, 0, 1.0f};
	float LightPosition[] = {lightDef.pos.x, lightDef.pos.y, lightDef.pos.z, 0.0f};

	//glLightfv(light, GL_AMBIENT, LightAmbient);
	//glLightfv(light, GL_DIFFUSE, fcolor);
	//glLightfv(light, GL_POSITION, LightPosition);

	//glEnable(light);
}