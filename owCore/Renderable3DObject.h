#pragma once

class Renderable3DObject
{
public:
	virtual void PreRender3D() = 0;
	virtual void Render3D() = 0;
	virtual void PostRender3D() = 0;

    //

	void SetDrawOrder(uint32 _order)
	{
		m_DrawOrder = _order;
	}
	uint32 GetDrawOrder() const
	{
		return m_DrawOrder;
	}

protected:
	bool Register(uint32 _order = 0);
	void Unregister();

private:
	uint32 m_DrawOrder;
};