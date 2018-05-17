#pragma once

#include "technique.h"

class Sky_GeometryPass : public Technique
{
public:
	Sky_GeometryPass(RenderDevice* _RenderDevice) : Technique(_RenderDevice, "shaders/Sky") {}
};