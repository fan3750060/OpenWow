#include "stdafx.h"

// General
#include "Mesh.h"

Mesh::Mesh()
	: m_Type(SN_TYPE_NONE)
{
}

SceneNodeTypes Mesh::GetType() const
{
	return m_Type;
}

void Mesh::SetType(SceneNodeTypes type)
{
	m_Type = type;
}
