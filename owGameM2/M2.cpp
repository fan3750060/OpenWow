#include "stdafx.h"

// General
#include "M2.h"

M2::M2(cstring name) :
	m_FileName(name),
	m_UniqueName(""),
	// Loops and sequences
	m_IsAnimated(false),
	// Bones
	m_HasBones(false),
	m_IsAnimBones(false),
	m_IsBillboard(false),
	// Vertices
	m_IsContainGeom(false),
	// Colors and textures
	m_IsAnimTextures(false),
	// Misc
	m_HasMisc(false)
{
	//Log::Info("M2[%s]: Loading...", m_FileName.c_str());
}

M2::~M2()
{
	ERASE_VECTOR(m_Bones);

	ERASE_VECTOR(m_Colors);
	ERASE_VECTOR(m_Materials);
	ERASE_VECTOR(m_Textures);
	ERASE_VECTOR(m_TextureWeights);
	ERASE_VECTOR(m_TexturesTransform);

	ERASE_VECTOR(m_Events);
	ERASE_VECTOR(m_Attachments);
	ERASE_VECTOR(m_Lights);
	ERASE_VECTOR(m_Cameras);

	ERASE_VECTOR(m_RibbonEmitters);


	ERASE_VECTOR(m_Skins);

	//Log::Info("M2[%s]: Unloading...", m_FileName.c_str());
}

void M2::drawModel(cmat4 _worldMatrix, CM2_MeshPartID_Provider* _provider, cvec4 _doodadColor)
{
	if (!m_IsContainGeom)
	{
		return;
	}

	CM2_Pass* pass = _Render->getTechniquesMgr()->M2_Pass;
	pass->Bind();
	{
		pass->setWorld(_worldMatrix);
		pass->SetAnimated(m_HasBones && m_IsAnimated);
		pass->SetColorDoodad(_doodadColor);

		if (m_HasBones && m_IsAnimated)
		{
			//pass->SetBoneStartIndex(p->bonesStartIndex); FIXME
			//pass->SetBoneMaxCount(p->boneInfluences);

			vector<mat4> bones;
			for (uint32 i = 0; i < m_Bones.size(); i++)
			{
				bones.push_back(m_Bones[i]->getTransformMatrix());
			}
			pass->SetBones(bones);
		}

		/*for (auto& it : m_Skins)
		{
			it->Draw(_provider);
			break;
		}*/
		m_Skins.back()->Draw(_provider);
	}
	pass->Unbind();

	/*RenderCollision(_worldMatrix);
	for (auto& it : m_Skins)
	{
		it->RenderNormals();
	}*/
}

void M2::Render(cmat4 _worldMatrix, CM2_MeshPartID_Provider* _provider, cvec4 _doodadColor)
{
	if (m_IsAnimated)
	{
		// draw ribbons
		for (auto it : m_RibbonEmitters)
		{
			it->Render(_worldMatrix);
		}

#ifdef MDX_PARTICLES_ENABLE
		// draw particle systems
		for (uint32 i = 0; i < m_Header.particle_emitters.size; i++)
		{
			particleSystems[i].draw();
		}
#endif
	}

	drawModel(_worldMatrix, _provider, _doodadColor);
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
	_Render->getTechniquesMgr()->Debug_Pass->setWorld(_worldMatrix);
	_Render->getTechniquesMgr()->Debug_Pass->SetColor4(vec4(0.0f, 1.0f, 0.0f, 0.7f));

	_Render->r.setGeometry(m_CollisionGeom);
	_Render->r.drawIndexed(0, m_CollisionIndCnt, 0, m_CollisionVetCnt);

	_Render->getTechniquesMgr()->Debug_Pass->Unbind();

	_Render->r.setFillMode(R_FillMode::RS_FILL_SOLID);
	_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
}

void M2::animate(uint16 _animationIndex, cmat4 _worldMatrix, uint32 _time, uint32 globalTime)
{
	if (m_HasBones)
	{
		for (uint32 i = 0; i < m_Bones.size(); i++)
		{
			m_Bones[i]->SetNeedCalculate();
		}

		for (uint32 i = 0; i < m_Bones.size(); i++)
		{
			m_Bones[i]->calcMatrix(_animationIndex, _time, globalTime);
		}

		for (uint32 i = 0; i < m_Bones.size(); i++)
		{
			m_Bones[i]->calcBillboard(_worldMatrix);
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


	for (auto& it : m_RibbonEmitters)
	{
		it->setup(_animationIndex, _time, globalTime, _worldMatrix);
	}

#ifdef MDX_PARTICLES_ENABLE
	for (uint32 i = 0; i < m_Header.particle_emitters.size; i++)
	{
		// random time distribution for teh win ..?
		int pt = (animtime + (int)(tmax*particleSystems[i].tofs)) % tmax;
		particleSystems[i].setup(_animationIndex, _time);
	}
#endif

	for (auto& it : m_TexturesTransform)
	{
		it->calc(_animationIndex, _time, globalTime);
	}

	for (auto& it : m_TextureWeights)
	{
		it->calc(_animationIndex, _time, globalTime);
	}

	for (auto& it : m_Colors)
	{
		it->calc(_animationIndex, _time, globalTime);
	}

	for (auto& it : m_Cameras)
	{
		it->calc(_time, globalTime);
	}
}

//
void M2::updateEmitters(float dt)
{
#ifdef MDX_PARTICLES_ENABLE
	for (uint32 i = 0; i < m_Header.particle_emitters.size; i++)
	{
		particleSystems[i].update(dt);
	}
#endif
}
