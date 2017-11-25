#pragma once

#include "technique.h"

class POST_Fog : public PostProcess_Common
{
public:
    POST_Fog() : PostProcess_Common("shaders/POST_Fog.fs") {}

    void SetFogDistance(float _distance)
    {
        setFloat("gFogDistance", _distance);
    }

    void SetFogModifier(float _modifier)
    {
        setFloat("gFogModifier", _modifier);
    }

    void SetFogColor(cvec3 _color)
    {
        setVec3("gFogColor", _color);
    }
};