#include "stdafx.h"

// Include
#include "RenderDevice.h"
#include "Shader.h"

// General
#include "Material.h"

Material::Material(RenderDevice* renderDevice)
	: m_RenderDevice(renderDevice)
	, m_Dirty(false)
{
}

Material::~Material()
{
}
