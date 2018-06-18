#include "stdafx.h"

// Include
#include "ADT.h"

// General
#include "ADT_MCNK.h"

// Additional
#include "ADT_Liquid.h"
#include "MapController.h"
#include "Map_Shared.h"

//

ADT_MCNK::ADT_MCNK(ADT* _parentTile, IFile* _file) :
	SceneNode(_parentTile),
	m_ParentTextures(&_parentTile->m_Textures),
	m_File(_file),
	m_BlendRBGShadowATexture(0),
	m_IndexesCountHigh(0),
	m_LiquidInstance(nullptr),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>()),
	m_DistancesSettings(GetSettingsGroup<CGroupDistances>())
{
	memset(mcly, 0x00, 16 * 4);

	setDrawOrder(21);
	setDebugColor(vec4(0.0f, 0.4f, 0.0f, 0.8f));
}

//

bool ADT_MCNK::Load()
{
	SmartPtr<IFile> f = m_File;
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
		m_Bounds.setMin(vec3(m_Translate.x, Math::MaxFloat, m_Translate.z));
		m_Bounds.setMax(vec3(m_Translate.x + C_ChunkSize, Math::MinFloat, m_Translate.z + C_ChunkSize));
		m_Bounds.calculateCenter();
	}

	R_Buffer* verticesBuffer = nullptr;
	R_Buffer* normalsBuffer = nullptr;


	uint8 blendbuf[64 * 64 * 4];
	memset(blendbuf, 0, 64 * 64 * 4);

	// Normals
	f->Seek(startPos + header.ofsNormal);
	{
		vec3 tempNormals[C_MapBufferSize];
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
		normalsBuffer = _Render->r.createVertexBuffer(C_MapBufferSize * sizeof(vec3), tempNormals, false);
	}

	// Heights
	f->Seek(startPos + header.ofsHeight);
	{
		vec3 tempVertexes[C_MapBufferSize];
		vec3* ttv = tempVertexes;
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

				m_Bounds.setMinY(minf(v.y, m_Bounds.getMin().y));
				m_Bounds.setMaxY(maxf(v.y, m_Bounds.getMax().y));
			}
		}

		m_Bounds.calculateCenter();

		verticesBuffer = _Render->r.createVertexBuffer(C_MapBufferSize * sizeof(vec3), tempVertexes, false);
	}

	// Textures
	f->Seek(startPos + header.ofsLayer);
	{
		for (uint32 i = 0; i < header.nLayers; i++)
		{
			f->ReadBytes(&mcly[i], sizeof(ADT_MCNK_MCLY));

			m_DiffuseTextures[i] = m_ParentTextures->at(mcly[i].textureIndex)->diffuseTexture;
			m_SpecularTextures[i] = m_ParentTextures->at(mcly[i].textureIndex)->specularTexture;
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

			CADT_Liquid* m_Liquid = new CADT_Liquid(8, 8);
			m_Liquid->CreateFromMCLQ(f, header);

			//m_LiquidInstance = new Liquid_Instance(this, m_Liquid, vec3(m_Translate.x, 0.0f, m_Translate.z));
		}
	}

	{ // Geom High
		vector<uint16>& mapArrayHigh = Map_Shared::GenarateHighMapArray(header.holes);
		m_IndexesCountHigh = mapArrayHigh.size();
		__ibHigh = _Render->r.createIndexBuffer(mapArrayHigh.size() * sizeof(uint16), mapArrayHigh.data(), false);

		__geomHigh = _Render->r.beginCreatingGeometry(_Render->getRenderStorage()->__layout_GxVBF_PNT2);
		__geomHigh->setGeomVertexParams(verticesBuffer, R_DataType::T_FLOAT, 0, 0);
		__geomHigh->setGeomVertexParams(normalsBuffer, R_DataType::T_FLOAT, 0, 0);
		__geomHigh->setGeomVertexParams(Map_Shared::BufferTextureCoordDetail, R_DataType::T_FLOAT, 0, 0);
		__geomHigh->setGeomVertexParams(Map_Shared::BufferTextureCoordAlpha, R_DataType::T_FLOAT, 0, 0);
		__geomHigh->setGeomIndexParams(__ibHigh, R_IndexFormat::IDXFMT_16);
		__geomHigh->finishCreatingGeometry();
	}

	{ // Geom Default
		vector<uint16>& mapArrayDefault = Map_Shared::GenarateDefaultMapArray(header.holes);
		m_IndexesCountDefault = mapArrayDefault.size();
		__ibDefault = _Render->r.createIndexBuffer(mapArrayDefault.size() * sizeof(uint16), mapArrayDefault.data(), false);

		__geomDefault = _Render->r.beginCreatingGeometry(_Render->getRenderStorage()->__layout_GxVBF_PNT2);
		__geomDefault->setGeomVertexParams(verticesBuffer, R_DataType::T_FLOAT, 0, 0);
		__geomDefault->setGeomVertexParams(normalsBuffer, R_DataType::T_FLOAT, 0, 0);
		__geomDefault->setGeomVertexParams(Map_Shared::BufferTextureCoordDetail, R_DataType::T_FLOAT, 0, 0);
		__geomDefault->setGeomVertexParams(Map_Shared::BufferTextureCoordAlpha, R_DataType::T_FLOAT, 0, 0);
		__geomDefault->setGeomIndexParams(__ibDefault, R_IndexFormat::IDXFMT_16);
		__geomDefault->finishCreatingGeometry();
	}

	////////////////////////////
	// Debug geom
	__geomDebugNormals = _Render->r.beginCreatingGeometry(_Render->getRenderStorage()->__layout_GxVBF_PN);
	__geomDebugNormals->setGeomVertexParams(verticesBuffer, R_DataType::T_FLOAT, 0, 0);
	__geomDebugNormals->setGeomVertexParams(normalsBuffer, R_DataType::T_FLOAT, 0, 0);
	__geomDebugNormals->setGeomIndexParams(__ibDefault, R_IndexFormat::IDXFMT_16);
	__geomDebugNormals->finishCreatingGeometry();

	//--

	m_BlendRBGShadowATexture = _Render->r.createTexture(R_TextureTypes::Tex2D, 64, 64, 1, R_TextureFormats::RGBA8, false, false, false, false);
	m_BlendRBGShadowATexture->uploadTextureData(0, 0, blendbuf);

	return true;
}

bool ADT_MCNK::Delete()
{
	return true;
}

void ADT_MCNK::PreRender3D()
{
	setVisible(false);

	// Check distance to camera
	float distToCamera2D = (_Render->getCamera()->Position.toX0Z() - m_Bounds.getCenter().toX0Z()).length();
	if (distToCamera2D > m_DistancesSettings.ADT_MCNK_Distance)
	{
		return;
	}

	// Check frustrum
	if (_Render->getCamera()->_frustum.cullBox(m_Bounds))
	{
		return;
	}

	setVisible(true);
}

void ADT_MCNK::Render3D()
{
	if (!m_QualitySettings.draw_mcnk)
	{
		return;
	}

	PERF_START(PERF_MAP);
	{
		_Render->r.setCullMode(R_CullMode::RS_CULL_BACK);
		//_Render->r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);
		_Render->r.setDepthTest(true);
		_Render->r.setDepthMask(true);
		_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

		_Render->getTechniquesMgr()->MCNK_Pass->Bind();
		{
			_Render->getTechniquesMgr()->MCNK_Pass->SetLayersCount(header.nLayers);

			// Bind m_DiffuseTextures
			for (uint32 i = 0; i < header.nLayers; i++)
			{
				_Render->r.setTexture(i, m_DiffuseTextures[i], m_QualitySettings.Texture_Sampler | SS_ADDR_WRAP, 0);
				_Render->r.setTexture(5 + i, m_SpecularTextures[i], m_QualitySettings.Texture_Sampler | SS_ADDR_WRAP, 0);
			}

			// Bind blend
			if (header.nLayers > 0)
			{
				_Render->r.setTexture(4, m_BlendRBGShadowATexture, SS_ADDR_CLAMP, 0);
			}

			// Bind shadow
			_Render->getTechniquesMgr()->MCNK_Pass->SetShadowMapExists(header.flags.has_mcsh);
			if (header.flags.has_mcsh)
			{
				_Render->getTechniquesMgr()->MCNK_Pass->SetShadowColor(vec3(0.0f, 0.0f, 0.0f) * 0.3f);
			}

			float distToCamera3D = (_Render->getCamera()->Position - m_Bounds.getCenter()).length();
			if (distToCamera3D > m_DistancesSettings.ADT_MCNK_HighRes_Distance || m_QualitySettings.draw_mcnk_low)
			{
				_Render->getTechniquesMgr()->MCNK_Pass->SetIsLowRes(1);
				_Render->r.setGeometry(__geomDefault);
				_Render->r.drawIndexed(PRIM_TRILIST, 0, m_IndexesCountDefault, 0, C_MapBufferSize, true);
			}
			else
			{
				_Render->getTechniquesMgr()->MCNK_Pass->SetIsLowRes(0);
				_Render->r.setGeometry(__geomHigh);
				_Render->r.drawIndexed(PRIM_TRILIST, 0, m_IndexesCountHigh, 0, C_MapBufferSize, true);
			}
		}
		_Render->getTechniquesMgr()->MCNK_Pass->Unbind();
	}
	PERF_STOP(PERF_MAP);
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


void ADT_MCNK::RenderNormals()
{
	_Render->getTechniquesMgr()->DebugNormal_Pass->Bind();

	_Render->r.setGeometry(__geomDebugNormals);
	_Render->r.drawIndexed(PRIM_TRILIST, 0, m_IndexesCountHigh, 0, C_MapBufferSize);

	_Render->getTechniquesMgr()->DebugNormal_Pass->Unbind();
}

