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

	m_DefaultModel = Add("World\\Scale\\50x50.m2");
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

std::shared_ptr<M2> CM2_Manager::CreateAction(cstring name)
{
	std::string newName = Utils::ToLower(name);
	if (newName.find("orgrimmarsmokeemitter.mdx") != -1 ||
		newName.find("orgrimmarfloatingembers.mdx") != -1)
	{
		return nullptr;
	}

	std::shared_ptr<M2> model = std::make_shared<M2>(name);

	CM2_Builder builder(model);
	if (!builder.Load())
	{
		//delete model;
		return m_DefaultModel;
	}

	return model;
}
bool CM2_Manager::DeleteAction(cstring name)
{
	return true;
}