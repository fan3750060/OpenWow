#pragma once

#include "technique.h"
#include "../lights_common.h"

class POST_Simple : public PostProcess_Common
{
public:
    POST_Simple() : PostProcess_Common("shaders/POST_Simple.fs") {}


    void SetAmbientColor(cvec3 _ambientColor)
    {
        setVec3("gAmbientColor", _ambientColor);
    }

    void SetAmbientIntensitive(float _ambientIntensitive)
    {
        setFloat("gAmbientIntensitive", _ambientIntensitive);
    }
};