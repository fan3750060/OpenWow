#pragma once

#include "technique.h"

class Debug_Normals : public Technique
{
public:
    Debug_Normals(RenderDevice* _RenderDevice) : 
		Technique(_RenderDevice, "shaders/Debug/Debug_Normal.vs", "shaders/Debug/Debug_Normal.fs", "shaders/Debug/Debug_Normal.gs") 
	{}
};