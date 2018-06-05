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
	_Bindings->RegisterUpdatableObject(this);
}

CM2_Manager::~CM2_Manager()
{
	_Bindings->UnregisterUpdatableObject(this);
}

void CM2_Manager::Update(double _Time, double _deltaTime)
{
	ResetAnim();

	while (_deltaTime > 0.1f)
	{
		updateEmitters(0.1f);
		_deltaTime -= 0.1f;
	}

	updateEmitters(_deltaTime);
}

//

M2* CM2_Manager::CreateAction(cstring name)
{
	M2* model = new M2(name);

	CM2_Builder builder(model);
	builder.Load();

	return model;
}
bool CM2_Manager::DeleteAction(cstring name)
{
	return true;
}

void CM2_Manager::ResetAnim()
{
	for (auto it : objects)
	{
		(it.second)->animcalc = false;
	}
}

void CM2_Manager::updateEmitters(float dt)
{
	for (auto it : objects)
	{
		(it.second)->updateEmitters(dt);
	}
}