#pragma once

#include "SceneNodeUI.h"
#include "Fonts/Font.h"

class UIText : public SceneNodeUI
{
public:
	UIText();
	virtual ~UIText();

	void                        SetFont(std::shared_ptr<Font> _font);
	void                        SetText(const std::string& _text);


	// SceneNodeUI
	void                        SetMesh(std::shared_ptr<IMesh> mesh) override;
	std::shared_ptr<IMesh>      GetMesh() const override;

	bool						RenderMesh(RenderUIEventArgs& renderEventArgs) override;

private:
	std::shared_ptr<Font>       m_Font;
	std::string                 m_Text;
};