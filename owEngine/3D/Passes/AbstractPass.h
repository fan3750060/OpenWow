#pragma once

#include "RenderPass.h"

// Most passes only need to override a single method of a RenderPass.
// Passes can derive from this class to minimize the number of methods that 
// must be overriden in order to implement a render pass.
// Most of the methods of the abstract pass resolve to do-nothing functions.
class AbstractPass : public IRenderPass
{
public:
	AbstractPass();
	virtual ~AbstractPass();

	// Enable or disable the pass. If a pass is disabled, the technique will skip it.
	virtual void SetEnabled(bool enabled);
	virtual bool IsEnabled() const;

	// Render the pass. This should only be called by the RenderTechnique.
	virtual void PreRender(Render3DEventArgs& e);
	virtual void Render(Render3DEventArgs& e) = 0;
	virtual void PostRender(Render3DEventArgs& e);

	// Inherited from Visitor
	virtual bool Visit(SceneNode3D& node);
	virtual bool Visit(IMesh& mesh);

private:
	bool m_Enabled;
};