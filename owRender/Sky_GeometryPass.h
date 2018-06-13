#pragma once

#include "GeometryBase.h"

class CSky_GeometryPass : public GeometryBase
{
public:
	CSky_GeometryPass(RenderDevice* _RenderDevice) : 
		GeometryBase(_RenderDevice, "shaders/Sky") 
	{}
};