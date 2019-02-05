#pragma once

// FORWARD BEGIN
class SceneNodeUI;
class UI_Texture_Material;
// FORWARD END

class UITexture : public SceneNodeUI
{
public:
	UITexture();
	virtual ~UITexture();

	// UITexture
	void SetTexture(std::shared_ptr<Texture> _texture);
	void SetColor(vec4 _color);

private:
	std::shared_ptr<UI_Texture_Material>   m_Material;
};