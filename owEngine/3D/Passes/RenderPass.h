#pragma once

class Render3DEventArgs;
class Scene3D;
class SceneNode3D;
class IMesh;
class CLight3D;

// A render pass describes a single pass to render a scene.
// This could include opaque pass, transparent pass,
// g-buffer pass, or post process effects that should be applied 
// to a scene before presenting the scene to the final back buffer.
// Passes can be added to a RenderTechnique and the render technique's 
// "Render" method should be used to render all the passes in the order that 
// they are added to the technique.
class IRenderPass : public IVisitor, public Object
{
public:
	// Enable or disable the pass. If a pass is disabled, the technique will skip it.
	virtual void SetEnabled(bool enabled) = 0;
	virtual bool IsEnabled() const = 0;

	// Render the pass. This should only be called by the RenderTechnique.
	virtual void PreRender(Render3DEventArgs& e) = 0;
	virtual void Render(Render3DEventArgs& e) = 0;
	virtual void PostRender(Render3DEventArgs& e) = 0;

	// Inherited from Visitor
	virtual bool Visit(SceneNode3D& node) = 0;
	virtual bool Visit(IMesh& mesh) = 0;
	virtual bool Visit(CLight3D& light) = 0;
};