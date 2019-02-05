#pragma once

class IMesh;
class Camera;
class Render3DEventArgs;
class IVisitor;

class SceneNodeUI : public Object, public std::enable_shared_from_this<SceneNodeUI>
{
	typedef Object base;
public:
	explicit SceneNodeUI();
	virtual ~SceneNodeUI();

	/**
	 * Assign a name to this scene node so that it can be searched for later.
	 */
	virtual cstring GetName() const;
	virtual void SetName(cstring name);

	// Translate
	void SetTranslate(cvec2 _translate);
	cvec2 GetTranslation() const;

	// Rotate
	void SetRotation(cvec3 _rotate);
	cvec3 GetRotation() const;

	// Scale
	void SetScale(cvec2 _scale);
	cvec2 GetScale() const;

	/**
	 * Gets the scene node's local transform (relative to it's parent world transform).
	 */
	mat4 GetLocalTransform() const;
	/**
	 * Gets the inverse of the local transform (relative to it's parent world transform).
	 */
	void SetLocalTransform(cmat4 localTransform);



	/**
	 * Gets the scene node's world transform (concatenated with parents world transform)
	 * This function should be used sparingly as it is computed every time
	 * it is requested.
	 */
	mat4 GetWorldTransfom() const;
	/**
	 * Gets the inverse world transform of this scene node.
	 * Use this function sparingly as it is computed every time it is requested.
	 */
	void SetWorldTransform(cmat4 worldTransform);



	/**
	 * Add a child node to this node.
	 * NOTE: Circular references are not checked!
	 * A scene node takes ownership of it's children.
	 * If you delete the parent node, all of its children will also be deleted.
	 */
	virtual void AddChild(std::shared_ptr<SceneNodeUI> pNode);
	virtual void RemoveChild(std::shared_ptr<SceneNodeUI> pNode);
	virtual void SetParent(std::weak_ptr<SceneNodeUI> pNode);

	/**
	 * Set a mesh to this ui node.
	 * The ui node does not take ownership of a mesh that is set on a mesh
	 * as it is possible that the same mesh is added to multiple scene nodes.
	 */
	virtual void SetMesh(std::shared_ptr<IMesh> mesh);
	virtual std::shared_ptr<IMesh> GetMesh() const;

	/**
	 * Called before all others calls
	 */
	virtual void UpdateViewport(const Viewport* viewport);

	/**
	 * Render this node and it's childs
	 */
	virtual bool Render(RenderUIEventArgs& renderEventArgs);
	virtual bool RenderMesh(RenderUIEventArgs& renderEventArgs);

	/**
	 * Input events
	 */
	bool OnKeyPressed(KeyEventArgs& e);
	bool OnKeyReleased(KeyEventArgs& e);

	void OnMouseMoved(MouseMotionEventArgs& e);
	bool OnMouseButtonPressed(MouseButtonEventArgs& e);
	bool OnMouseButtonReleased(MouseButtonEventArgs& e);
	bool OnMouseWheel(MouseWheelEventArgs& e);

protected:
	virtual mat4 GetParentWorldTransform() const;

	virtual void UpdateLocalTransform();
	virtual void UpdateWorldTransform();

	// PerObject constant buffer data.
	__declspec(align(16)) struct PerObject
	{
		glm::mat4 ModelOrtho;
		glm::mat4 Model;
	};
	PerObject* m_PerObjectData;
	std::shared_ptr<ConstantBuffer> m_PerObjectConstantBuffer;

private:
	typedef std::vector<std::shared_ptr<SceneNodeUI>> NodeList;
	typedef std::multimap<std::string, std::shared_ptr<SceneNodeUI>> NodeNameMap;
	typedef std::vector<std::shared_ptr<IMesh>> MeshList;

	std::string         m_Name;

	// Transforms node from parent's space to world space for rendering.
	mat4                m_LocalTransform;
	mat4                m_WorldTransform;

	vec2                m_Translate;
	vec3				m_Rotate;
	vec2                m_Scale;


	std::weak_ptr<SceneNodeUI>	m_pParentNode;
	NodeList				m_Children;
	NodeNameMap				m_ChildrenByName;
	std::shared_ptr<IMesh>  m_Mesh;
};