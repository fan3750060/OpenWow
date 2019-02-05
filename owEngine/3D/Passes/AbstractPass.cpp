#include <stdafx.h>

// General
#include "AbstractPass.h"

AbstractPass::AbstractPass()
	: m_Enabled(true)
{}

AbstractPass::~AbstractPass()
{}

// Enable or disable the pass. If a pass is disabled, the technique will skip it.
void AbstractPass::SetEnabled(bool enabled)
{
	m_Enabled = enabled;
}

bool AbstractPass::IsEnabled() const
{
	return m_Enabled;
}

// Render the pass. This should only be called by the RenderTechnique.
void AbstractPass::PreRender(Render3DEventArgs& e)
{}

void AbstractPass::Render(Render3DEventArgs& e)
{}

void AbstractPass::PostRender(Render3DEventArgs& e)
{}


// Inherited from Visitor

bool AbstractPass::Visit(SceneNode3D& node)
{
	return false;
}

bool AbstractPass::Visit(IMesh& mesh)
{
	return false;
}
