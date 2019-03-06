#include "stdafx.h"

// General
#include "UIBaseNode.h"

// Additional
#include "UIWindow.h"
#include "Application.h"

CUIBaseNode::CUIBaseNode()
	: m_Name("CUIBaseNode")
	, m_Translate(vec2())
	, m_Rotate(vec3())
	, m_Scale(1.0f, 1.0f)
	, m_IsMouseOnNode(false)
{
}

CUIBaseNode::~CUIBaseNode()
{
}

cstring CUIBaseNode::GetName() const
{
	return m_Name;
}

void CUIBaseNode::SetName(cstring name)
{
	m_Name = name;
}

// Translate
void CUIBaseNode::SetTranslate(cvec2 _translate) 
{ 
	m_Translate = _translate;

	UpdateLocalTransform();
}
cvec2 CUIBaseNode::GetTranslation() const 
{ 
	return m_Translate; 
}

// Rotate
void CUIBaseNode::SetRotation(cvec3 _rotate) 
{ 
	m_Rotate = _rotate;

	UpdateLocalTransform();
}
cvec3 CUIBaseNode::GetRotation() const 
{ 
	return m_Rotate; 
}

// Scale
void CUIBaseNode::SetScale(cvec2 _scale) 
{ 
	m_Scale = _scale;

	UpdateLocalTransform();
}
cvec2 CUIBaseNode::GetScale() const 
{ 
	return m_Scale; 
}

Rect CUIBaseNode::GetBounds() const
{
	return Rect(m_Translate.x, m_Translate.y, m_Scale.x, m_Scale.y);
}

bool CUIBaseNode::IsPointInBounds(glm::vec2 Point) const
{
	Rect rect = GetBounds();

	return (Point.x >= rect.X) && (Point.x < (rect.X + rect.Width)) && (Point.y >= rect.Y) && (Point.y < (rect.Y + rect.Height));
}

// Local transform

mat4 CUIBaseNode::GetLocalTransform() const
{
	return m_LocalTransform;
}

void CUIBaseNode::SetLocalTransform(cmat4 localTransform)
{
	m_LocalTransform = localTransform;

	UpdateWorldTransform();
}

// World transform

mat4 CUIBaseNode::GetWorldTransfom() const
{
	return m_WorldTransform;
}

void CUIBaseNode::SetWorldTransform(cmat4 worldTransform)
{
	mat4 inverseParentTransform = glm::inverse(GetParentWorldTransform());
	SetLocalTransform(inverseParentTransform * worldTransform);
}


void CUIBaseNode::SetParent(std::weak_ptr<CUIBaseNode> parentNode)
{
	std::shared_ptr<CUIBaseNode> me = shared_from_this();

	// Delete from current parent
	if (std::shared_ptr<CUIBaseNode> currentParent = m_pParentNode.lock())
	{
		if (std::shared_ptr<CUIWindowNode> currentParentAsWindow = std::dynamic_pointer_cast<CUIWindowNode, CUIBaseNode>(currentParent))
		{
			currentParentAsWindow->RemoveChild(me);
			m_pParentNode.reset();
		}
	}

	if (std::shared_ptr<CUIBaseNode> parent = parentNode.lock())
	{
		std::shared_ptr<CUIWindowNode> parentAsWindow = std::dynamic_pointer_cast<CUIWindowNode, CUIBaseNode>(parent);
		if (parentAsWindow)
		{
			parentAsWindow->AddChild(me);
		}
		else
		{
			fail2("Only 'CUIWindowNode' can be parent of any ui node.");
		}
	}
}

void CUIBaseNode::SetParentInternal(std::weak_ptr<CUIBaseNode> parent)
{
	m_pParentNode = parent;
	UpdateWorldTransform();
}


mat4 CUIBaseNode::GetParentWorldTransform() const
{
	if (std::shared_ptr<CUIBaseNode> parent = m_pParentNode.lock())
	{
		return parent->GetWorldTransfom();
	}

	return mat4(1.0f);
}

//

void CUIBaseNode::UpdateLocalTransform()
{
	m_LocalTransform = mat4();

	m_LocalTransform = glm::translate(m_LocalTransform, vec3(m_Translate, 0.0f));
	m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.x, vec3(1, 0, 0));
	m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.y, vec3(0, 1, 0));
	m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.z, vec3(0, 0, 1));
	m_LocalTransform = glm::scale(m_LocalTransform, vec3(m_Scale, 1.0f));

	UpdateWorldTransform();
}

void CUIBaseNode::UpdateWorldTransform()
{
	m_WorldTransform = GetParentWorldTransform() * m_LocalTransform;
}

void CUIBaseNode::SetMesh(std::shared_ptr<IMesh> mesh)
{
	assert1(mesh != nullptr);
	m_Mesh = mesh;
}

std::shared_ptr<IMesh> CUIBaseNode::GetMesh() const
{
	return m_Mesh;
}

void CUIBaseNode::UpdateViewport(const Viewport* viewport)
{
	// Do nothing...
}



//
// Render functional
//
bool CUIBaseNode::Accept(IVisitor& visitor)
{
	bool visitResult = visitor.Visit(*this);
	if (!visitResult)
		return false;

	// Visit meshes
	AcceptMesh(visitor);

	return visitResult;
}

bool CUIBaseNode::AcceptMesh(IVisitor & visitor)
{
	std::shared_ptr<IMesh> mesh = GetMesh();
	if (mesh != nullptr)
	{
		return mesh->Accept(visitor);
	}

	return false;
}



//
// Input events
//
bool CUIBaseNode::OnKeyPressed(KeyEventArgs & e)
{
	return false;
}

void CUIBaseNode::OnKeyReleased(KeyEventArgs & e)
{
	// Do nothing
}

void CUIBaseNode::OnMouseMoved(MouseMotionEventArgs& e)
{
	// Do nothing
}

bool CUIBaseNode::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	return false;
}

void CUIBaseNode::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	// Do nothing
}

bool CUIBaseNode::OnMouseWheel(MouseWheelEventArgs & e)
{
	return false;
}



//
// Syntetic events
//
void CUIBaseNode::OnMouseEntered()
{
	// Do nothing
}

void CUIBaseNode::OnMouseLeaved()
{
	// Do nothing
}



//
// Syntetic events PRIVATE
//
bool CUIBaseNode::IsMouseOnNode() const
{
	return m_IsMouseOnNode;
}

void CUIBaseNode::DoMouseEntered()
{
	m_IsMouseOnNode = true;
}

void CUIBaseNode::DoMouseLeaved()
{
	m_IsMouseOnNode = false;
}