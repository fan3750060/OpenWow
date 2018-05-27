#pragma once

#include "MDX.h"

class MDXsManager : public IMDXManager, public RefManager1Dim<MDX>, public IUpdatable
{
public:
	MDXsManager();
	~MDXsManager();

	void resetAnim();
	void updateEmitters(float dt);

	// IMDXManager
	MDX* Add(cstring name) { return RefManager1Dim::Add(name); }
	bool Exists(cstring name) const { return RefManager1Dim::Exists(name); }
	void Delete(cstring name) { RefManager1Dim::Delete(name); }
	void Delete(MDX* item) { RefManager1Dim::Delete(item); }

	// RefManager1Dim
	MDX* CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;

	// IUpdatable
	void Input(double _time, double _dTime) override {}
	void Update(double _Time, double _deltaTime);
};
