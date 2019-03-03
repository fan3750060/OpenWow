#pragma once

#include "SceneNodeUI.h"
#include "Fonts/Font.h"

class UIText : public SceneNodeUI
{
public:
	UIText();
	virtual ~UIText();

	// UIText
	void                        SetFont(std::shared_ptr<CFontMesh> _font);
	void                        SetText(const std::string& _text);
	void                        SetColor(cvec4 _color);

	// SceneNodeUI
	void                        SetMesh(std::shared_ptr<IMesh> mesh) override;
	std::shared_ptr<IMesh>      GetMesh() const override;

	bool						RenderMesh(RenderUIEventArgs& renderEventArgs) override;

private:
	std::shared_ptr<CFontMesh>         m_Font;
	std::shared_ptr<UI_Font_Material>  m_Material;
	std::string                        m_Text;
};