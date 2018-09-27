#include "stdafx.h"

// General
#include "RenderQueue.h"

// Additional
#include "Render.h"

RenderQueue::RenderQueue(RenderDevice * _RenderDevice) :
	m_RD(_RenderDevice)
{
}

void RenderQueue::PushCall(RenderDrawCall & _call)
{
	m_DrawCalls.push_back(_call);
}

void RenderQueue::ProcessDrawCalls()
{
	for (auto& call : m_DrawCalls)
	{
		_ASSERT(call.renderState != nullptr);
		_ASSERT(call.technique != nullptr);
		_ASSERT(call.bindUniformsFunc != nullptr);

		call.technique->Bind();
		call.bindUniformsFunc(call.technique);
		
		if (call.firstIndex == 0 && call.numIndices == 0)
		{
			m_RD->draw(call.firstVert, call.numVerts, call.renderState);
		}
		else
		{
			m_RD->drawIndexed(call.firstIndex, call.numIndices, call.firstVert, call.numVerts, call.renderState, false);
		}

		call.technique->Unbind();
	}

	m_DrawCalls.clear();
}
