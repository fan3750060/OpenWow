#pragma once

#include "BasePass.h"

// Use this pass to render the lights as geometry in the scene.
class LightsPass : public BasePass
{
public:
    typedef BasePass base;

    LightsPass( std::vector<Light>& lights, std::shared_ptr<Scene3D> pointLight, std::shared_ptr<Scene3D> spotLight, std::shared_ptr<Scene3D> directionalLight, std::shared_ptr<PipelineState> pipeline );
    virtual ~LightsPass();

    // Render the pass. This should only be called by the RenderTechnique.
//    virtual void PreRender( Render3DEventArgs& e );
    virtual void Render( Render3DEventArgs& e );

    // Inherited from Visitor
    virtual bool Visit( Scene3D& scene );
    virtual bool Visit( SceneNode3D& node );
    virtual bool Visit( IMesh& mesh );

protected:
    const Light* GetCurrentLight();
    const uint32_t GetCurrentLightIndex();

private:
    std::vector<Light>& m_Lights;
    // The light we are currently rendering.
    Light* m_pCurrentLight;
    uint32_t m_uiLightIndex;

    std::weak_ptr<IRenderDevice> m_RenderDevice;

    // A material that can be used to render the lights as geometry in the scene.
    std::shared_ptr<Material> m_LightMaterial;

    std::shared_ptr<PipelineState> m_Pipeline;

    std::shared_ptr<Scene3D> m_PointLightScene;
    std::shared_ptr<Scene3D> m_pSpotLightScene;
    std::shared_ptr<Scene3D> m_pDirectionalLightScene;

};