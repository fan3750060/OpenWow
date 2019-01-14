#pragma once

class RenderEventArgs;
class RenderUIEventArgs;
class Scene;
class SceneNode;
class UINode;
class IMesh;

// A render pass describes a single pass to render a scene.
// This could include opaque pass, transparent pass,
// g-buffer pass, or post process effects that should be applied 
// to a scene before presenting the scene to the final back buffer.
// Passes can be added to a RenderTechnique and the render technique's 
// "Render" method should be used to render all the passes in the order that 
// they are added to the technique.
class IRenderUIPass : public IVisitor, public Object
{
public:
	// Enable or disable the pass. If a pass is disabled, the technique will skip it.
	virtual void SetEnabled(bool enabled) = 0;
	virtual bool IsEnabled() const = 0;

	// Render the pass. This should only be called by the RenderTechnique.
	virtual void RenderUI(RenderUIEventArgs& e) = 0;

	// Inherited from Visitor
	virtual bool Visit(UINode& node) = 0;
	virtual bool Visit(IMesh& mesh) = 0;
};