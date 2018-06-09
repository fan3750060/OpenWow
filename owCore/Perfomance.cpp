#include "stdafx.h"

// General
#include "Perfomance.h"

// Additional
#include "BaseManager.h"

Perfomance::Perfomance()
{
	FrameBegin();
	m_Engine = GetManager<IEngine>();
}