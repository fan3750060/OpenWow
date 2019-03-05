#pragma once

class CUIBaseNode : public Object, public std::enable_shared_from_this<CUIBaseNode>
{
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

	mat4 GetLocalTransform() const;
	void SetLocalTransform(cmat4 localTransform);

	mat4 GetWorldTransfom() const;
	void SetWorldTransform(cmat4 worldTransform);


	virtual void AddChild(std::shared_ptr<CUIBaseNode> pNode);
	virtual void RemoveChild(std::shared_ptr<CUIBaseNode> pNode);
	virtual void SetParent(std::weak_ptr<CUIBaseNode> pNode);

	virtual void SetMesh(std::shared_ptr<IMesh> mesh);
	virtual std::shared_ptr<IMesh> GetMesh() const;

	// Called before all others calls
	virtual void UpdateViewport(const Viewport* viewport);

	//Render this node and it's childs
	virtual bool Render(RenderUIEventArgs& renderEventArgs);
	virtual bool RenderMesh(RenderUIEventArgs& renderEventArgs);

	// Input events
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
		glm::mat4 ModelViewProjection;
		glm::mat4 ModelView;
		glm::mat4 Model;
	};
	PerObject* m_PerObjectData;
	std::shared_ptr<ConstantBuffer> m_PerObjectConstantBuffer;

private:
	typedef std::vector<std::shared_ptr<CUIBaseNode>> NodeList;
	typedef std::multimap<std::string, std::shared_ptr<CUIBaseNode>> NodeNameMap;
	typedef std::vector<std::shared_ptr<IMesh>> MeshList;

	std::string                 m_Name;

	vec2                        m_Translate;
	vec3				        m_Rotate;
	vec2                        m_Scale;

	mat4                        m_LocalTransform;
	mat4                        m_WorldTransform;

	std::weak_ptr<CUIBaseNode>	m_pParentNode;
	NodeList				    m_Children;
	NodeNameMap				    m_ChildrenByName;
	std::shared_ptr<IMesh>      m_Mesh;
};