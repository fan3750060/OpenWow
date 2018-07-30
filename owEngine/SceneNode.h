#pragma once

class SceneNode :
	public IUpdatable,
	public IRenderable3D
{
public:
	SceneNode(SceneNode* _parent = nullptr);
	virtual ~SceneNode();

	// ISceneNode
	void setParent(SceneNode* _parent) { m_Parent = _parent; }
	const SceneNode* getParent() { return m_Parent; }
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
	vector<SceneNode*>& getChilds() { return m_Childs; }

	// Translate
	void setTranslate(cvec3 _translate, bool _calculateMatrix = true) 
	{ 
		if (m_Translate == _translate) return;
		m_Translate = _translate; 
		if (_calculateMatrix) CalculateMatrix();
	}
	cvec3 getTranslate() const { return m_Translate; }

	// Rotate
	void setRotate(cvec3 _rotate, bool _calculateMatrix = true)
	{ 
		if (m_Rotate == _rotate) return;
		m_Rotate = _rotate; 
		if (_calculateMatrix) CalculateMatrix();
	}
	cvec3 getRotate() const { return m_Rotate; }
	
	// Rotate Quaternion
	void setRotateQuat(cquat _rotate, bool _calculateMatrix = true)
	{
		m_RotateQuat = _rotate;
		if (_calculateMatrix) CalculateMatrix();
	}
	cquat getRotateQuat() const { return m_RotateQuat; }

	// Scale
	void setScale(cvec3 _scale, bool _calculateMatrix = true)
	{ 
		if (m_Scale == _scale) return;
		m_Scale = _scale; 
		if (_calculateMatrix) CalculateMatrix();
	}
	cvec3 getScale() const { return m_Scale; }

	// Bounds
	void setBounds(BoundingBox _bbox) { m_Bounds = _bbox; }
	cbbox getBounds() const { return m_Bounds; }

	// General transform
	void setRelTrans(cmat4 _matrix) { m_RelTransform = _matrix; }
	cmat4 getRelTrans() const { return m_RelTransform; }

	void setAbsTrans(cmat4 _matrix) { m_AbsTransform = _matrix; }
	cmat4 getAbsTrans() const { return m_AbsTransform; }

	// Checks
	bool checkFrustum() const;
	bool checkDistance2D(float _distance) const;
	bool checkDistance(float _distance) const;

	// ISceneNode::Selectable
	void setSelectable() { m_Selectable = true; }
	bool isSelectable() const { return m_Selectable; }
	virtual string getObjectInfo() const { return "emp"; };

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
	virtual void CalculateMatrix(bool _isRotationQuat = false);

private:
	SceneNode*			m_Parent;
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

private: // IRenderable
	bool				m_IsVisible;
	bool				m_IsOpaque;
	uint32				m_DrawOrder;
	vec4				m_DebugColor;

protected:
	const CGroupQuality&	m_QualitySettings;
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
				float distToCameraLeft = glm::length(m_Camera->Position - left->getBounds().getCenter()) - left->getBounds().getRadius();
				float distToCameraRight = glm::length(m_Camera->Position - right->getBounds().getCenter()) - right->getBounds().getRadius();
				return distToCameraLeft > distToCameraRight;
			}
		}

		return false;
	}

private:
	Camera * m_Camera;
};