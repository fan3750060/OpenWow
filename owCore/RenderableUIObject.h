#pragma once


class RenderableUIObject
{
public:
	virtual void RenderUI() = 0;
    
    //

	void SetDrawOrder(uint32 _order)
	{
		m_DrawOrder = _order;
	}
	uint32 GetDrawOrder() const
	{
		return m_DrawOrder;
	}

private:
	uint32 m_DrawOrder;
};