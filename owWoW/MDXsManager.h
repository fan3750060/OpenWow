#pragma once

#include "MDX.h"

class MDXsManager : public RefManager1Dim<MDX>, public UpdatableObject
{
public:
	MDXsManager();
	~MDXsManager();

	// UpdatableObject
	void Update(double _Time, double _deltaTime);

	// RefManager override
	MDX* CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;

	void resetAnim();
	void updateEmitters(float dt);
};