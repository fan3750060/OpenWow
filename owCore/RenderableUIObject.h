#pragma once

class CRenderableUIObject : public IRenderableUI
{
public:
	void SetDrawOrder(uint32 _order) override { m_DrawOrder = _order; }
	uint32 GetDrawOrder() const override { return m_DrawOrder; }

private:
	uint32 m_DrawOrder;
};