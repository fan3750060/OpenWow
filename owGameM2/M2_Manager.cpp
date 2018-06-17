#include "stdafx.h"

// General
#include "M2_Manager.h"

// Additional
#include "M2.h"
#include "M2_Builder.h"

CM2_Manager::CM2_Manager()
{
	ADDCONSOLECOMMAND_CLASS("models_info", CM2_Manager, PrintAllInfo);

	AddManager<IM2Manager>(this);
}

CM2_Manager::~CM2_Manager()
{
}

/*void CM2_Manager::Update(double _Time, double _deltaTime)
{
	ResetAnim();

	while (_deltaTime > 100.f)
	{
		updateEmitters(100.f);
		_deltaTime -= 100.f;
	}

	updateEmitters(_deltaTime);
}*/

//

M2* CM2_Manager::CreateAction(cstring name)
{
	string newName = Utils::ToLower(name);
	if (newName.find("orgrimmarsmokeemitter.mdx") != -1 ||
		newName.find("orgrimmarfloatingembers.mdx") != -1)
	{
		return nullptr;
	}

	M2* model = new M2(name);

	CM2_Builder builder(model);
	if (!builder.Load())
	{
		delete model;
		return nullptr;
	}

	return model;
}
bool CM2_Manager::DeleteAction(cstring name)
{
	return true;
}

void CM2_Manager::updateEmitters(float dt)
{
	for (auto it : objects)
	{
		(it.second)->updateEmitters(dt);
	}
}