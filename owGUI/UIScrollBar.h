#pragma once

#include "UIElement.h"

class UIScrollBar : public UIElement {
	typedef UIElement base;
public:
	UIScrollBar();
	~UIScrollBar();

	 void Init(cvec2 _position, cvec2 _size);

	//

	int Data() const { return static_cast<int>(value * 100.0); };

	//

	void OnRenderUI();

	// IInputrListener
	void OnMouseMoved(cvec2 _mousePos) override;
	void OnMouseLeaved() override;
	bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override;
	bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos) override;
	bool OnMouseWheel(int _yoffset) override;

private:
	double value;

	vec2 begunokPoint; // begunok render point
	int begunokYOffset;
	vec2 begunokSize;

	bool begunokSelected; // mouse on begunok
	bool begunokPressed; // mouse pressed

	vec2 begunokBounds;
	int begunokBoundsSize;

	Image* begunokImage;
};
