#pragma once

class WDL_Node_Material : public MaterialDebug
{
public:
	WDL_Node_Material(RenderDevice* _device) :
		MaterialDebug(_device)
	{}
	virtual ~WDL_Node_Material()
	{}
};