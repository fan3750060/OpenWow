#include "stdafx.h"

// General
#include "Item_M2Instance.h"

CItem_M2Instance::CItem_M2Instance(std::string _model) :
	CM2_Base_Instance(_model)
{
}

CItem_M2Instance::~CItem_M2Instance()
{
}

void CItem_M2Instance::AddVisualEffect(std::shared_ptr<CM2_Base_Instance> _visualEffect)
{
	_visualEffect->SetParent(shared_from_this());
}
