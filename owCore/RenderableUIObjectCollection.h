#pragma once

class CRenderableUIObject : public IRenderableUI
{
public:
	CRenderableUIObject(IRenderableUI* _object, uint8 _priority) :
		m_Object(_object),
		m_DrawOrder(_priority)
	{}

	CRenderableUIObject& operator=(const CRenderableUIObject& _other)
	{
		this->m_Object = _other.m_Object;
		this->m_DrawOrder = _other.m_DrawOrder;
		return *this;
	}

	void RenderUI() { m_Object->RenderUI(); }

	IRenderableUI* getInputListener() { return m_Object; }
	uint32 getDrawOrder() const { return m_DrawOrder; }

private:
	IRenderableUI*	m_Object;
	uint32			m_DrawOrder;
};

struct CRenderableUIObjectCompare
{
	bool operator() (const CRenderableUIObject& left, const CRenderableUIObject& right) const
	{
		return left.getDrawOrder() < right.getDrawOrder();
	}
};

class CRenderableUIObjectCollection
{
public:
	~CRenderableUIObjectCollection();

	bool RegisterObject(IRenderableUI* _uiObject, uint32 _DrawOrder = 0);
	void UnregisterObject(IRenderableUI* _uiObject);

	void RenderUI();

private:
	vector<CRenderableUIObject>   m_Objects;
};