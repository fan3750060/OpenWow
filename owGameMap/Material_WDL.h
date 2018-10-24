#pragma once

class Material_WDL : public MaterialDebug
{
public:
	Material_WDL(RenderDevice* _device) :
		MaterialDebug(_device)
	{}
	virtual ~Material_WDL()
	{}
};