#pragma once

class RenderUIEventArgs;
class Scene;
class SceneNode;
class UINode;
class IMesh;

class IRenderUIPass : public IVisitor, public Object
{
public:
	// Enable or disable the pass. If a pass is disabled, the technique will skip it.
	virtual void SetEnabled(bool enabled) = 0;
	virtual bool IsEnabled() const = 0;

	// Render the pass. This should only be called by the RenderTechnique.
	virtual void RenderUI(RenderUIEventArgs& e) = 0;

	// Inherited from Visitor
	virtual bool Visit(SceneNode& node) = 0;
	virtual bool Visit(UINode& node) = 0;
	virtual bool Visit(IMesh& mesh) = 0;
};