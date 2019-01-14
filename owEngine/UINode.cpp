#include "stdafx.h"

// General
#include "UINode.h"

/*UINode::UINode(std::weak_ptr<UINode> _parent)
	: m_Name("UINode")
{
	SetParent(_parent);
}*/

UINode::UINode()
	: m_Name("UINode")
	, m_Scale(1.0f, 1.0f)
	, m_IsLocalDirty(true)
	, m_IsWorldDirty(true)
{
}

UINode::~UINode()
{
	m_Children.clear();
}

cstring UINode::GetName() const
{
	return m_Name;
}

void UINode::SetName(cstring name)
{
	m_Name = name;
}

// Translate
void UINode::setTranslate(cvec2 _translate) 
{ 
	m_Translate = _translate;
	m_IsLocalDirty = true;
	m_IsWorldDirty = true;
}
cvec2 UINode::getTranslate() const 
{ 
	return m_Translate; 
}

// Rotate
void UINode::setRotate(cvec3 _rotate) 
{ 
	m_Rotate = _rotate;
	m_IsLocalDirty = true;
	m_IsWorldDirty = true;
}
cvec3 UINode::getRotate() const 
{ 
	return m_Rotate; 
}

// Scale
void UINode::setScale(cvec2 _scale) 
{ 
	m_Scale = _scale;
	m_IsLocalDirty = true;
	m_IsWorldDirty = true;
}
cvec2 UINode::getScale() const 
{ 
	return m_Scale; 
}

bool UINode::IsDirty() const
{
	return m_IsLocalDirty || m_IsWorldDirty;
}

// Local transform

mat4 UINode::GetLocalTransform()
{
	UpdateLocalTransform();
	return m_LocalTransform;
}

void UINode::SetLocalTransform(cmat4 localTransform)
{
	m_LocalTransform = localTransform;
	m_IsLocalDirty = true;
	m_IsWorldDirty = true;
}

// World transform

mat4 UINode::GetWorldTransfom()
{
	UpdateWorldTransform();
	return m_WorldTransform;
}

void UINode::SetWorldTransform(cmat4 worldTransform)
{
	mat4 inverseParentTransform = glm::inverse(GetParentWorldTransform());
	SetLocalTransform(inverseParentTransform * worldTransform);
}


mat4 UINode::GetParentWorldTransform() const
{
	mat4 parentTransform(1.0f);
	if (std::shared_ptr<UINode> parent = m_pParentNode.lock())
	{
		parentTransform = parent->GetWorldTransfom();
	}

	return parentTransform;
}

//

void UINode::UpdateLocalTransform()
{
	if (m_IsLocalDirty)
	{
		m_LocalTransform = mat4();

		m_LocalTransform = glm::translate(m_LocalTransform, vec3(m_Translate, 0.0f));
		m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.x, vec3(1, 0, 0));
		m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.y, vec3(0, 1, 0));
		m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.z, vec3(0, 0, 1));
		m_LocalTransform = glm::scale(m_LocalTransform, vec3(m_Scale, 1.0f));
		m_IsLocalDirty = false;
		m_IsWorldDirty = true;
	}
}

void UINode::UpdateWorldTransform()
{
	if (m_IsWorldDirty)
	{
		m_WorldTransform = GetParentWorldTransform() * m_LocalTransform;
		m_IsWorldDirty = false;
	}
}

void UINode::SetLocalUnderty()
{
	m_IsLocalDirty = false;
}

void UINode::AddChild(std::shared_ptr<UINode> pNode)
{
	if (pNode)
	{
		NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), pNode);
		if (iter == m_Children.end())
		{
			pNode->m_pParentNode = shared_from_this();
			pNode->m_IsWorldDirty = true;

			m_Children.push_back(pNode);
			if (!pNode->GetName().empty())
			{
				m_ChildrenByName.insert(NodeNameMap::value_type(pNode->GetName(), pNode));
			}
		}
	}
}

void UINode::RemoveChild(std::shared_ptr<UINode> pNode)
{
	if (pNode)
	{
		NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), pNode);
		if (iter != m_Children.end())
		{
			pNode->SetParent(std::weak_ptr<UINode>());

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

void UINode::SetParent(std::weak_ptr<UINode> wpNode)
{
	// Parents own their children.. If this node is not owned
	// by anyone else, it will cease to exist if we remove it from it's parent.
	// As a precaution, store myself is a shared pointer so I don't get deleted
	// half-way through this function!
	// Technically self deletion shouldn't occur because the thing invoking this function
	// should have a std::shared_ptr to it.
	std::shared_ptr<UINode> me = shared_from_this();

	if (std::shared_ptr<UINode> parent = wpNode.lock())
	{
		parent->AddChild(me);
	}
	else if (parent = m_pParentNode.lock())
	{
		// Setting parent to NULL.. remove from current parent and reset parent node.
		//mat4 worldTransform = GetWorldTransfom();
		parent->RemoveChild(me);
		m_pParentNode.reset();
		//SetLocalTransform(worldTransform);
	}
}

void UINode::AddMesh(std::shared_ptr<IMesh> mesh)
{
	assert(mesh);
	MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
	if (iter == m_Meshes.end())
	{
		m_Meshes.push_back(mesh);
	}
}

void UINode::RemoveMesh(std::shared_ptr<IMesh> mesh)
{
	assert(mesh);
	MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
	if (iter != m_Meshes.end())
	{
		m_Meshes.erase(iter);
	}
}

void UINode::Update(Viewport* viewport)
{
	// Do nothing...
}

bool UINode::Accept(IVisitor& visitor)
{
	bool visitResult = visitor.Visit(*this);

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

	return visitResult;
}
