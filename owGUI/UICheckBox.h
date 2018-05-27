#pragma once

#include "UIElement.h"

class UICheckBox : public UIElement {
	typedef UIElement base;
public:
	UICheckBox();
	virtual ~UICheckBox();

	 void Init(cvec2 _position);

	bool IsChecked() const { return isChecked; }

	virtual void OnRenderUI();

	// IInputrListener
	bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos) override;

protected:
	bool isChecked;
	Image* checkBoxImages[2];
};
