#include "stdafx.h"

// General
#include "MDX.h"

// Additional
#include "MDX_Skin_Batch.h"
#include "WorldController.h"

MDX::MDX(cstring name) :
	m_Loaded(false),
	m_FileName(name),
	m_UniqueName(""),
	// Loops and sequences
	m_IsAnimated(false),
	// Bones
	m_HasBones(nullptr),
	m_AnimBones(false),
	m_IsBillboard(false),
	// Vertices
	m_ContainGeom(false),
	// Colors and textures
	m_AnimTextures(false),
	// Misc
	m_HasMisc(false),
	// My types
	m_AnimationIndex(0),
	m_AnimationTime(0)
{
	//Log::Info("MDX[%s]: Loading...", m_FileName.c_str());
	
	// Replace .MDX with .M2
	if (m_FileName.back() != '2')
	{
		m_FileName[m_FileName.length() - 2] = '2';
		m_FileName[m_FileName.length() - 1] = '\0';
		m_FileName.resize(m_FileName.length() - 1);
	}
}

MDX::~MDX()
{
	Log::Info("MDX[%s]: Unloading...", m_FileName.c_str());
}

//

void MDX::drawModel()
{
	for (uint32 i = 0; i < m_Header.colors.size; i++)
	{
		m_Colors[i].calc(m_AnimationIndex, m_AnimationTime, _World->EnvM()->globalTime);
	}

	if (!m_ContainGeom)
	{
		return;
	}

	for (auto it : m_Skins)
	{
		it->Draw();
		break;
	}
}

void MDX::Render()
{
	if (!m_Loaded)
	{
		return;
	}

	//assert1(getDeleted() == false);

	// Cull bounging box
	BoundingBox aabb = m_Bounds;
	aabb.transform(_Pipeline->GetWorld());
	if (_CameraFrustum->_frustum.cullBox(aabb))
	{
		return;
	}
	
	if (m_IsAnimated)
	{
		uint32 duration = m_Sequences[m_AnimationIndex].end_timestamp - m_Sequences[m_AnimationIndex].start_timestamp;

		if (duration > 0 )
		{
			/*if (duration == 0)
			{
				duration = 50;
			}*/
			m_AnimationTime = ((_World->EnvM()->globalTime) % duration) + m_Sequences[m_AnimationIndex].start_timestamp;

			if (m_IsBillboard)
			{
				animate(0, (_World->EnvM()->globalTime ));
			}
			else
			{
				if (!animcalc)
				{
					animate(0, _World->EnvM()->globalTime );
					animcalc = true;
				}
			}

			// draw particle systems
#ifdef MDX_PARTICLES_ENABLE
			for (uint32 i = 0; i < m_Header.particle_emitters.size; i++)
			{
				particleSystems[i].draw();
			}

			// draw ribbons
			for (uint32 i = 0; i < m_Header.ribbon_emitters.size; i++)
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
	for (uint32 i = 0, l = lbase; i < m_Header.lights.size; i++)
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

	for (uint32 i = 0, l = lbase; i < m_Header.lights.size; i++)
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
	for (uint32 i = 0; i < m_Header.particle_emitters.size; i++)
	{
		particleSystems[i].update(dt);
	}
#endif
}
