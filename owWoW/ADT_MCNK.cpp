#include "stdafx.h"

// Include
#include "ADT.h"

// General
#include "ADT_MCNK.h"

// Additional
#include "MapController.h"
#include "Map_Shared.h"

//

ADT_MCNK::ADT_MCNK(ADT* _parentTile) :
	m_ParentTile(_parentTile),
	m_BlendRBGShadowATexture(0),
	m_Indexes(nullptr),
	m_IndexesCount(0),
	m_LiquidInstance(nullptr)
{
	memset(mcly, 0x00, 16 * 4);

	m_BlendRBGShadowATexture = 0;
}

ADT_MCNK::~ADT_MCNK()
{
	_Bindings->UnregisterRenderable3DObject(this);
}

//

void ADT_MCNK::Load(IFile* f)
{
	uint32_t startPos = f->GetPos();

	// Read header
	f->ReadBytes(&header, sizeof(ADT_MCNK_Header));

	// Scene node params
	{
		// Set translate
		m_Translate.x = header.xpos * (-1.0f) + C_ZeroPoint;
		m_Translate.y = header.ypos;
		m_Translate.z = header.zpos * (-1.0f) + C_ZeroPoint;

		// Bounds
		m_Bounds.Min = vec3(m_Translate.x, Math::MaxFloat, m_Translate.z);
		m_Bounds.Max = vec3(m_Translate.x + C_ChunkSize, Math::MinFloat, m_Translate.z + C_ChunkSize);
		m_Bounds.calculateInternal();

		// DO NOT CALCULATE MATRIX!!!!
	}

	vec3 tempVertexes[C_MapBufferSize];
	vec3 tempNormals[C_MapBufferSize];

	uint8* blendbuf = new uint8[64 * 64 * 4];
	memset(blendbuf, 0, 64 * 64 * 4);


	// Normals
	f->Seek(startPos + header.ofsNormal);
	{
		// Normal vectors for each corresponding vector above. Its followed by some weird unknown data which is not included in the chunk itself and might be some edge flag bitmaps.
		vec3* ttn = tempNormals;
		for (int j = 0; j < 17; j++)
		{
			for (uint32 i = 0; i < ((j % 2) ? 8 : 9); i++)
			{
				int8 nor[3];
				f->ReadBytes(nor, 3);

				*ttn++ = vec3(-(float)nor[1] / 127.0f, (float)nor[2] / 127.0f, -(float)nor[0] / 127.0f);
			}
		}
	}

	// Heights
	f->Seek(startPos + header.ofsHeight);
	{
		vec3* ttv = tempVertexes;
		

		// vertices
		for (uint32 j = 0; j < 17; j++)
		{
			for (uint32 i = 0; i < ((j % 2) ? 8 : 9); i++)
			{
				float h;
				f->ReadBytes(&h, sizeof(float));

				float xpos = i * C_UnitSize;
				float zpos = j * 0.5f * C_UnitSize;
				if (j % 2)
				{
					xpos += C_UnitSize * 0.5f;
				}

				vec3 v = m_Translate + vec3(xpos, h, zpos);
				*ttv++ = v;

				m_Bounds.Min.y = minf(v.y, m_Bounds.Min.y);
				m_Bounds.Max.y = maxf(v.y, m_Bounds.Max.y);
			}
		}
	}

	// Textures
	f->Seek(startPos + header.ofsLayer);
	{
		for (uint32 i = 0; i < header.nLayers; i++)
		{
			f->ReadBytes(&mcly[i], sizeof(ADT_MCNK_MCLY));

			m_DiffuseTextures[i] = m_ParentTile->m_Textures[mcly[i].textureIndex].diffuseTexture;
			m_SpecularTextures[i] = m_ParentTile->m_Textures[mcly[i].textureIndex].specularTexture;
		}
	}

	// Shadows
	f->Seek(startPos + header.ofsShadow);
	{
		uint8 sbuf[64 * 64];
		uint8* p;
		uint8 c[8];
		p = sbuf;
		for (int j = 0; j < 64; j++)
		{
			f->ReadBytes(c, 8);
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
	f->Seek(startPos + header.ofsAlpha);
	{
		for (uint32 i = 1; i < header.nLayers; i++)
		{
			uint8 amap[64 * 64];
			memset(amap, 0x00, 64 * 64);
			const uint8* abuf = f->GetDataFromCurrent() + mcly[i].offsetInMCAL;
			uint8* p;
			p = amap;
			for (uint8 j = 0; j < 64; j++) 
			{
				for (uint8 i = 0; i < 32; i++)
				{
					uint8 c = *abuf++;
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

	// Liquids
	f->Seek(startPos + header.ofsLiquid);
	{
		if (header.sizeLiquid > 8)
		{
			CRange height;
			f->ReadBytes(&height, 8);

			Liquid* m_Liquid = new Liquid(8, 8);
			m_Liquid->CreateFromMCLQ(f, header);

			m_LiquidInstance = new Liquid_Instance(m_Liquid, vec3(m_Translate.x, 0.0f, m_Translate.z));
		}
	}

	////////////////////////////

	uint32 t = C_MapBufferSize * sizeof(float);

	// Vertex buffer
	R_Buffer* __vb = _Render->r.createVertexBuffer(10 * t, nullptr);
	__vb->updateBufferData(0 * t, C_MapBufferSize * sizeof(vec3), tempVertexes);
	__vb->updateBufferData(3 * t, C_MapBufferSize * sizeof(vec3), tempNormals);
	__vb->updateBufferData(6 * t, C_MapBufferSize * sizeof(vec2), Map_Shared::GetTextureCoordDetail());
	__vb->updateBufferData(8 * t, C_MapBufferSize * sizeof(vec2), Map_Shared::GetTextureCoordAlpha());


	// Index Buffer
	vector<uint16>& mapArray = Map_Shared::GenarateDefaultMapArray(header.holes);
	m_Indexes = mapArray.data();
	m_IndexesCount = mapArray.size();
	R_Buffer* __ib = _Render->r.createIndexBuffer(m_IndexesCount * sizeof(uint16), m_Indexes);

	// Geom
	__geom = _Render->r.beginCreatingGeometry(_Render->Storage()->__layout_GxVBF_PNT2);
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 0 * t, 0);
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 3 * t, 0);
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 6 * t, 0);
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 8 * t, 0);
	__geom->setGeomIndexParams(__ib, R_IndexFormat::IDXFMT_16);
	__geom->finishCreatingGeometry();


	//***************** DEBUG NORMALS

	// Vertex buffer
	R_Buffer* __vb2 = _Render->r.createVertexBuffer(6 * t, nullptr);
	__vb2->updateBufferData(0 * t, C_MapBufferSize * sizeof(vec3), tempVertexes);
	__vb2->updateBufferData(3 * t, C_MapBufferSize * sizeof(vec3), tempNormals);

	__geomDebugNormals = _Render->r.beginCreatingGeometry(_Render->Storage()->__layout_GxVBF_PN);
	__geomDebugNormals->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 0 * t, 0);
	__geomDebugNormals->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 3 * t, 0);
	__geomDebugNormals->setGeomIndexParams(__ib, R_IndexFormat::IDXFMT_16);
	__geomDebugNormals->finishCreatingGeometry();

	//--

	m_BlendRBGShadowATexture = _Render->r.createTexture(R_TextureTypes::Tex2D, 64, 64, 1, R_TextureFormats::RGBA8, false, false, false, false);
	m_BlendRBGShadowATexture->uploadTextureData(0, 0, blendbuf);

	_Bindings->RegisterRenderable3DObject(this, 20);
}

void ADT_MCNK::PreRender3D(double t, double dt)
{
	m_IsVisible = !_CameraFrustum->_frustum.cullBox(m_Bounds) && _Config.draw_map_chunk;
	
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
}

void ADT_MCNK::Render3D()
{
	PERF_START(PERF_MAP);
	//------------------

	_Pipeline->Clear();

	//_Render->r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);
	_Render->TechniquesMgr()->m_MapChunk_GeometryPass->Bind();
	_Render->TechniquesMgr()->m_MapChunk_GeometryPass->SetPV();
	_Render->TechniquesMgr()->m_MapChunk_GeometryPass->SetLayersCount(header.nLayers);

	// Bind m_DiffuseTextures
	for (uint32 i = 0; i < header.nLayers; i++)
	{
		_Render->r.setTexture(i, m_DiffuseTextures[i], _Config.Quality.Texture_Sampler | SS_ADDR_WRAP, 0);
		_Render->r.setTexture(5 + i, m_SpecularTextures[i], _Config.Quality.Texture_Sampler | SS_ADDR_WRAP, 0);
	}

	// Bind blend
	if (header.nLayers > 0)
		_Render->r.setTexture(4, m_BlendRBGShadowATexture, SS_ADDR_CLAMP, 0);

	// Bind shadow
	_Render->TechniquesMgr()->m_MapChunk_GeometryPass->SetShadowMapExists(header.flags.has_mcsh);
	if (header.flags.has_mcsh)
		_Render->TechniquesMgr()->m_MapChunk_GeometryPass->SetShadowColor(vec3(0.0f, 0.0f, 0.0f) * 0.3f);

	_Render->r.setGeometry(__geom);
	_Render->r.drawIndexed(PRIM_TRILIST, 0, m_IndexesCount, 0, C_MapBufferSize);

	_Render->TechniquesMgr()->m_MapChunk_GeometryPass->Unbind();

	//_Render->r.setFillMode(R_FillMode::RS_FILL_SOLID);

	//---------------
	PERF_STOP(PERF_MAP);
}

void ADT_MCNK::PostRender3D()
{
}

//

/*void ADT_MCNK::drawPass(int anim)
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
	}

	glDrawElements(GL_TRIANGLE_STRIP, striplen, GL_UNSIGNED_SHORT, m_Indexes);

	if (anim)
	{
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glActiveTexture(GL_TEXTURE1);
	}
}*/


/*void ADT_MCNK::Render_DEBUG()
{
	if (_CameraFrustum->_frustum.cullBox(m_Bounds))
	{
		return;
	}

	_Render->r.setGeometry(__geomDebugNormals);
	_Render->r.drawIndexed(PRIM_TRILIST, 0, m_IndexesCount, 0, C_MapBufferSize);
}*/

