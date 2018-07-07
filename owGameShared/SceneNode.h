#pragma once

class SceneNode :
	public CRefItem,
	public ILoadable,
	public IUpdatable,
	public IRenderable3D
{
public:
	SceneNode();
	SceneNode(SceneNode* _parent);
	virtual ~SceneNode();

	vector<SceneNode*>& getChilds() { return m_Childs; }

	// ISceneNode
	void setParent(SceneNode* _parent) { m_Parent = _parent; }
	SceneNode* getParent() { return m_Parent; }
	void addChild(SceneNode* _child)
	{
		assert1(std::find(m_Childs.begin(), m_Childs.end(), this) == m_Childs.end());
		m_Childs.push_back(_child);
	}
	void removeChild(SceneNode* _child)
	{
		if (m_Childs.empty())
		{
			return;
		}

		assert1(std::find(m_Childs.begin(), m_Childs.end(), _child) != m_Childs.end());
		m_Childs.erase(std::remove(m_Childs.begin(), m_Childs.end(), _child), m_Childs.end());
	}

	void setTranslate(vec3 _translate) { m_Translate = _translate; CalculateMatrix(); }
	cvec3 getTranslate() const { return m_Translate; }
	void setRotate(vec3 _rotate) { m_Rotate = _rotate; CalculateMatrix(); }
	cvec3 getRotate() const { return m_Rotate; }
	cquat getRotateQuat() const { return m_RotateQuat; }
	cvec3 getScale() const { return m_Scale; }
	void setBounds(BoundingBox _bbox) { m_Bounds = _bbox; }
	cbbox getBounds() const { return m_Bounds; }
	cmat4 getRelTrans() const { return m_RelTransform; }
	cmat4 getAbsTrans() const { return m_AbsTransform; }

	// ISceneNode::Selectable
	void setSelectable() { m_Selectable = true; }
	bool getSelectable() { return m_Selectable; }
	virtual string getObjectInfo() { return "emp"; };

	// ILoadable
	virtual bool Load() override;
	virtual bool Delete() override;
	void setLoaded() override { m_IsLoaded = true; }
	bool isLoaded() const override { return m_IsLoaded; }

	// IUpdatable
	virtual void Input(CInput* _input, double _time, double _dTime) override {};
	virtual void Update(double _time, double _dTime) override {};

	// IRenderable
	virtual bool PreRender3D() override { return true; };
	virtual void Render3D() override { /*override me*/ }
	virtual void PostRender3D() override { /* override me*/ }
	virtual void RenderDebug3D() { Render3D(); };
	void setVisible(bool _value) override { m_IsVisible = _value; }
	bool isVisible() const override { return m_IsVisible; }
	void setOpaque(bool _value) override { m_IsOpaque = _value; }
	bool isOpaque() const override { return m_IsOpaque; }
	void setDrawOrder(uint32 _order) override { m_DrawOrder = _order; _Bindings->m_Renderable3DObjectCollection->SetNeedSort(); }
	uint32 getDrawOrder() const override { return m_DrawOrder; }
	void setDebugColor(vec4 _value) { m_DebugColor = _value; }
	vec4 getDebugColor() const { return m_DebugColor; }

protected:
	void CalculateMatrix(bool _isRotationQuat = false);

protected:
	SceneNode * m_Parent;
	vector<SceneNode*>  m_Childs;

	vec3                m_Translate;
	vec3				m_Rotate;
	quat				m_RotateQuat;
	vec3                m_Scale;
	BoundingBox         m_Bounds;

private:
	mat4				m_RelTransform;
	mat4				m_AbsTransform;
	bool				m_Selectable;

private: // ILoadable
	bool				m_IsLoaded;

private: // IRenderable
	bool				m_IsVisible;
	bool				m_IsOpaque;
	uint32				m_DrawOrder;
	vec4				m_DebugColor;
};

class SceneNodeCompare
{
public:
	SceneNodeCompare(Camera* _camera) :
		m_Camera(_camera)
	{}

	/*bool operator() (const SceneNode* left, const SceneNode* right) const
	{
		if (left->getDrawOrder() < right->getDrawOrder())
		{
			return true;
		}
		else if (left->getDrawOrder() > right->getDrawOrder())
		{
			return false;
		}
		else
		{
			float distToCameraLeft = (m_Camera->Position - left->getBounds().getCenter()).length();
			float distToCameraRight = (m_Camera->Position - right->getBounds().getCenter()).length();
			return distToCameraLeft > distToCameraRight;
		}
	}*/

	bool operator() (const SceneNode* left, const SceneNode* right) const
	{
		if (left->isOpaque() && right->isOpaque())
		{
			return left->getDrawOrder() < right->getDrawOrder();
		}
		else if (!(left->isOpaque()) && right->isOpaque())
		{
			return false;
		}
		else if (left->isOpaque() && !(right->isOpaque()))
		{
			return true;
		}
		else if (!(left->isOpaque()) && !(right->isOpaque()))
		{
			if (left->getDrawOrder() < right->getDrawOrder())
			{
				return true;
			}
			else if (left->getDrawOrder() > right->getDrawOrder())
			{
				return false;
			}
			else
			{
				float distToCameraLeft = (m_Camera->Position - left->getBounds().getCenter()).length() - left->getBounds().getRadius();
				float distToCameraRight = (m_Camera->Position - right->getBounds().getCenter()).length() - right->getBounds().getRadius();
				return distToCameraLeft > distToCameraRight;
			}
		}
	}

private:
	Camera * m_Camera;
};