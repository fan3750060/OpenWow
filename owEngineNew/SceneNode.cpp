#include "stdafx.h"

// General
#include "SceneNode.h"

/*SceneNode::SceneNode(std::weak_ptr<SceneNode> _parent)
	: m_Name("SceneNode")
{
	SetParent(_parent);
}*/

SceneNode::SceneNode(cmat4 localTransform)
	: m_LocalTransform(localTransform)
	, m_Name("SceneNode")
	, m_Type(SN_TYPE_NONE)
	, m_Scale(1.0f, 1.0f, 1.0f)
{
	m_InverseTransform = glm::inverse(m_LocalTransform);
}

SceneNode::~SceneNode()
{
	// Delete children.
	m_Children.clear();
}

cstring SceneNode::GetName() const
{
	return m_Name;
}

void  SceneNode::SetName(cstring name)
{
	m_Name = name;
}

mat4 SceneNode::GetLocalTransform() const
{
	return m_LocalTransform;
}

void SceneNode::SetLocalTransform(cmat4 localTransform)
{
	m_LocalTransform = localTransform;
	m_InverseTransform = glm::inverse(localTransform);
}

mat4 SceneNode::GetInverseLocalTransform() const
{
	return m_InverseTransform;
}

mat4 SceneNode::GetWorldTransfom() const
{
	return GetParentWorldTransform() * m_LocalTransform;
}

void SceneNode::SetWorldTransform(cmat4 worldTransform)
{
	mat4 inverseParentTransform = glm::inverse(GetParentWorldTransform());
	SetLocalTransform(inverseParentTransform * worldTransform);
}

mat4 SceneNode::GetInverseWorldTransform() const
{
	return glm::inverse(GetWorldTransfom());
}

mat4 SceneNode::GetParentWorldTransform() const
{
	mat4 parentTransform(1.0f);
	if (std::shared_ptr<SceneNode> parent = m_pParentNode.lock())
	{
		parentTransform = parent->GetWorldTransfom();
	}

	return parentTransform;
}

void SceneNode::CalculateLocalTransform(bool _isRotationQuat)
{
	m_LocalTransform = mat4();

	m_LocalTransform = glm::translate(m_LocalTransform, m_Translate);
	if (_isRotationQuat)
	{
		m_LocalTransform *= glm::toMat4(m_RotateQuat);
	}
	else
	{
		m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.x, vec3(1, 0, 0));
		m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.y, vec3(0, 1, 0));
		m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.z, vec3(0, 0, 1));
	}
	m_LocalTransform = glm::scale(m_LocalTransform, m_Scale);
	m_InverseTransform = glm::inverse(m_LocalTransform);
}

void SceneNode::AddChild(std::shared_ptr<SceneNode> pNode)
{
	if (pNode)
	{
		NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), pNode);
		if (iter == m_Children.end())
		{
			//mat4 worldTransform = pNode->GetWorldTransfom();
			pNode->m_pParentNode = shared_from_this();
			//mat4 localTransform = GetInverseWorldTransform() * worldTransform;
			//pNode->SetLocalTransform(localTransform);
			m_Children.push_back(pNode);
			if (!pNode->GetName().empty())
			{
				m_ChildrenByName.insert(NodeNameMap::value_type(pNode->GetName(), pNode));
			}
		}
	}
}

void SceneNode::RemoveChild(std::shared_ptr<SceneNode> pNode)
{
	if (pNode)
	{
		NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), pNode);
		if (iter != m_Children.end())
		{
			pNode->SetParent(std::weak_ptr<SceneNode>());

			m_Children.erase(iter);

			// Also remove it from the name map.
			NodeNameMap::iterator iter2 = m_ChildrenByName.find(pNode->GetName());
			if (iter2 != m_ChildrenByName.end())
			{
				m_ChildrenByName.erase(iter2);
			}
		}
		else
		{
			// Maybe this node appears lower in the hierarchy...
			for (auto child : m_Children)
			{
				child->RemoveChild(pNode);
			}
		}
	}
}

void SceneNode::SetParent(std::weak_ptr<SceneNode> wpNode)
{
	// Parents own their children.. If this node is not owned
	// by anyone else, it will cease to exist if we remove it from it's parent.
	// As a precaution, store myself is a shared pointer so I don't get deleted
	// half-way through this function!
	// Technically self deletion shouldn't occur because the thing invoking this function
	// should have a std::shared_ptr to it.
	//std::shared_ptr<SceneNode> me = shared_from_this();

	if (std::shared_ptr<SceneNode> parent = wpNode.lock())
	{
		parent->AddChild(shared_from_this());
	}
	else if (parent = m_pParentNode.lock())
	{
		// Setting parent to NULL.. remove from current parent and reset parent node.
		//mat4 worldTransform = GetWorldTransfom();
		parent->RemoveChild(shared_from_this());
		m_pParentNode.reset();
		//SetLocalTransform(worldTransform);
	}
}

void SceneNode::AddMesh(std::shared_ptr<IMesh> mesh)
{
	assert(mesh);
	MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
	if (iter == m_Meshes.end())
	{
		m_Meshes.push_back(mesh);
	}
}

void SceneNode::RemoveMesh(std::shared_ptr<IMesh> mesh)
{
	assert(mesh);
	MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
	if (iter != m_Meshes.end())
	{
		m_Meshes.erase(iter);
	}
}

void SceneNode::Update(Camera* camera)
{
}

void SceneNode::Accept(IVisitor& visitor)
{
	visitor.Visit(*this);

	// Visit meshes.
	for (auto mesh : m_Meshes)
	{
		mesh->Accept(visitor);
	}

	// Now visit children
	for (auto child : m_Children)
	{
		child->Accept(visitor);
	}
}

bool SceneNode::checkFrustum(const Camera& _camera) const
{
	return !_camera.GetFrustum().cullBox(getBounds());
}

bool SceneNode::checkDistance2D(cvec3 _camPos, float _distance) const
{
	// Check distance to camera
	float distToCamera2D = glm::length(Fix_X0Z(_camPos) - Fix_X0Z(getBounds().getCenter())) - getBounds().getRadius();
	return distToCamera2D < _distance;
}

bool SceneNode::checkDistance(cvec3 _camPos, float _distance) const
{
	// Check distance to camera
	float distToCamera = glm::length(_camPos - getBounds().getCenter()) - getBounds().getRadius();
	return distToCamera < _distance;
}