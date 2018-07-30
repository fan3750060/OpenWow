#pragma once

class Mesh;
class Camera;
class RenderEventArgs;
class Visitor;

class SceneNode : public Object, public std::enable_shared_from_this<SceneNode>
{
public:
	typedef Object base;

	explicit SceneNode(cmat4 localTransform = mat4(1.0f));
	virtual ~SceneNode();

	/**
	 * Assign a name to this scene node so that it can be searched for later.
	 */
	cstring GetName() const;
	void SetName(cstring name);

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
	void AddChild(std::shared_ptr<SceneNode> pNode);
	void RemoveChild(std::shared_ptr<SceneNode> pNode);
	void SetParent(std::weak_ptr<SceneNode> pNode);

	/**
	 * Add a mesh to this scene node.
	 * The scene node does not take ownership of a mesh that is set on a mesh
	 * as it is possible that the same mesh is added to multiple scene nodes.
	 * Deleting the scene node does not delete the meshes associated with it.
	 */
	void AddMesh(std::shared_ptr<Mesh> mesh);
	void RemoveMesh(std::shared_ptr<Mesh> mesh);

	/**
	 * Render meshes associated with this scene node.
	 * This method will traverse it's children.
	 */
	void Render(RenderEventArgs& renderEventArgs);

	/**
	 * Allow a visitor to visit this node.
	 */
	void Accept(IVisitor& visitor);

protected:
	mat4 GetParentWorldTransform() const;

private:
	typedef std::vector< std::shared_ptr<SceneNode> > NodeList;
	typedef std::multimap< std::string, std::shared_ptr<SceneNode> > NodeNameMap;
	typedef std::vector< std::shared_ptr<Mesh> > MeshList;

	std::string m_Name;

	// Transforms node from parent's space to world space for rendering.
	mat4 m_LocalTransform;
	// This is the inverse of the local -> world transform.
	mat4 m_InverseTransform;

	std::weak_ptr<SceneNode> m_pParentNode;
	NodeList m_Children;
	NodeNameMap m_ChildrenByName;
	MeshList m_Meshes;
};