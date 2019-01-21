#include "stdafx.h"

// General
#include "Item_M2Instance.h"

CItem_M2Instance::CItem_M2Instance(std::shared_ptr<M2> _model) :
	CM2_Base_Instance(_model)
{ 

}

void CItem_M2Instance::AddVisualEffect(std::shared_ptr<CM2_Base_Instance> _visualEffect)
{
	_visualEffect->SetParent(shared_from_this());
}
