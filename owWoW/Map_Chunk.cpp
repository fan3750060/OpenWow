#include "stdafx.h"

// Include
#include "Map_Tile.h"

// General
#include "Map_Chunk.h"

// Additional
#include "Map.h"
#include "Map_Shared.h"

//

MapChunk::MapChunk(MapTile* _parentTile) :
	m_ParentTile(_parentTile),
	m_GamePositionX(0),
	m_GamePositionY(0),
	m_GamePositionZ(0),
	areaID(-1),
	visible(false),
	hasholes(false),
	m_BlendRBGShadowATexture(0),
	m_Indexes(nullptr),
	m_IndexesCount(0)
{
	memset(mcly, 0x00, 16 * 4);
	waterlevel[0] = 0;
	waterlevel[1] = 0;

	m_BlendRBGShadowATexture = 0;


	colorBufferEnable = false;

}

MapChunk::~MapChunk()
{
	/*if (m_Liquid != nullptr)
	{
		delete m_Liquid;
	}*/
}

//

void MapChunk::Load(File& f)
{
	f.SeekRelative(8); // Skip CHUNK + SIZE

	uint32_t startPos = f.GetPos();

	// Read header
	f.ReadBytes(&header, 0x80);

	areaID = header.areaid;

	m_GamePositionX = header.xpos;
	m_GamePositionY = header.ypos;
	m_GamePositionZ = header.zpos;
	m_GamePositionX = m_GamePositionX * (-1.0f) + C_ZeroPoint;
	m_GamePositionZ = m_GamePositionZ * (-1.0f) + C_ZeroPoint;

	hasholes = (header.holes != 0);

	vec3 tempVertexes[C_MapBufferSize];
	vec3 tempNormals[C_MapBufferSize];

	uint8* blendbuf = new uint8[64 * 64 * 4];
	memset(blendbuf, 0, 64 * 64 * 4);

	//--

	// Normals
	f.Seek(startPos + header.ofsNormal);
	{
		// Normal vectors for each corresponding vector above. Its followed by some weird unknown data which is not included in the chunk itself and might be some edge flag bitmaps.
		vec3* ttn = tempNormals;
		for (int j = 0; j < 17; j++)
		{
			for (uint32 i = 0; i < ((j % 2) ? 8 : 9); i++)
			{
				int8 nor[3];
				f.ReadBytes(nor, 3);

				*ttn++ = vec3(-(float)nor[1] / 127.0f, (float)nor[2] / 127.0f, -(float)nor[0] / 127.0f);
			}
		}
	}

	// Heights
	f.Seek(startPos + header.ofsHeight);
	{
		vec3* ttv = tempVertexes;
		vec3 vmin = vec3(m_GamePositionX, Math::MaxFloat, m_GamePositionZ);
		vec3 vmax = vec3(m_GamePositionX + C_ChunkSize, Math::MinFloat, m_GamePositionZ + C_ChunkSize);

		// vertices
		for (uint32 j = 0; j < 17; j++)
		{
			for (uint32 i = 0; i < ((j % 2) ? 8 : 9); i++)
			{
				float h;
				f.ReadBytes(&h, sizeof(float));

				float xpos = i * C_UnitSize;
				float zpos = j * 0.5f * C_UnitSize;
				if (j % 2)
				{
					xpos += C_UnitSize * 0.5f;
				}

				vec3 v = vec3(m_GamePositionX + xpos, m_GamePositionY + h, m_GamePositionZ + zpos);
				*ttv++ = v;

				vmin.y = minf(v.y, vmin.y);
				vmax.y = maxf(v.y, vmax.y);
			}
		}
		m_Bounds.set(vmin, vmax, false);
	}

	// Textures
	f.Seek(startPos + header.ofsLayer);
	{
		for (uint32 i = 0; i < header.nLayers; i++)
		{
			f.ReadBytes(&mcly[i], 16);

			if (mcly[i].flags.animation_enabled)
			{
				//animated[i] = mcly[i].flags;
				Log::Error("ANIMATED!!!");
			}
			else
			{
				animated[i] = 0;
			}
		}
	}

	// Shadows
	f.Seek(startPos + header.ofsShadow);
	{
		uint8 sbuf[64 * 64], *p, c[8];
		p = sbuf;
		for (int j = 0; j < 64; j++)
		{
			f.ReadBytes(c, 8);
			for (int i = 0; i < 8; i++)
			{
				for (int b = 0x01; b != 0x100; b <<= 1)
				{
					*p++ = (c[i] & b) ? 85 : 0;
				}
			}
		}

		for (int p = 0; p < 64 * 64; p++)
		{
			blendbuf[p * 4 + 3] = sbuf[p];
		}
	}

	// Alpha
	f.Seek(startPos + header.ofsAlpha);
	{
		if (header.nLayers > 0)
		{
			for (uint32 i = 1; i < header.nLayers; i++)
			{
				uint8 amap[64 * 64];
				memset(amap, 0x00, 64 * 64);
				uint8* abuf = f.GetDataFromCurrent() + mcly[i].offsetInMCAL, *p;
				p = amap;
				for (int j = 0; j < 64; j++) {
					for (int i = 0; i < 32; i++) {
						unsigned char c = *abuf++;
						*p++ = (c & 0x0f) << 4;
						*p++ = (c & 0xf0);
					}

				}

				if (!header.flags.do_not_fix_alpha_map)
				{
					for (uint8 i = 0; i < 64; ++i)
					{
						amap[i * 64 + 63] = amap[i * 64 + 62];
						amap[63 * 64 + i] = amap[62 * 64 + i];
					}
					amap[63 * 64 + 63] = amap[62 * 64 + 62];
				}

				for (int p = 0; p < 64 * 64; p++)
				{
					blendbuf[p * 4 + (i - 1)] = amap[p];
				}

			}
		}
	}

	// Liquids
	f.Seek(startPos + header.ofsLiquid);
	{
		if (header.sizeLiquid > 8)
		{
			CRange height;
			f.ReadBytes(&height, 8);

			Liquid* liquid = new Liquid(8, 8, vec3(m_GamePositionX, 0, m_GamePositionZ));
			liquid->CreateFromMCLQ(f, header);
			m_ParentTile->m_MH2O.push_back(liquid);
		}
	}

	////////////////////////////

	uint32 t = C_MapBufferSize * sizeof(float);

	// Vertex buffer
	R_Buffer* __vb = _Render->r->createVertexBuffer(10 * t, nullptr);

	_Render->r->updateBufferData(__vb, 0 * t, C_MapBufferSize * sizeof(vec3), tempVertexes);
	_Render->r->updateBufferData(__vb, 3 * t, C_MapBufferSize * sizeof(vec3), tempNormals);
	_Render->r->updateBufferData(__vb, 6 * t, C_MapBufferSize * sizeof(vec2), Map_Shared::GetTextureCoordDetail());
	_Render->r->updateBufferData(__vb, 8 * t, C_MapBufferSize * sizeof(vec2), Map_Shared::GetTextureCoordAlpha());


	//

	__geom = _Render->r->beginCreatingGeometry(_RenderStorage->__layout_GxVBF_PNT2);

	_Render->r->setGeomVertexParams(__geom, __vb, R_DataType::T_FLOAT, 0 * t, 0);
	_Render->r->setGeomVertexParams(__geom, __vb, R_DataType::T_FLOAT, 3 * t, 0);
	_Render->r->setGeomVertexParams(__geom, __vb, R_DataType::T_FLOAT, 6 * t, 0);
	_Render->r->setGeomVertexParams(__geom, __vb, R_DataType::T_FLOAT, 8 * t, 0);

	vector<uint16>& mapArray = Map_Shared::GenarateDefaultMapArray(header.holes);
	m_Indexes = mapArray.data();
	m_IndexesCount = mapArray.size();

	R_Buffer* __ib = _Render->r->createIndexBuffer(m_IndexesCount * sizeof(uint16), m_Indexes);
	_Render->r->setGeomIndexParams(__geom, __ib, R_IndexFormat::IDXFMT_16);

	_Render->r->finishCreatingGeometry(__geom);


	//***************** DEBUG NORMALS

	// Vertex buffer
	R_Buffer* __vb2 = _Render->r->createVertexBuffer(6 * t, nullptr);
	_Render->r->updateBufferData(__vb2, 0 * t, C_MapBufferSize * sizeof(vec3), tempVertexes);
	_Render->r->updateBufferData(__vb2, 3 * t, C_MapBufferSize * sizeof(vec3), tempNormals);

	__geomDebugNormals = _Render->r->beginCreatingGeometry(_RenderStorage->__layout_GxVBF_PN);
	_Render->r->setGeomVertexParams(__geomDebugNormals, __vb, R_DataType::T_FLOAT, 0 * t, 0);
	_Render->r->setGeomVertexParams(__geomDebugNormals, __vb, R_DataType::T_FLOAT, 3 * t, 0);
	_Render->r->setGeomIndexParams(__geomDebugNormals, __ib, R_IndexFormat::IDXFMT_16);
	_Render->r->finishCreatingGeometry(__geomDebugNormals);

	//--

	m_BlendRBGShadowATexture = _Render->r->createTexture(R_TextureTypes::Tex2D, 64, 64, 1, R_TextureFormats::RGBA8, false, false, false, false);
	_Render->r->uploadTextureData(m_BlendRBGShadowATexture, 0, 0, blendbuf);
}

void MapChunk::Post_Load()
{
	// R_Texture layer definitions for this map chunk. 16 bytes per layer, up to 4 layers (thus, layer count = size / 16).
	for (uint32 i = 0; i < header.nLayers; i++)
	{
		m_DiffuseTextures[i] = m_ParentTile->m_Textures[mcly[i].textureIndex].diffuseTexture;
		m_SpecularTextures[i] = m_ParentTile->m_Textures[mcly[i].textureIndex].specularTexture;
	}
}

//

/*void MapChunk::drawPass(int anim)
{
	if (anim)
	{
		glActiveTexture(GL_TEXTURE0);
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();

		// note: this is ad hoc and probably completely wrong
		int spd = (anim & 0x08) | ((anim & 0x10) >> 2) | ((anim & 0x20) >> 4) | ((anim & 0x40) >> 6);
		int dir = anim & 0x07;
		const float texanimxtab[8] = {0, 1, 1, 1, 0, -1, -1, -1};
		const float texanimytab[8] = {1, 1, 0, -1, -1, -1, 0, 1};
		float fdx = -texanimxtab[dir], fdy = texanimytab[dir];

		int animspd = (int)(200.0f * C_DetailSize);
		float f = (((int)(_TimeManager->animtime * (spd / 15.0f))) % animspd) / (float)animspd;
		glTranslatef(f * fdx, f * fdy, 0);
	}*/

	//glDrawElements(GL_TRIANGLE_STRIP, striplen, GL_UNSIGNED_SHORT, m_Indexes);

	/*if (anim)
	{
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glActiveTexture(GL_TEXTURE1);
	}
}*/


void MapChunk::Render()
{
	visible = false;

	if (_CameraFrustum->_frustum.cullBox(m_Bounds))
	{
		return;
	}

	// Draw chunk before fog
	/*float mydist = (_Camera->Position - vcenter).length() - r;
	if (mydist > _Config.culldistance)
	{
		if (_Config.uselowlod)
		{
			this->drawNoDetail();
			return;
		}
	}*/

	visible = true;

	_TechniquesMgr->m_MapChunk_GeometryPass->SetLayersCount(header.nLayers);


	_Render->r->setGeometry(__geom);

	//_Render->r->setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	// Bind m_DiffuseTextures
	for (uint32 i = 0; i < header.nLayers; i++)
	{
		_Render->r->setTexture(i, m_DiffuseTextures[i], _Config.Quality.Texture_Sampler | SS_ADDR_WRAP, 0);
		_Render->r->setTexture(5 + i, m_SpecularTextures[i], _Config.Quality.Texture_Sampler | SS_ADDR_WRAP, 0);
	}

	// Bind blend
	if (header.nLayers > 0)
	{
		_Render->r->setTexture(4, m_BlendRBGShadowATexture, SS_ADDR_CLAMP, 0);
	}

	// Bind shadow
	_TechniquesMgr->m_MapChunk_GeometryPass->SetShadowMapExists(header.flags.has_mcsh);
	if (header.flags.has_mcsh)
	{
		_TechniquesMgr->m_MapChunk_GeometryPass->SetShadowColor(vec3(0.0f, 0.0f, 0.0f) * 0.3f);
	}

	_Render->r->drawIndexed(PRIM_TRILIST, 0, m_IndexesCount, 0, C_MapBufferSize);

	//_Render->r->setFillMode(R_FillMode::RS_FILL_SOLID);
}

void MapChunk::Render_DEBUG()
{
	if (_CameraFrustum->_frustum.cullBox(m_Bounds))
	{
		return;
	}

	_Render->r->setGeometry(__geomDebugNormals);
	_Render->r->drawIndexed(PRIM_TRILIST, 0, m_IndexesCount, 0, C_MapBufferSize);
}

