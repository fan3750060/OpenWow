#pragma once

class CUIWindowNode;

class CUIBaseNode : public Object, public std::enable_shared_from_this<CUIBaseNode>
{
	friend CUIWindowNode;
	typedef Object base;
public:
	explicit CUIBaseNode();
	virtual ~CUIBaseNode();

	virtual cstring GetName() const;
	virtual void SetName(cstring name);

	void SetTranslate(cvec2 _translate);
	cvec2 GetTranslation() const;

	void SetRotation(cvec3 _rotate);
	cvec3 GetRotation() const;

	void SetScale(cvec2 _scale);
	cvec2 GetScale() const;

	// Bounds functional
	Rect GetBounds() const;
	bool IsPointInBounds(glm::vec2 Point) const;


	mat4 GetLocalTransform() const;
	void SetLocalTransform(cmat4 localTransform);

	mat4 GetWorldTransfom() const;
	void SetWorldTransform(cmat4 worldTransform);

	// Parent & childs functional
	virtual void SetParent(std::weak_ptr<CUIBaseNode> parent);
	virtual void SetParentInternal(std::weak_ptr<CUIBaseNode> parent);

	// Meshes
	virtual void SetMesh(std::shared_ptr<IMesh> mesh);
	virtual std::shared_ptr<IMesh> GetMesh() const;

	// Called before all others calls
	virtual void UpdateViewport(const Viewport* viewport);

	// Allow a visitor to visit this node. 
	virtual bool Accept(IVisitor& visitor);
	virtual bool AcceptMesh(IVisitor& visitor);

	// Input events
	virtual bool OnKeyPressed(KeyEventArgs& e);
	virtual void OnKeyReleased(KeyEventArgs& e);
	virtual void OnMouseMoved(MouseMotionEventArgs& e);
	virtual bool OnMouseButtonPressed(MouseButtonEventArgs& e);
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e);
	virtual bool OnMouseWheel(MouseWheelEventArgs& e);

	// Syntetic events
	virtual void OnMouseEntered();
	virtual void OnMouseLeaved();

protected:
	virtual mat4 GetParentWorldTransform() const;

	virtual void UpdateLocalTransform();
	virtual void UpdateWorldTransform();

private: // Syntetic events
	bool IsMouseOnNode() const;
	void DoMouseEntered();
	void DoMouseLeaved();

private:
	typedef std::vector<std::shared_ptr<IMesh>> MeshList;

	std::string                 m_Name;

	vec2                        m_Translate;
	vec3				        m_Rotate;
	vec2                        m_Scale;

	mat4                        m_LocalTransform;
	mat4                        m_WorldTransform;

	std::weak_ptr<CUIBaseNode>	m_pParentNode;

	std::shared_ptr<IMesh>      m_Mesh;

private: // Syntetic events
	bool                        m_IsMouseOnNode;
};