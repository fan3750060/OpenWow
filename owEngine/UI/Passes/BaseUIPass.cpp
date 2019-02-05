#include <stdafx.h>

#include <Application.h>
#include <SceneUI.h>
#include <SceneNodeUI.h>

// General
#include "BaseUIPass.h"

BaseUIPass::BaseUIPass()
	: m_Enabled(true)
	, m_RenderDevice(_RenderDevice)
{
}

BaseUIPass::BaseUIPass(std::shared_ptr<SceneUI> uiScene, std::shared_ptr<PipelineState> pipeline)
	: m_Enabled(true)
	, m_UIScene(uiScene)
	, m_Pipeline(pipeline)
	, m_RenderDevice(_RenderDevice)
{
}

BaseUIPass::~BaseUIPass()
{}

void BaseUIPass::SetEnabled(bool enabled)
{
	m_Enabled = enabled;
}

bool BaseUIPass::IsEnabled() const
{
	return m_Enabled;
}

void BaseUIPass::RenderUI(RenderUIEventArgs& e)
{
	if (m_Pipeline)
	{
		m_Pipeline->Bind();
	}

	if (m_UIScene)
	{
		m_UIScene->Render(e);
	}

	if (m_Pipeline)
	{
		m_Pipeline->UnBind();
	}
}

