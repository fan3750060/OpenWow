#pragma once

class IMesh;
class Camera;
class RenderEventArgs;
class IVisitor;

class SceneNode : public Object, public std::enable_shared_from_this<SceneNode>
{
	typedef Object base;
public:
	explicit SceneNode(cmat4 localTransform = mat4(1.0f));
	virtual ~SceneNode();

	/**
	 * Assign a name to this scene node so that it can be searched for later.
	 */
	virtual cstring GetName() const;
	virtual void SetName(cstring name);

	// Translate
	void setTranslate(cvec3 _translate, bool _calculateMatrix = true) { if (m_Translate == _translate) return; m_Translate = _translate; if (_calculateMatrix) CalculateLocalTransform(); }
	cvec3 getTranslate() const { return m_Translate; }

	// Rotate
	void setRotate(cvec3 _rotate, bool _calculateMatrix = true) { if (m_Rotate == _rotate) return; m_Rotate = _rotate; if (_calculateMatrix) CalculateLocalTransform(); }
	cvec3 getRotate() const { return m_Rotate; }

	// Rotate Quaternion
	void setRotateQuat(cquat _rotate, bool _calculateMatrix = true) { m_RotateQuat = _rotate; if (_calculateMatrix) CalculateLocalTransform(true); }
	cquat getRotateQuat() const { return m_RotateQuat; }

	// Scale
	void setScale(cvec3 _scale, bool _calculateMatrix = true) { if (m_Scale == _scale) return; m_Scale = _scale; if (_calculateMatrix) CalculateLocalTransform(); }
	cvec3 getScale() const { return m_Scale; }

	// Bounds
	void setBounds(BoundingBox _bbox) { m_Bounds = _bbox; }
	cbbox getBounds() const { return m_Bounds; }

	/**
	 * Gets the scene node's local transform (relative to it's parent world transform).
	 */
	mat4 GetLocalTransform() const;
	void SetLocalTransform(cmat4 localTransform);

	/**
	 * Gets the inverse of the local transform (relative to it's parent world transform).
	 */
	mat4 GetInverseLocalTransform() const;

	/**
	 * Gets the scene node's world transform (concatenated with parents world transform)
	 * This function should be used sparingly as it is computed every time
	 * it is requested.
	 */
	mat4 GetWorldTransfom() const;
	void SetWorldTransform(cmat4 worldTransform);

	/**
	 * Gets the inverse world transform of this scene node.
	 * Use this function sparingly as it is computed every time it is requested.
	 */
	mat4 GetInverseWorldTransform() const;

	/**
	 * Add a child node to this node.
	 * NOTE: Circular references are not checked!
	 * A scene node takes ownership of it's children.
	 * If you delete the parent node, all of its children will also be deleted.
	 */
	virtual void AddChild(std::shared_ptr<SceneNode> pNode);
	virtual void RemoveChild(std::shared_ptr<SceneNode> pNode);
	virtual void SetParent(std::weak_ptr<SceneNode> pNode);

	/**
	 * Add a mesh to this scene node.
	 * The scene node does not take ownership of a mesh that is set on a mesh
	 * as it is possible that the same mesh is added to multiple scene nodes.
	 * Deleting the scene node does not delete the meshes associated with it.
	 */
	virtual void AddMesh(std::shared_ptr<IMesh> mesh);
	virtual void RemoveMesh(std::shared_ptr<IMesh> mesh);

	/**
	 * Allow a visitor to visit this node.
	 */
	virtual void Accept(IVisitor& visitor);

protected:
	virtual mat4 GetParentWorldTransform() const;
	virtual void CalculateLocalTransform(bool _isRotationQuat = false);

private:
	typedef std::vector<std::shared_ptr<SceneNode>> NodeList;
	typedef std::multimap<std::string, std::shared_ptr<SceneNode>> NodeNameMap;
	typedef std::vector<std::shared_ptr<IMesh>> MeshList;

	std::string         m_Name;
	SceneNodeTypes      m_Type;

	// Transforms node from parent's space to world space for rendering.
	mat4                m_LocalTransform;
	vec3                m_Translate;
	vec3				m_Rotate;
	quat				m_RotateQuat;
	vec3                m_Scale;
	BoundingBox         m_Bounds;
	// This is the inverse of the local -> world transform.
	mat4                m_InverseTransform;

	std::weak_ptr<SceneNode>  m_pParentNode;
	NodeList                  m_Children;
	NodeNameMap               m_ChildrenByName;
	MeshList                  m_Meshes;
};