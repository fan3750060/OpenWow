#pragma once

class CRenderable3DObject : public IRenderable3D
{
public:
	CRenderable3DObject() :
		m_IsVisible(false),
		m_IsOpaque(false)
	{}
	virtual ~CRenderable3DObject()
	{}

	// IRenderable
	bool isVisible() const { return m_IsVisible; }
	void setVisible(bool _value) override { m_IsVisible = _value; }
	void setOpaque(bool _value) override { m_IsOpaque = _value; }
	bool isOpaque() const override { return m_IsOpaque; }
	inline void setDrawOrder(uint32 _order) override { m_DrawOrder = _order; }
	inline uint32 getDrawOrder() const override { return m_DrawOrder; }

private:
	bool m_IsVisible;
	bool m_IsOpaque;
	uint32 m_DrawOrder;
};