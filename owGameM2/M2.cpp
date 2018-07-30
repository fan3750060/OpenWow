#include "stdafx.h"

// Include
#include "M2_Base_Instance.h"
#include "M2_Builder.h"
#include "M2_Skin_Builder.h"

// General
#include "M2.h"

M2::M2(cstring name) :
	m_FileName(name),
	m_UniqueName(""),

	m_Materials(nullptr),
	m_Miscellaneous(nullptr),
	m_Skeleton(nullptr),

	// Loops and sequences
	m_IsAnimated(false),

	// Vertices
	m_IsContainGeom(false)
{
	//Log::Info("M2[%s]: Loading...", m_FileName.c_str());
}

M2::~M2()
{
	SafeDelete(m_Materials);
	SafeDelete(m_Miscellaneous);
	SafeDelete(m_Skeleton);



	ERASE_VECTOR(m_Skins);

	//Log::Info("M2[%s]: Unloading...", m_FileName.c_str());
}

void M2::Render(CM2_Base_Instance* _instance)
{
	if (m_IsContainGeom)
	{
		CM2_Pass* pass = _Render->getTechniquesMgr()->M2_Pass.operator->();
		pass->Bind();
		{
			pass->setWorld(_instance->getAbsTrans());
			pass->SetColorDoodad(_instance->getColor());

			/*for (auto& it : m_Skins)
			{
				it->Draw(_instance);
				break;
			}*/
			m_Skins.back()->Draw(_instance);
		}
		pass->Unbind();

		/*RenderCollision(_worldMatrix);

		for (auto& it : m_Skins)
		{
		it->RenderNormals();
		}*/
	}

	// Ribbons
	m_Miscellaneous->render(_instance->getAbsTrans());
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

void M2::update(double _time, double _dTime)
{
	if (m_Miscellaneous)
	{
		m_Miscellaneous->update(_time, _dTime);
	}
}

void M2::calc(uint16 _animationIndex, cmat4 _worldMatrix, uint32 _time, uint32 globalTime)
{
	if (m_Skeleton)
	{
		m_Skeleton->calc(_animationIndex, _time, globalTime, _worldMatrix);
	}

	if (m_Materials)
	{
		m_Materials->calc(_animationIndex, _time, globalTime);
	}

	if (m_Miscellaneous)
	{
		m_Miscellaneous->calc(_animationIndex, _time, globalTime, _worldMatrix);
	}
}
