#include "stdafx.h"

// General
#include "SceneNode3D.h"

SceneNode3D::SceneNode3D(cmat4 localTransform)
	: m_LocalTransform(localTransform)
	, m_Name("SceneNode3D")
	, m_Type(SN_TYPE_NONE)
	, m_Translate(vec3())
	, m_Rotate(vec3())
	, m_RotateQuat(quat())
	, m_IsRotateQuat(false)
	, m_Scale(1.0f, 1.0f, 1.0f)
{
	m_InverseLocalTransform = glm::inverse(m_LocalTransform);
}

SceneNode3D::~SceneNode3D()
{
	m_Children.clear();
}

cstring SceneNode3D::GetName() const
{
	return m_Name;
}

void SceneNode3D::SetName(cstring name)
{
	m_Name = name;
}

// Translate
void SceneNode3D::SetTranslate(cvec3 _translate)
{
	m_Translate = _translate;

	UpdateLocalTransform();
}
cvec3 SceneNode3D::GetTranslation() const
{
	return m_Translate;
}

// Rotate
void SceneNode3D::SetRotation(cvec3 _rotate)
{
	m_Rotate = _rotate;

	UpdateLocalTransform();
}
cvec3 SceneNode3D::GetRotation() const
{
	return m_Rotate;
}

// Rotate Quaternion
void SceneNode3D::SetRotationQuaternion(cquat _rotate)
{
	m_RotateQuat = _rotate;
	m_IsRotateQuat = true;

	UpdateLocalTransform();
}
cquat SceneNode3D::GetRotationQuaternion() const
{
	return m_RotateQuat;
}

// Scale
void SceneNode3D::SetScale(cvec3 _scale)
{
	m_Scale = _scale;

	UpdateLocalTransform();
}
cvec3 SceneNode3D::GetScale() const
{
	return m_Scale;
}

// Bounds
void SceneNode3D::SetBounds(BoundingBox _bbox)
{
	m_Bounds = _bbox;
}
cbbox SceneNode3D::GetBounds() const
{
	return m_Bounds;
}

// Local transform

mat4 SceneNode3D::GetLocalTransform() const
{
	return m_LocalTransform;
}

mat4 SceneNode3D::GetInverseLocalTransform() const
{
	return m_InverseLocalTransform;
}

void SceneNode3D::SetLocalTransform(cmat4 localTransform)
{
	m_LocalTransform = localTransform;
	m_InverseLocalTransform = glm::inverse(localTransform);

	UpdateWorldTransform();
}

// World transform

mat4 SceneNode3D::GetWorldTransfom() const
{
	return m_WorldTransform;
}

mat4 SceneNode3D::GetInverseWorldTransform() const
{
	return m_InverseWorldTransform;
}

void SceneNode3D::SetWorldTransform(cmat4 worldTransform)
{
	mat4 inverseParentTransform = glm::inverse(GetParentWorldTransform());
	SetLocalTransform(inverseParentTransform * worldTransform);
}


mat4 SceneNode3D::GetParentWorldTransform() const
{
	mat4 parentTransform(1.0f);
	if (std::shared_ptr<SceneNode3D> parent = m_pParentNode.lock())
	{
		parentTransform = parent->GetWorldTransfom();
	}

	return parentTransform;
}

//

void SceneNode3D::UpdateLocalTransform()
{
	m_LocalTransform = mat4();

	m_LocalTransform = glm::translate(m_LocalTransform, m_Translate);
	if (m_IsRotateQuat)
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
	m_InverseLocalTransform = glm::inverse(m_LocalTransform);

	UpdateWorldTransform();
}

void SceneNode3D::UpdateWorldTransform()
{
	m_WorldTransform = GetParentWorldTransform() * m_LocalTransform;
	m_InverseWorldTransform = glm::inverse(m_WorldTransform);
}

void SceneNode3D::AddChild(std::shared_ptr<SceneNode3D> childNode)
{
	if (childNode)
	{
		NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), childNode);
		if (iter == m_Children.end())
		{
			childNode->m_pParentNode = shared_from_this();
			childNode->UpdateWorldTransform();

			m_Children.push_back(childNode);
			if (!childNode->GetName().empty())
				m_ChildrenByName.insert(NodeNameMap::value_type(childNode->GetName(), childNode));
		}
	}
}

void SceneNode3D::RemoveChild(std::shared_ptr<SceneNode3D> childNode)
{
	if (childNode)
	{
		NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), childNode);
		if (iter != m_Children.end())
		{
			childNode->SetParent(std::weak_ptr<SceneNode3D>());
			childNode->UpdateWorldTransform();

			m_Children.erase(iter);
			NodeNameMap::iterator iter2 = m_ChildrenByName.find(childNode->GetName());
			if (iter2 != m_ChildrenByName.end())
				m_ChildrenByName.erase(iter2);
		}
		else
		{
			// Maybe this node appears lower in the hierarchy...
			for (auto child : m_Children)
			{
				child->RemoveChild(childNode);
			}
		}
	}
}

void SceneNode3D::SetParent(std::weak_ptr<SceneNode3D> parentNode)
{
	// Remove from current parent
	std::shared_ptr<SceneNode3D> currentParent = m_pParentNode.lock();
	if (currentParent != nullptr)
	{
		currentParent->RemoveChild(shared_from_this());
		m_pParentNode.reset();
	}

	// Add to new parent
	if (std::shared_ptr<SceneNode3D> newParent = parentNode.lock())
		newParent->AddChild(SceneNode3D::shared_from_this());
}

void SceneNode3D::AddMesh(std::shared_ptr<IMesh> mesh)
{
	assert(mesh);
	MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
	if (iter == m_Meshes.end())
		m_Meshes.push_back(mesh);
}

void SceneNode3D::RemoveMesh(std::shared_ptr<IMesh> mesh)
{
	assert(mesh);
	MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
	if (iter != m_Meshes.end())
		m_Meshes.erase(iter);
}

void SceneNode3D::UpdateCamera(const Camera* camera)
{
	// Do nothing...
}

bool SceneNode3D::Accept(IVisitor& visitor)
{
	bool visitResult = visitor.Visit(*this);
	if (!visitResult)
		return false;

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

void SceneNode3D::OnUpdate(UpdateEventArgs & e)
{
}

bool SceneNode3D::checkFrustum(const Camera* _camera) const
{
	assert1(_camera != nullptr);
	return !_camera->GetFrustum().cullBox(GetBounds());
}

bool SceneNode3D::checkDistance2D(cvec3 _camPos, float _distance) const
{
	// Check distance to camera
	float distToCamera2D = glm::length(Fix_X0Z(_camPos) - Fix_X0Z(GetBounds().getCenter())) - GetBounds().getRadius();
	return distToCamera2D < _distance;
}

bool SceneNode3D::checkDistance(cvec3 _camPos, float _distance) const
{
	// Check distance to camera
	float distToCamera = glm::length(_camPos - GetBounds().getCenter()) - GetBounds().getRadius();
	return distToCamera < _distance;
}