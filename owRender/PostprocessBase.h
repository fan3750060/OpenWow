#pragma once

#include "Technique.h"

// FORWARD BEGIN
class TechniquesManager;
// FORWARD END

class PostprocessBase : public Technique
{
	friend TechniquesManager;
public:
    PostprocessBase(RenderDevice* _RenderDevice, cstring _fsName) : 
		Technique(_RenderDevice, "shaders/Postprocess/Postprocess_VS.vs", _fsName)
    { 
		Bind();
		setTexture("gbuf0", 0);
		setTexture("gbuf1", 1);
		setTexture("gbuf2", 2);
		setTexture("gbuf3", 3);
		Unbind();

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