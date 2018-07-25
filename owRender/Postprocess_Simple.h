#pragma once

#include "PostprocessBase.h"

class CPOST_Simple : public PostprocessBase
{
public:
    CPOST_Simple(RenderDevice* _RenderDevice) : 
		PostprocessBase(_RenderDevice, "shaders/Postprocess/Postprocess_Simple.fs") 
	{}
};