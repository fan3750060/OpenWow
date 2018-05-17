#pragma once

#include "PostProcess_Common.h"
#include "lights_common.h"

class POST_Simple : public PostProcess_Common
{
public:
    POST_Simple(RenderDevice* _RenderDevice) : PostProcess_Common(_RenderDevice, "shaders/POST_Simple.fs") {}


    void SetAmbientColor(cvec3 _ambientColor)
    {
        setVec3("gAmbientColor", _ambientColor);
    }

    void SetAmbientIntensitive(float _ambientIntensitive)
    {
        setFloat("gAmbientIntensitive", _ambientIntensitive);
    }
};