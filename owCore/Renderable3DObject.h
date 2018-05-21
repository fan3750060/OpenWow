#pragma once

class Renderable3DObject
{
public:
	virtual void PreRender3D(double t, double dt) = 0;
	virtual void Render3D() = 0;
	virtual void PostRender3D() = 0;

	//

	inline void SetDrawOrder(uint32 _order) { m_DrawOrder = _order; }
	inline uint32 GetDrawOrder() const { return m_DrawOrder; }
	inline bool IsVisible() { return m_IsVisible; }

protected:
	bool m_IsVisible;

private:
	uint32 m_DrawOrder;
};