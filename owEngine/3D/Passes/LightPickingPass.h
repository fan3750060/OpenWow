#pragma once

#include "LightsPass.h"

class PipelineState;
struct Light;

/**
 * A rendering pass that is used to 
 * create a texture that can be used to determine
 * which light is clicked with the mouse.
 */
class LightPickingPass : public LightsPass
{
public:
    typedef LightsPass base;

    LightPickingPass( std::vector<Light>& lights, std::shared_ptr<Scene3D> pointLight, std::shared_ptr<Scene3D> spotLight, std::shared_ptr<Scene3D> directionalLight, std::shared_ptr<PipelineState> pipeline );
    virtual ~LightPickingPass();

    virtual void PreRender( Render3DEventArgs& e );

    virtual bool Visit( IMesh& mesh );

private:

    __declspec( align( 16 ) ) struct LightParams
    {
        uint32_t m_LightIndex;
    };
    LightParams* m_pLightParams;
    std::shared_ptr<ConstantBuffer> m_LightParamsCB;

	std::weak_ptr<IRenderDevice> m_RenderDevice;

};