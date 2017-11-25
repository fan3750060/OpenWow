#include "stdafx.h"

// General
#include "Renderable3DObject.h"

// Additional
#include "Renderable3DObjectCollection.h"

bool Renderable3DObject::Register(uint32 _order)
{
	Renderable3DObjectCollection::RegisterObject(this);
	SetDrawOrder(_order);

	return true;
}

void Renderable3DObject::Unregister()
{
	Renderable3DObjectCollection::UnregisterObject(this);
}
