#include "stdafx.h"

// General
#include "MDXsManager.h"

// Additional
#include "MDX.h"

MDXsManager::MDXsManager()
{
	ADDCONSOLECOMMAND_CLASS("models_info", MDXsManager, PrintAllInfo);

	AddManager<IMDXManager>(this);
	_Bindings->RegisterUpdatableObject(this);
}

MDXsManager::~MDXsManager()
{
	_Bindings->UnregisterUpdatableObject(this);
}

void MDXsManager::Update(double _Time, double _deltaTime)
{
	while (_deltaTime > 0.1f)
	{
		updateEmitters(0.1f);
		_deltaTime -= 0.1f;
	}

	updateEmitters(_deltaTime);
}

//

MDX* MDXsManager::CreateAction(cstring name)
{
	MDX* model = new MDX(name);
	model->Init();
	return model;
}
bool MDXsManager::DeleteAction(cstring name)
{
	return true;
}

void MDXsManager::resetAnim()
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		if (it->second != nullptr) // FIXME: Incorrect names in objects
		{
			(it->second)->animcalc = false;
		}
	}
}

void MDXsManager::updateEmitters(float dt)
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		if (it->second != nullptr)  // FIXME: Incorrect names in objects
		{
			(it->second)->updateEmitters(dt);
		}
	}
}