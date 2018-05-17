#include "stdafx.h"

// General
#include "MDXsManager.h"

// Additional
#include "MDX.h"

MDXsManager::MDXsManager()
{
	ADDCONSOLECOMMAND_CLASS("models_info", MDXsManager, PrintAllInfo);

	RefManager1DimAssync::Init();
}

MDXsManager::~MDXsManager()
{
	RefManager1DimAssync::Destroy();

	DeleteAll();

	Log::Info("MDXsManager[]: All MDXs destroyed.");
}

//

MDX* MDXsManager::CreateAction(cstring name)
{
	MDX* model = new MDX(name);
	return model;
}

void MDXsManager::LoadAction(string name, MDX*& _model)
{
	//wglMakeCurrent(_Render->dc, _Render->glrc3);

	_model->Init();
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