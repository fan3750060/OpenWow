#pragma once

#include "MDX.h"

class MDXsManager : public RefManager1DimAssync<MDX>
{
public:
	MDXsManager();
	~MDXsManager();

	// RefManager override
	MDX* CreateAction(cstring name) override;
	void LoadAction(string name, MDX*& _model) override;
	bool DeleteAction(cstring name) override;

	void resetAnim();
	void updateEmitters(float dt);
};