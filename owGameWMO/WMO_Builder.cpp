#include "stdafx.h"

// General
#include "WMO_Builder.h"

CWMO_Builder::CWMO_Builder(WMO* _model) :
	m_WMO(_model),
	m_F(nullptr)
{
	m_F = GetManager<IFilesManager>()->Open(m_WMO->getFilename());
	if (m_F == nullptr)
	{
		Log::Info("CWMO_Builder[%s]: Unable to open file.", m_WMO->getFilename().c_str());
	}
}

CWMO_Builder::~CWMO_Builder()
{
}

void CWMO_Builder::Load()
{
}
