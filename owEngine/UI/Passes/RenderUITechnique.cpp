#include <stdafx.h>

// General
#include "RenderUITechnique.h"

RenderUITechnique::RenderUITechnique()
{}

RenderUITechnique::~RenderUITechnique()
{}

void RenderUITechnique::SetPass(std::shared_ptr<IRenderUIPass> pass)
{
	m_Pass = pass;
}

// Render the scene using the passes that have been configured.
void RenderUITechnique::Render(RenderUIEventArgs& renderEventArgs)
{
	if (m_Pass != nullptr)
	{
		if (m_Pass->IsEnabled())
		{
			m_Pass->RenderUI(renderEventArgs);
		}
	}
}


