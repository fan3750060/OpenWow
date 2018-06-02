#pragma once

class CRenderable3DObject : public IRenderable3D, public IOrderedObject
{
public:
	CRenderable3DObject() :
		m_IsVisible(false)
	{}

	// IRenderable
	bool IsVisible() const { return m_IsVisible; }
	void SetVisible(bool _value) override { m_IsVisible = _value; }

	// IOrderedObject
	inline void SetDrawOrder(uint32 _order) override { m_DrawOrder = _order; }
	inline uint32 GetDrawOrder() const override { return m_DrawOrder; }

private:
	bool m_IsVisible;
	uint32 m_DrawOrder;
};