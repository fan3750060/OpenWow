#pragma once

#include "GeometryBase.h"

class CDebug_Normals : public GeometryBase
{
public:
    CDebug_Normals(RenderDevice* _RenderDevice) : 
		GeometryBase(_RenderDevice, "shaders/Debug/Debug_Normal.vs", "shaders/Debug/Debug_Normal.fs", "shaders/Debug/Debug_Normal.gs")
	{}
};