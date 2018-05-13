#include "stdafx.h"

// General
#include "MDX.h"

// Additional
#include "MDX_Skin_Batch.h"

MDX::MDX(cstring name) : RefItemNamed(name), m_Loaded(false)
{
	//Log::Info("MDX[%s]: Loading...", m_ModelFileName.c_str());
	
	// Replace .MDX with .M2
	m_ModelFileName = name;
	if (m_ModelFileName.back() != '2')
	{
		m_ModelFileName[m_ModelFileName.length() - 2] = '2';
		m_ModelFileName[m_ModelFileName.length() - 1] = '\0';
		m_ModelFileName.resize(m_ModelFileName.length() - 1);
	}

	m_ModelName = m_ModelFileName.substr(0, m_ModelFileName.length() - 3);

	//

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		m_SpecialTextures[i] = -1;
		m_TextureReplaced[i] = nullptr;
		m_TexturesUseSpecialTexture[i] = false;
	}

	m_GlobalLoops = nullptr;
	m_AnimationTime = 0;
	m_AnimationIndex = 0;
	m_Colors = nullptr;
	m_Lights = nullptr;
	m_TextureWeights = nullptr;

#ifdef MDX_PARTICLES_ENABLE
	particleSystems = nullptr;
	ribbons = nullptr;
#endif
}

MDX::~MDX()
{
	if (!m_Loaded)
	{
		return;
	}

	Log::Info("MDX[%s]: Unloading...", m_ModelFileName.c_str());

	if (header.textures.size)
	{
		for (uint32 i = 0; i < header.textures.size; i++)
		{
			if (m_Textures[i] != nullptr)
			{
				_TexturesMgr->Delete(m_Textures[i]);
			}
		}
	}
}

//

void MDX::Init(bool forceAnim)
{
	File f = m_ModelFileName;
	if (!f.Open())
	{
		Log::Info("MDX[%s]: Unable to open file.", m_ModelFileName.c_str());
		return;
	}

	// Header
	memcpy(&header, f.GetData(), sizeof(ModelHeader));

	m_Bounds.set(header.bounding_box.min, header.bounding_box.max, true);

	// Is animated
	animated = isAnimated(f) || forceAnim;

	if (header.global_loops.size)
	{
		m_GlobalLoops = new uint32[header.global_loops.size];
		memcpy(m_GlobalLoops, (f.GetData() + header.global_loops.offset), sizeof(M2Loop) * header.global_loops.size);
	}

	if (animated)
	{
		initAnimated(f);
	}
	else
	{
		initCommon(f);
	}

	m_Loaded = true;
}



void MDX::initCommon(File& f)
{
	M2Vertex* m_OriginalVertexes = (M2Vertex*)(f.GetData() + header.vertices.offset);

	// Convert vertices
	for (uint32 i = 0; i < header.vertices.size; i++)
	{
		From_XYZ_To_XZminusY(m_OriginalVertexes[i].pos);
		From_XYZ_To_XZminusY(m_OriginalVertexes[i].normal);
	}

	// m_DiffuseTextures
	if (header.textures.size)
	{
		//m_Textures = new R_Texture*[header.textures.size];
		m_M2Textures = (M2Texture*)(f.GetData() + header.textures.offset);

		
		for (uint32 i = 0; i < header.textures.size; i++)
		{
			assert1(i < TEXTURE_MAX);

			if (m_M2Textures[i].type == 0) // Common texture
			{
				char buff[256];
				strncpy_s(buff, (const char*)(f.GetData() + m_M2Textures[i].filename.offset), m_M2Textures[i].filename.size);
				buff[m_M2Textures[i].filename.size] = '\0';
				m_Textures.push_back(_TexturesMgr->Add(buff));
			}
			else // special texture - only on characters and such...
			{
				/*m_Textures[i] = nullptr;
				m_SpecialTextures[i] = m_M2Textures[i].type;

				if (m_M2Textures[i].type < TEXTURE_MAX)
				{
					m_TexturesUseSpecialTexture[m_M2Textures[i].type] = true;
				}

				// a fix for weapons with type-3 m_DiffuseTextures.
				if (m_M2Textures[i].type == 3)
				{
					m_TextureReplaced[m_M2Textures[i].type] = _TexturesMgr->Add("Item\\ObjectComponents\\Weapon\\ArmorReflect4.BLP");
				}*/
			}
		}
	}

	// init colors
	if (header.colors.size)
	{
		m_Colors = new MDX_Part_Color[header.colors.size];
		for (uint32 i = 0; i < header.colors.size; i++)
		{
			m_Colors[i].init(f, ((M2Color*)(f.GetData() + header.colors.offset))[i], m_GlobalLoops);
		}
	}

	// init transparency
	if (header.texture_weights.size)
	{
		m_TextureWeights = new MDX_Part_TextureWeight[header.texture_weights.size];
		for (uint32 i = 0; i < header.texture_weights.size; i++)
		{
			m_TextureWeights[i].init(f, ((M2TextureWeight*)(f.GetData() + header.texture_weights.offset))[i], m_GlobalLoops);
		}
	}
	
	// Vertex buffer
	__vb = _Render->r->createVertexBuffer(header.vertices.size * 12 * sizeof(float), m_OriginalVertexes);

	// Load LODs
	assert1(header.skin_profiles.size);
	for (uint32 i = 0; i < header.skin_profiles.size; i++)
	{
		m_Skins.push_back(new Model_Skin(this, f, ((M2SkinProfile*)(f.GetData() + header.skin_profiles.offset))[i]));
	}
}

//

void MDX::drawModel()
{
	for (uint32 i = 0; i < header.colors.size; i++)
	{
		m_Colors[i].calc(m_AnimationIndex, m_AnimationTime);
	}

	/*for (auto it = m_Skins.begin(); it != m_Skins.end(); ++it)
	{
		(*it)->Draw();
	}*/

	m_Skins.back()->Draw();
}

void MDX::Render()
{
	if (!m_Loaded)
	{
		return;
	}
	
	if (animated && false)
	{
		uint32 duration = m_Sequences[m_AnimationIndex].end_timestamp - m_Sequences[m_AnimationIndex].start_timestamp;

		if (duration > 0 )
		{
			/*if (duration == 0)
			{
				duration = 50;
			}*/
			m_AnimationTime = _EnvironmentManager->globalTime % duration;


			if (m_IsBillboard)
			{
				animate(0);
			}
			else
			{
				if (!animcalc)
				{
					animate(0);
					animcalc = true;
				}
			}

			// draw particle systems
#ifdef MDX_PARTICLES_ENABLE
			for (uint32 i = 0; i < header.particle_emitters.size; i++)
			{
				particleSystems[i].draw();
			}

			// draw ribbons
			for (uint32 i = 0; i < header.ribbon_emitters.size; i++)
			{
				ribbons[i].draw();
			}
#endif
		}
	}

	drawModel();
}

//

void MDX::lightsOn(uint32 lbase)
{
	/*if (!m_Loaded)
	{
		return;
	}*/

	// setup lights
	for (uint32 i = 0, l = lbase; i < header.lights.size; i++)
	{
		m_Lights[i].setup(m_AnimationTime, l++);
	}
}

void MDX::lightsOff(uint32 lbase)
{
	/*if (!m_Loaded)
	{
		return;
	}*/

	for (uint32 i = 0, l = lbase; i < header.lights.size; i++)
	{
		//glDisable(l++);
	}
}

void MDX::updateEmitters(float dt)
{
	if (!m_Loaded)
	{
		return;
	}

#ifdef MDX_PARTICLES_ENABLE
	for (uint32 i = 0; i < header.particle_emitters.size; i++)
	{
		particleSystems[i].update(dt);
	}
#endif
}
