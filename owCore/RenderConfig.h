#pragma once

class RenderConfig
{
public:
    CLASS_INSTANCE(RenderConfig);

    RenderConfig();
	~RenderConfig();

public:
    inline void Switch(bool& var)
    {
        var = !var;
    }

	

	//

public:
	// Drawing


};

#define _Config RenderConfig::instance()