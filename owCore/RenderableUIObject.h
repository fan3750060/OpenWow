#pragma once

class CRenderableUIObject : public IRenderableUI
{
public:
	void setDrawOrder(uint32 _order) override { m_DrawOrder = _order; }
	uint32 getDrawOrder() const override { return m_DrawOrder; }

private:
	uint32 m_DrawOrder;
};