#include "stdafx.h"

// Includes
#include "Wmo_Material.h"

// General
#include "liquid.h"

// Additional
#include "WorldController.h"

struct Liquid_VertexData
{
	vec3 position;
	vec3 textureCoord;
	vec3 normal;
};

//

Liquid::Liquid(uint32 x, uint32 y, vec3 base) : 
    m_TilesX(x),
    m_TilesY(y), 
    m_Position(base),
	ydir(1.0f)
{
	m_TilesCount = (m_TilesX + 1) * (m_TilesY + 1);
}

Liquid::~Liquid()
{
	for (uint32_t i = 0; i < textures.size(); i++)
	{
		_Render->TexturesMgr()->Delete(textures[i]);
	}

	_Bindings->UnregisterRenderable3DObject(this);
}

//

void Liquid::CreateFromMCLQ(File & f, ADT_MCNK_Header header)
{
	ydir = 1.0f;

	initGeometry(f);

	if (header.flags.lq_river)
	{
		InitTextures(DBC_LIQUIDTYPE_Type::lq_river);
	}
	else if (header.flags.lq_ocean)
	{
		InitTextures(DBC_LIQUIDTYPE_Type::lq_ocean);
	}
	else if(header.flags.lq_magma)
	{
		InitTextures(DBC_LIQUIDTYPE_Type::lq_magma);
	}
	else if(header.flags.lq_slime)
	{
		InitTextures(DBC_LIQUIDTYPE_Type::lq_slime);
	}
	
}

void Liquid::CreateFromWMO(File& f, WMOMaterial* _material, const DBC_LiquidTypeRecord* _liquidType, bool _indoor)
{
	ydir = -1.0f; // Magic for WMO

	initGeometry(f);
	InitTextures((DBC_LIQUIDTYPE_Type)_liquidType->Get_Type());

	if (_indoor)
	{
		m_WaterColorLight = fromRGB(_material->GetDiffuseColor());
		m_WaterColorDark = m_WaterColorLight;
	}
	else
	{
		m_WaterColorLight = _World->EnvM()->skies->GetColor(LIGHT_COLOR_RIVER_LIGHT);
		m_WaterColorDark = _World->EnvM()->skies->GetColor(LIGHT_COLOR_RIVER_DARK);
	}
}

void Liquid::PreRender3D(double t, double dt)
{
	m_IsVisible = true;
}

void Liquid::Render3D()
{
	if (globalBufferSize == 0)
	{
		return;
	}

	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	_Render->TechniquesMgr()->m_Water->Bind();
	_Render->TechniquesMgr()->m_Water->SetPV();

	uint32_t texidx = (uint32_t)(_World->EnvM()->animtime / 60.0f) % textures.size();
	_Render->r.setTexture(10, textures[texidx], 0, 0);

	/*_Render->TechniquesMgr()->m_Water->SetWaterColorLight(_EnvironmentManager->GetSkyColor(LIGHT_COLOR_OCEAN_LIGHT));
	_Render->TechniquesMgr()->m_Water->SetWaterColorDark(_EnvironmentManager->GetSkyColor(LIGHT_COLOR_OCEAN_DARK));
	_Render->TechniquesMgr()->m_Water->SetShallowAlpha(_EnvironmentManager->skies->oceanShallowAlpha);
	_Render->TechniquesMgr()->m_Water->SetDeepAlpha(_EnvironmentManager->skies->oceanDeepAlpha);*/

	_Render->TechniquesMgr()->m_Water->SetWaterColorLight(_World->EnvM()->skies->GetColor(LIGHT_COLOR_RIVER_LIGHT));
	_Render->TechniquesMgr()->m_Water->SetWaterColorDark(_World->EnvM()->skies->GetColor(LIGHT_COLOR_RIVER_DARK));
	_Render->TechniquesMgr()->m_Water->SetShallowAlpha(_World->EnvM()->skies->GetWaterShallowAlpha());
	_Render->TechniquesMgr()->m_Water->SetDeepAlpha(_World->EnvM()->skies->GetWaterDarkAlpha());

	_Render->r.setGeometry(__geom);

	_Render->r.draw(PRIM_TRILIST, 0, globalBufferSize);
	PERF_INC(PERF_MAP_CHUNK_MH20);

	_Render->TechniquesMgr()->m_Water->Unbind();
}

void Liquid::PostRender3D()
{
}

//



#pragma region Types

#include "../shared/pack_begin.h"

struct Liquid_Vertex
{
	union
	{
		struct SWVert
		{
			char depth;
			char flow0Pct;
			char flow1Pct;
			char filler;
			float height;
		} waterVert;

		struct SOVert
		{
			char depth;
			char foam;
			char wet;
			char filler;
		} oceanVert;

		struct SMOMagmaVert
		{
			uint16 s;
			uint16 t;
			float height;
		} magmaVert;
	};
};

struct Liquid_Flag
{
	uint8 liquid : 6;    // 0x01 - 0x20
	uint8 fishable : 1;  // 0x40
	uint8 shared : 1;    // 0x80
};

#include "../shared/pack_end.h"

#pragma endregion

void Liquid::initGeometry(File& f)
{
	Liquid_Vertex* map = (Liquid_Vertex*)f.GetDataFromCurrent();
	Liquid_Flag* flags = (Liquid_Flag*)(f.GetDataFromCurrent() + m_TilesCount * sizeof(Liquid_Vertex));

	Liquid_Layer waterLayer;

	waterLayer.x = 0;
	waterLayer.y = 0;
	waterLayer.Width = m_TilesX;
	waterLayer.Height = m_TilesY;

	waterLayer.LiquidType = 0;
	waterLayer.MinHeightLevel = 0;
	waterLayer.MaxHeightLevel = 0;
	waterLayer.LiquidObjectOrLVF = 0; // FIXME Send format to create buffer

	for (uint32 j = 0; j < m_TilesY + 1; j++)
	{
		for (uint32 i = 0; i < m_TilesX + 1; i++)
		{
			uint32 p = j * (m_TilesX + 1) + i;

			if (flags[p].liquid & 0x08)
			{
				waterLayer.renderTiles.push_back(false);
			}
			else
			{
				waterLayer.renderTiles.push_back(true);
			}

			waterLayer.heights.push_back(map[p].magmaVert.height);
			//waterLayer.depths.push_back((map[p].magmaVert.s / 255.0f) * 0.5f + 0.5f);
		}
	}

	m_WaterLayers.push_back(waterLayer);

	createBuffer(m_Position);
}

void Liquid::createBuffer(cvec3 _position)
{
	vector<Liquid_VertexData> mh2oVertices;

	for (unsigned l = 0; l < m_WaterLayers.size(); l++)
	{
		Liquid_Layer& layer = m_WaterLayers[l];

		for (uint8 y = layer.y; y < layer.Height + layer.y; y++)
		{
			for (uint8 x = layer.x; x < layer.Width + layer.x; x++)
			{

				unsigned tx = x - layer.x;
				unsigned ty = y - layer.y;

				// p1--p4
				// |    |
				// p2--p3
				unsigned p1 = tx + ty * (layer.Width + 1);
				unsigned p2 = tx + (ty + 1)     * (layer.Width + 1);
				unsigned p3 = tx + 1 + (ty + 1) * (layer.Width + 1);
				unsigned p4 = tx + 1 + ty * (layer.Width + 1);

				// heights
				float h1, h2, h3, h4;
				h1 = h2 = h3 = h4 = 0.0f;
				if (layer.heights.size() > 0)
				{
					h1 = layer.heights[p1];
					h2 = layer.heights[p2];
					h3 = layer.heights[p3];
					h4 = layer.heights[p4];
				}

				// R_Texture coords
				std::pair<float, float> t1, t2, t3, t4;
				t1 = make_pair(0.0f, 0.0f);
				t2 = make_pair(0.0f, 1.0f);
				t3 = make_pair(1.0f, 1.0f);
				t4 = make_pair(1.0f, 0.0f);
				if (layer.textureCoords.size() > 0)
				{
					t1 = layer.textureCoords[p1];
					t2 = layer.textureCoords[p2];
					t3 = layer.textureCoords[p3];
					t4 = layer.textureCoords[p4];
				}

				// alpha
				float a1, a2, a3, a4;
				a1 = a2 = a3 = a4 = 1.0f;
				if (layer.depths.size() > 0)
				{
					a1 = minf(static_cast<float>(layer.depths[p1]) / 255.0f, 1.0f); // whats the magic formular here ???
					a2 = minf(static_cast<float>(layer.depths[p2]) / 255.0f, 1.0f);
					a3 = minf(static_cast<float>(layer.depths[p3]) / 255.0f, 1.0f);
					a4 = minf(static_cast<float>(layer.depths[p4]) / 255.0f, 1.0f);
				}

				// Skip hidden water tile
				if (layer.renderTiles.size() != 0)
				{
					if (!layer.renderTiles[tx + ty * layer.Width])
					{
						continue;
					}
				}

				// Insert vertex

				mh2oVertices.push_back
				({
					_position + vec3(C_UnitSize * static_cast<float>(x), h1, ydir * (C_UnitSize * static_cast<float>(y))),
					vec3(t1.first, t1.second, a1),
					defaultNormal
					});

				mh2oVertices.push_back
				({
					_position + vec3(C_UnitSize * static_cast<float>(x), h2, ydir * (C_UnitSize + C_UnitSize * static_cast<float>(y))),
					vec3(t2.first, t2.second, a2),
					defaultNormal
					});

				mh2oVertices.push_back
				({
					_position + vec3(C_UnitSize + C_UnitSize * static_cast<float>(x), h4, ydir * (C_UnitSize * static_cast<float>(y))),
					vec3(t4.first, t4.second, a4),
					defaultNormal
					});

				//

				mh2oVertices.push_back
				({
					_position + vec3(C_UnitSize + C_UnitSize * static_cast<float>(x), h4, ydir * (C_UnitSize * static_cast<float>(y))),
					vec3(t4.first, t4.second, a4),
					defaultNormal
					});

				mh2oVertices.push_back
				({
					_position + vec3(C_UnitSize * static_cast<float>(x), h2, ydir * (C_UnitSize + C_UnitSize * static_cast<float>(y))),
					vec3(t2.first, t2.second, a2),
					defaultNormal
					});

				mh2oVertices.push_back
				({
					_position + vec3(C_UnitSize + C_UnitSize * static_cast<float>(x), h3, ydir * (C_UnitSize + C_UnitSize * static_cast<float>(y))),
					vec3(t3.first, t3.second, a3),
					defaultNormal
					});
			}
		}
	}

	globalBufferSize = static_cast<uint32>(mh2oVertices.size());

	if (globalBufferSize == 0)
	{
		return;
	}


	// Vertex buffer
	R_Buffer* __vb = _Render->r.createVertexBuffer(mh2oVertices.size() * sizeof(Liquid_VertexData), mh2oVertices.data());

	// Index bufer
	//uint32 __ib = _Render->r.createIndexBuffer(m_IndicesCount, m_Indices);

	// Geometry
	__geom = _Render->r.beginCreatingGeometry(_Render->Storage()->__layoutWater);
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 0, sizeof(Liquid_VertexData));
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 12, sizeof(Liquid_VertexData));
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 24, sizeof(Liquid_VertexData));
	//_Render->r.setGeomIndexParams(__geom, __ib, R_IndexFormat::IDXFMT_16);
	__geom->finishCreatingGeometry();

	_Bindings->RegisterRenderable3DObject(this, 20);
}

void Liquid::InitTextures(DBC_LIQUIDTYPE_Type _liquidType)
{
	string baseName;

	if (_liquidType == DBC_LIQUIDTYPE_Type::lq_river)
	{
		baseName = "XTextures\\river\\lake_a";
	}
	else if (_liquidType == DBC_LIQUIDTYPE_Type::lq_ocean)
	{
		baseName = "XTextures\\ocean\\ocean";
	}
	else if (_liquidType == DBC_LIQUIDTYPE_Type::lq_magma)
	{
		baseName = "XTextures\\lava\\lava";
	}
	else if (_liquidType == DBC_LIQUIDTYPE_Type::lq_slime)
	{
		baseName = "XTextures\\slime\\slime";
	}
	else
	{
		fail1();
	}
	
	char buf[256];
	for (int i = 1; i <= 30; i++) {
		sprintf(buf, "%s.%d.blp", baseName.c_str(), i);
		textures.push_back(_Render->TexturesMgr()->Add(buf));
	}
}
