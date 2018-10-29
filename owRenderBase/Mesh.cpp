#include "stdafx.h"

// General
#include "Mesh.h"

IMesh::IMesh()
	: m_Type(SN_TYPE_NONE)
{
}

IMesh::~IMesh()
{
}

SceneNodeTypes IMesh::GetType() const
{
	return m_Type;
}

void IMesh::SetType(SceneNodeTypes type)
{
	m_Type = type;
}
