#pragma once

#include "UIBaseNode.h"
#include "Materials/UI_Texture_Material.h"

class CUITextureNode : public CUIBaseNode
{
public:
	CUITextureNode();
	virtual ~CUITextureNode();

	// CUITextureNode
	void SetTexture(std::shared_ptr<Texture> _texture);
	void SetColor(vec4 _color);

	// CUIBaseNode
	bool AcceptMesh(IVisitor& visitor) override;

private:
	std::shared_ptr<UI_Texture_Material>   m_Material;
};