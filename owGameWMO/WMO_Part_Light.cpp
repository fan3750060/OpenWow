#include "stdafx.h"

// Includes

// General
#include "Wmo_Part_Light.h"

WMO_Part_Light::WMO_Part_Light(const SWMO_LightDef& _proto)
{
	lightDef = _proto;

	m_Position = vec3(lightDef.pos.x, lightDef.pos.z, -lightDef.pos.y);

	fcolor = fromARGB(lightDef.color);
	//fcolor *= lightDef.intensity;
	fcolor.w = 1.0f;

	//Log::Warn("Atten = %f, %f", lightDef.attenStart, lightDef.attenEnd);
}

void WMO_Part_Light::setup(uint32 light)
{
	float LightAmbient[] = {0, 0, 0, 1.0f};
	float LightPosition[] = { m_Position.x, m_Position.y, m_Position.z, 0.0f};

	//glLightfv(light, GL_AMBIENT, LightAmbient);
	//glLightfv(light, GL_DIFFUSE, fcolor);
	//glLightfv(light, GL_POSITION, LightPosition);

	//glEnable(light);
}

void WMO_Part_Light::Render(cmat4 _world)
{
	_Render->DrawSphere(mat4(), _world * m_Position, lightDef.attenStart, fcolor);
	_Render->DrawSphere(mat4(), _world * m_Position, lightDef.attenEnd, fcolor);
}
