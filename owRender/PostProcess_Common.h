#pragma once

class PostProcess_Common : public Technique
{
public:
    PostProcess_Common(cstring _fsName) : Technique("shaders/Common_SimpleVertex.vs", _fsName) 
    { 
        gScreenSize     = getLocation("gScreenSize");
        gCameraPosition = getLocation("gCameraPosition");
    }

    void SetScreenSize(float _width, float _height)
    {
        setVec2(gScreenSize, vec2(_width, _height));
    }

    void SetCameraPos(cvec3 _cameraPos)
    {
        setVec3(gCameraPosition, _cameraPos);
    }

private:
    int32 gScreenSize;
    int32 gCameraPosition;
};