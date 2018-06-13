#pragma once

#include "PostprocessBase.h"

class CPOST_Simple : public PostprocessBase
{
public:
    CPOST_Simple(RenderDevice* _RenderDevice) : 
		PostprocessBase(_RenderDevice, "shaders/Postprocess/Postprocess_Simple.fs") 
	{}


    void SetAmbientColor(cvec3 _ambientColor)
    {
        setVec3("gAmbientColor", _ambientColor);
    }

    void SetAmbientIntensitive(float _ambientIntensitive)
    {
        setFloat("gAmbientIntensitive", _ambientIntensitive);
    }
};