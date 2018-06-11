#pragma once

#include "PostprocessBase.h"
#include "lights_common.h"

class POST_DirectionalLight : public PostprocessBase
{
public:
    POST_DirectionalLight(RenderDevice* _RenderDevice) : PostprocessBase(_RenderDevice, "shaders/Postprocess/Postprocess_Light_Direction.fs")
    {
        gDirectionalLight_Base_ambient = getLocation("gDirectionalLight.Base.ambient");
        gDirectionalLight_Base_diffuse = getLocation("gDirectionalLight.Base.diffuse");
        gDirectionalLight_Base_specular = getLocation("gDirectionalLight.Base.specular");
        gDirectionalLight_Direction = getLocation("gDirectionalLight.Direction");

        gSpecularPower = getLocation("gSpecularPower");
    }

    void SetDirectionalLight(const DirectionalLight& Light)
    {
        setVec3(gDirectionalLight_Base_ambient, Light.ambient);
        setVec3(gDirectionalLight_Base_diffuse, Light.diffuse);
        setVec3(gDirectionalLight_Base_specular, Light.specular);
        setVec3(gDirectionalLight_Direction, Light.Direction);
    }

    void SetMatSpecularPower(float Power)
    {
        setFloat("gSpecularPower", Power);
    }

private:
    int32 gDirectionalLight_Base_ambient;
    int32 gDirectionalLight_Base_diffuse;
    int32 gDirectionalLight_Base_specular;
    int32 gDirectionalLight_Direction;

    int32 gSpecularPower;
};