#pragma once

#include "WMO.h"

class CWMO_Builder
{
public:
	CWMO_Builder(WMO* _model);
	~CWMO_Builder();

	void Load();

	// Loader
	void Step1Header();
	void Step2GlobalLoops();
	void Step3Bones();

private:
	WMO*			m_WMO;
	std::shared_ptr<IFile>			m_F;
};