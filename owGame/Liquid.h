#pragma once

#include "LiquidLayer.h"

class Liquid
{
public:
	Liquid(uint32 x, uint32 y);

protected:
	void createLayers(std::shared_ptr<const DBC_LiquidTypeRecord> _type, std::shared_ptr<IFile> f);
	void createBuffer();
	
public:
	uint32                                                              m_TilesX, m_TilesY;
	uint32                                                              m_TilesCount;
	BoundingBox                                                         m_Bounds;

	std::vector<std::shared_ptr<Liquid_Layer>>							m_WaterLayers;

	float                                                               ydir;

private:
	const vec3 defaultNormal = vec3(0.0f, 1.0f, 0.0f);
};
