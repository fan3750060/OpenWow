#include "stdafx.h"

// General
#include "M2.h"

M2::M2(cstring name) :
	m_FileName(name),
	m_UniqueName(""),
	// Loops and sequences
	m_IsAnimated(false),
	// Bones
	m_HasBones(nullptr),
	m_IsAnimBones(false),
	m_IsBillboard(false),
	// Vertices
	m_IsContainGeom(false),
	// Colors and textures
	m_AnimTextures(false),
	// Misc
	m_HasMisc(false)
{
	//Log::Info("M2[%s]: Loading...", m_FileName.c_str());

	// Replace .M2 with .M2
	if (m_FileName.back() != '2')
	{
		m_FileName[m_FileName.length() - 2] = '2';
		m_FileName[m_FileName.length() - 1] = '\0';
		m_FileName.resize(m_FileName.length() - 1);
	}
}

M2::~M2()
{
	Log::Info("M2[%s]: Unloading...", m_FileName.c_str());
}

//

void M2::drawModel(cmat4 _worldMatrix)
{
	if (!m_IsContainGeom)
	{
		return;
	}

	//RenderCollision();

	_Render->getTechniquesMgr()->M2_Pass->Bind();
	_Render->getTechniquesMgr()->M2_Pass->SetWorldMatrix(_worldMatrix);
	_Render->getTechniquesMgr()->M2_Pass->SetAnimated(m_HasBones && m_IsAnimated);
	if (m_HasBones && m_IsAnimated)
	{
		//_Render->getTechniquesMgr()->M2_Pass->SetBoneStartIndex(p->bonesStartIndex); FIXME
		//_Render->getTechniquesMgr()->M2_Pass->SetBoneMaxCount(p->boneInfluences);

		vector<mat4> bones;
		for (uint32 i = 0; i < m_Header.bones.size; i++)
		{
			bones.push_back(m_Bones[i].getTransformMatrix());
		}
		_Render->getTechniquesMgr()->M2_Pass->SetBones(bones);
	}

	for (auto& it : m_Skins)
	{
		it->Draw();
	}

	_Render->getTechniquesMgr()->M2_Pass->Unbind();


	/*for (auto& it : m_Skins)
	{
		it->RenderNormals();
	}*/
}

void M2::Render(cmat4 _worldMatrix)
{
	if (m_IsAnimated)
	{
		// draw ribbons
		for (auto it : m_RibbonEmitters)
		{
			it.draw();
		}

#ifdef MDX_PARTICLES_ENABLE
		// draw particle systems
		for (uint32 i = 0; i < m_Header.particle_emitters.size; i++)
		{
			particleSystems[i].draw();
		}
#endif
	}

	drawModel(_worldMatrix);
}

void M2::RenderCollision(cmat4 _worldMatrix)
{
	if (m_CollisionGeom == nullptr)
	{
		return;
	}

	_Render->r.setCullMode(R_CullMode::RS_CULL_BACK);
	_Render->r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	_Render->getTechniquesMgr()->Debug_Pass->Bind();
	_Render->getTechniquesMgr()->Debug_Pass->SetWorldMatrix(_worldMatrix);
	_Render->getTechniquesMgr()->Debug_Pass->SetColor4(vec4(0.0f, 1.0f, 0.0f, 0.7f));

	_Render->r.setGeometry(m_CollisionGeom);
	_Render->r.drawIndexed(PRIM_TRILIST, 0, m_Header.collisionTriangles.size, 0, m_Header.collisionVertices.size);

	_Render->getTechniquesMgr()->Debug_Pass->Unbind();

	_Render->r.setFillMode(R_FillMode::RS_FILL_SOLID);
	_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
}

void M2::animate(uint16 _animationIndex, uint32 _time, uint32 globalTime)
{
	if (m_HasBones)
	{
		for (uint32 i = 0; i < m_Header.bones.size; i++)
		{
			m_Bones[i].SetNeedCalculate();
		}

		for (uint32 i = 0; i < m_Header.bones.size; i++)
		{
			m_Bones[i].calcMatrix(m_Bones.data(), _animationIndex, _time, globalTime);
		}
	}

	/*for (uint32 i = 0; i < m_Header.lights.size; i++)
	{
	if (m_Lights[i].getBoneIndex() >= 0)
	{
	m_Lights[i].tpos = m_Bones[m_Lights[i].getBoneIndex()].getTransformMatrix() * m_Lights[i].pos;
	m_Lights[i].tdir = m_Bones[m_Lights[i].getBoneIndex()].getRotateMatrix() * m_Lights[i].dir;
	}
	}*/


	for (auto it : m_RibbonEmitters)
	{
		it.setup(_animationIndex, _time, globalTime);
	}

#ifdef MDX_PARTICLES_ENABLE
	for (uint32 i = 0; i < m_Header.particle_emitters.size; i++)
	{
		// random time distribution for teh win ..?
		int pt = (animtime + (int)(tmax*particleSystems[i].tofs)) % tmax;
		particleSystems[i].setup(_animationIndex, _time);
	}
#endif

	if (m_AnimTextures)
	{
		for (uint32 i = 0; i < m_Header.textureTransforms.size; i++)
		{
			m_TexturesTransform[i].calc(_animationIndex, _time, globalTime);
		}
	}

	for (auto& it : m_TextureWeights)
	{
		it.calc(_animationIndex, _time, globalTime);
	}

	for (auto& it : m_Colors)
	{
		it.calc(_animationIndex, _time, globalTime);
	}

	for (auto& it : m_Cameras)
	{
		it.calc(_time, globalTime);
	}
}

//

void M2::lightsOn(uint32 lbase)
{
	/*if (!m_Loaded)
	{
		return;
	}

	// setup lights
	for (uint32 i = 0, l = lbase; i < m_Header.lights.size; i++)
	{
		m_Lights[i].setup(_time, l++);
	}*/
}

void M2::lightsOff(uint32 lbase)
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

void M2::updateEmitters(float dt)
{
#ifdef MDX_PARTICLES_ENABLE
	for (uint32 i = 0; i < m_Header.particle_emitters.size; i++)
	{
		particleSystems[i].update(dt);
	}
#endif
}
