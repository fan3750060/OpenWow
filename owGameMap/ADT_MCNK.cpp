#include "stdafx.h"

// Include
#include "ADT.h"
#include "MapController.h"

// General
#include "ADT_MCNK.h"

// Additional
#include "ADT_Liquid.h"
#include "MapController.h"
#include "Map_Shared.h"
#include "ADT_MCNK_Material.h"

ADT_MCNK::ADT_MCNK(std::weak_ptr<MapController> _mapController, std::weak_ptr<ADT> _parentTile, std::shared_ptr<IFile> _file) :
	m_MapController(_mapController),
	m_ParentADT(_parentTile),
	m_File(_file),
	m_BlendRBGShadowATexture(0),
	m_LiquidInstance(nullptr),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	memset(mcly, 0x00, sizeof(ADT_MCNK_MCLY) * 4);
}

ADT_MCNK::~ADT_MCNK()
{
	//Log::Info("ADT_MCNK Deleted");
}

void ADT_MCNK::UpdateLocalTransform()
{
	SetLocalUnderty();
}

//
// SceneNode
//
bool ADT_MCNK::Accept(IVisitor& visitor)
{
	const BasePass& visitorAsBasePass = reinterpret_cast<BasePass&>(visitor);
	const Camera& camera = *(visitorAsBasePass.GetRenderEventArgs().Camera);

	float distToCamera2D = (camera.GetTranslation() - getBounds().getCenter()).length() - getBounds().getRadius();
	if (distToCamera2D > m_QualitySettings.ADT_MCNK_Distance)
	{
		return false;
	}

	// Check frustrum
	if (!checkFrustum(camera))
	{
		return false;
	}

	return SceneNode::Accept(visitor);
}

//

bool ADT_MCNK::Load()
{
	uint32_t startPos = m_File->getPos();

	// Read header
	m_File->readBytes(&header, sizeof(ADT_MCNK_Header));

	// Scene node params
	{
		// Set translate
		setTranslate(vec3(header.xpos * (-1.0f) + C_ZeroPoint, header.ypos, header.zpos * (-1.0f) + C_ZeroPoint));
		// Bounds
		BoundingBox bbox
		(
			vec3(getTranslate().x,               Math::MaxFloat, getTranslate().z), 
			vec3(getTranslate().x + C_ChunkSize, Math::MinFloat, getTranslate().z + C_ChunkSize)
		);
		setBounds(bbox);
	}

	std::shared_ptr<Buffer> verticesBuffer = nullptr;
	std::shared_ptr<Buffer> normalsBuffer = nullptr;
	std::shared_ptr<Buffer> mccvBuffer = nullptr;

	uint8 blendbuf[64 * 64 * 4];
	memset(blendbuf, 0, 64 * 64 * 4);

	// Normals
	m_File->seek(startPos + header.ofsNormal);
	{
		struct int24
		{
			int24() :
				x(0),
				y(0),
				z(0)
			{}

			int24(int8 _x, int8 _y, int8 _z) :
				x(_x),
				y(_y),
				z(_z)
			{}

			int8 x;
			int8 y;
			int8 z;
		};

		/*int24 normals_INT24[C_MapBufferSize];
		memset(normals_INT24, 0x00, sizeof(int24) * C_MapBufferSize);
		int24* t_normals_INT24 = normals_INT24;*/

		vec3 tempNormals[C_MapBufferSize];
		vec3* ttn = tempNormals;

		for (int j = 0; j < 17; j++)
		{
			for (uint32 i = 0; i < ((j % 2) ? 8 : 9); i++)
			{
				int24 nor;
				m_File->readBytes(&nor, sizeof(int24));

				*ttn++ = vec3(-(float)nor.y / 127.0f, (float)nor.z / 127.0f, -(float)nor.x / 127.0f);
				//*t_normals_INT24++ = nor;
			}
		}

		//normalsBuffer = _Render->r.createVertexBuffer(C_MapBufferSize * sizeof(vec3), tempNormals, false);
		//normalsBuffer = _Render->r.createVertexBuffer(C_MapBufferSize * sizeof(int24), normals_INT24, false);
		normalsBuffer = _RenderDevice->CreateVertexBuffer(tempNormals, C_MapBufferSize);
	}

	// Heights
	m_File->seek(startPos + header.ofsHeight);
	{
		float heights[C_MapBufferSize];
		float* t_heights = heights;

		vec3 tempVertexes[C_MapBufferSize];
		vec3* ttv = tempVertexes;

		BoundingBox bbox = getBounds();

		for (uint32 j = 0; j < 17; j++)
		{
			for (uint32 i = 0; i < ((j % 2) ? 8 : 9); i++)
			{
				float h;
				m_File->readBytes(&h, sizeof(float));

				float xpos = i * C_UnitSize;
				float zpos = j * 0.5f * C_UnitSize;
				if (j % 2)
				{
					xpos += C_UnitSize * 0.5f;
				}

				vec3 v = getTranslate() + vec3(xpos, h, zpos);
				*ttv++ = v;

				bbox.setMinY(minf(v.y, bbox.getMin().y));
				bbox.setMaxY(maxf(v.y, bbox.getMax().y));
			}
		}

		bbox.calculateCenter();
		setBounds(bbox);

		verticesBuffer = _RenderDevice->CreateVertexBuffer(tempVertexes, C_MapBufferSize);
	}

	// Textures
	m_File->seek(startPos + header.ofsLayer);
	{
		std::shared_ptr<ADT> parentADT = m_ParentADT.lock();
		assert1(parentADT != NULL);

		for (uint32 i = 0; i < header.nLayers; i++)
		{
			m_File->readBytes(&mcly[i], sizeof(ADT_MCNK_MCLY));

			m_DiffuseTextures[i] = parentADT->m_Textures.at(mcly[i].textureIndex)->diffuseTexture;
			m_SpecularTextures[i] = parentADT->m_Textures.at(mcly[i].textureIndex)->specularTexture;
		}
	}

	// Shadows
	m_File->seek(startPos + header.ofsShadow);
	{
		uint8 sbuf[64 * 64];
		uint8* p;
		uint8 c[8];
		p = sbuf;
		for (int j = 0; j < 64; j++)
		{
			m_File->readBytes(c, 8);
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
	m_File->seek(startPos + header.ofsAlpha);
	{
		std::shared_ptr<MapController> mapController = m_MapController.lock();
		assert1(mapController != NULL);

		for (uint32 i = 1; i < header.nLayers; i++)
		{
			uint8 amap[64 * 64];
			memset(amap, 0x00, 64 * 64);
			const uint8* abuf = m_File->getDataFromCurrent() + mcly[i].offsetInMCAL;

			if (mcly[i].flags.alpha_map_compressed) // Compressed: MPHD is only about bit depth!
			{
				// compressed
				const uint8* input = abuf;

				for (uint16 offset_output = 0; offset_output < 4096;)
				{
					const bool fill = *input & 0x80;
					const uint16 n = *input & 0x7F;
					++input;

					if (fill)
					{
						memset(&amap[offset_output], *input, n);
						++input;
					}
					else
					{
						memcpy(&amap[offset_output], input, n);
						input += n;
					}

					offset_output += n;
				}
			}
			else if (mapController->isUncompressedAlpha()) // Uncomressed (4096)
			{
				memcpy(amap, abuf, 64 * 64);
			}
			else
			{
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
	m_BlendRBGShadowATexture = _RenderDevice->CreateTexture();
	m_BlendRBGShadowATexture->LoadTextureCustom(64, 64, blendbuf);

	// Liquids
	m_File->seek(startPos + header.ofsLiquid);
	{
		if (header.sizeLiquid > 8)
		{
			CRange height;
			m_File->readBytes(&height, 8);

			std::shared_ptr<CADT_Liquid> m_Liquid = std::make_shared<CADT_Liquid>(8, 8);
			m_Liquid->CreateFromMCLQ(m_File, header);

			m_LiquidInstance = std::make_shared<Liquid_Instance>(m_Liquid, vec3(getTranslate().x, 0.0f, getTranslate().z));
			m_LiquidInstance->SetParent(weak_from_this());
		}
	}

	// MCCV colors
	m_File->seek(startPos + header.ofsMCCV);
	{
		uint32 mccvColorsUINT8[C_MapBufferSize];
		memset(mccvColorsUINT8, 0x00, sizeof(uint32) * C_MapBufferSize);

		if (header.flags.has_mccv)
		{
			uint32* t_mccvColorsUINT8 = mccvColorsUINT8;

			for (int j = 0; j < 17; j++)
			{
				for (uint32 i = 0; i < ((j % 2u) ? 8u : 9u); i++)
				{
					uint8 nor[4];
					m_File->readBytes(&nor, sizeof(uint32));

					*t_mccvColorsUINT8++ = uint32(
						(uint8)(nor[3]) << 24 |
						(uint8)(nor[0]) << 16 |
						(uint8)(nor[1]) << 8 |
						(uint8)(nor[2])
					);
				}
			}
		}

		mccvBuffer = _RenderDevice->CreateUInt32VertexBuffer(mccvColorsUINT8, C_MapBufferSize, 0, sizeof(uint8));
	}

	m_File.reset();

	{ // Geom High
		/*vector<uint16>& mapArrayHigh = _MapShared->GenarateHighMapArray(header.holes);
		__ibHigh = _RenderDevice->CreateUInt16IndexBuffer((const uint16*)mapArrayHigh.data(), mapArrayHigh.size());

		__geomHigh = _RenderDevice->CreateMesh();
		__geomHigh->AddVertexBuffer(BufferBinding("POSITION", 0), verticesBuffer);
		__geomHigh->SetIndexBuffer(__ibHigh);
		__geomHigh->SetMaterial(_RenderDevice->GetDefaultMaterial());


		AddMesh(__geomHigh);*/

		/*__geomHigh = _Render->r.beginCreatingGeometry(PRIM_TRILIST, _Render->getRenderStorage()->__layout_GxVBF_PNCT2);
		__geomHigh->setGeomVertexParams(verticesBuffer, R_DataType::T_FLOAT, 0, 0);
		__geomHigh->setGeomVertexParams(normalsBuffer, R_DataType::T_INT8, 0, 0);
		__geomHigh->setGeomVertexParams(mccvBuffer, R_DataType::T_UINT8, 0, 0, true);
		__geomHigh->setGeomVertexParams(_MapShared->BufferTextureCoordDetail, R_DataType::T_FLOAT, 0, 0);
		__geomHigh->setGeomVertexParams(_MapShared->BufferTextureCoordAlpha, R_DataType::T_FLOAT, 0, 0);
		__geomHigh->setGeomIndexParams(__ibHigh, R_IndexFormat::IDXFMT_16);
		__geomHigh->finishCreatingGeometry();*/
	}



	// Material
	std::shared_ptr<ADT_MCNK_Material> mat = std::make_shared<ADT_MCNK_Material>();
	for (uint32 i = 0; i < header.nLayers; i++)
	{
		mat->SetTexture(i, m_DiffuseTextures[i]); // DXT1
	}
	mat->SetTexture(4, m_BlendRBGShadowATexture);
	mat->SetLayersCnt(header.nLayers);

	{ // Geom Default
		std::vector<uint16>& mapArrayDefault = _MapShared->GenarateDefaultMapArray(header.holes);
		std::shared_ptr<Buffer> __ibDefault = _RenderDevice->CreateIndexBuffer(mapArrayDefault);

		__geomDefault = _RenderDevice->CreateMesh();
		__geomDefault->AddVertexBuffer(BufferBinding("POSITION", 0), verticesBuffer);
		__geomDefault->AddVertexBuffer(BufferBinding("NORMAL", 0), normalsBuffer);
		//__geomDefault->AddVertexBuffer(BufferBinding("COLOR", 0), mccvBuffer);
		__geomDefault->AddVertexBuffer(BufferBinding("TEXCOORD", 0), _MapShared->BufferTextureCoordDetail);
		__geomDefault->AddVertexBuffer(BufferBinding("TEXCOORD", 1), _MapShared->BufferTextureCoordAlpha);
		__geomDefault->SetIndexBuffer(__ibDefault);
		__geomDefault->SetMaterial(mat);
		__geomDefault->SetType(SN_TYPE_ADT_CHUNK);

		AddMesh(__geomDefault);
	}

	return true;
}

bool ADT_MCNK::Delete()
{
	return true;
}

/*
	if (!m_QualitySettings.draw_mcnk)
	{
		return;
	}

	int8 layersCnt = header.nLayers - 1;
	if (layersCnt < 0)
	{
		return;
	}

	float distToCamera3D = glm::length(_Render->getCamera()->Position - getBounds().getCenter()) - getBounds().getRadius();
	bool isDefaultGeom = distToCamera3D > m_QualitySettings.ADT_MCNK_HighRes_Distance || m_QualitySettings.draw_mcnk_low;

	{
		CMCNK_Pass* pass = _Render->getTechniquesMgr()->MCNK_Pass.operator->();

		pass->Bind();
		{
			pass->SetIsNortrend(truem_ParentADT->m_Header.flags.IsNortrend);
			pass->SetIsMCCVExists(m_QualitySettings.draw_map_mccv && header.flags.has_mccv);
			pass->SetLayersCount(header.nLayers);

			// Bind shadow
			pass->SetShadowMapExists(header.flags.has_mcsh);
			if (header.flags.has_mcsh)
			{
				pass->SetShadowColor(vec3(0.0f, 0.0f, 0.0f) * 0.3f);
			}

			if (isDefaultGeom)
			{
				_Render->r.drawIndexed(0, m_IndexesCountDefault, 0, C_MapBufferSize, &m_StateDefault, true);
			}
			else
			{
				_Render->r.drawIndexed(0, m_IndexesCountHigh, 0, C_MapBufferSize, &m_StateHigh, true);
			}
		}
		pass->Unbind();
	}
*/

//

/*void ADT_MCNK::drawPass(int anim) VERY OLD :)
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
