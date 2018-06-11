#pragma once

#include "PostprocessBase.h"

class POST_Simple : public PostprocessBase
{
public:
    POST_Simple(RenderDevice* _RenderDevice) : PostprocessBase(_RenderDevice, "shaders/Postprocess/Postprocess_Simple.fs") {}


    void SetAmbientColor(cvec3 _ambientColor)
    {
        setVec3("gAmbientColor", _ambientColor);
    }

    void SetAmbientIntensitive(float _ambientIntensitive)
    {
        setFloat("gAmbientIntensitive", _ambientIntensitive);
    }
};