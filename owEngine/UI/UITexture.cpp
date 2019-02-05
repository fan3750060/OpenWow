#include "stdafx.h"

// Include
#include "SceneNodeUI.h"
#include "MaterialWrapper.h"
#include "UI/Materials/UI_Texture_Material.h"

// General
#include "UITexture.h"

// Additional
#include "Application.h"

UITexture::UITexture()
{
	m_Material = std::make_shared<UI_Texture_Material>();
	m_Material->SetWrapper(m_Material);;

	std::shared_ptr<IMesh> mesh = _RenderDevice->CreateScreenQuad();
	mesh->SetMaterial(m_Material);

	SetMesh(mesh);
}

UITexture::~UITexture()
{}



//
// UITexture
//
void UITexture::SetTexture(std::shared_ptr<Texture> _texture)
{
	m_Material->SetTexture(_texture);
}

void UITexture::SetColor(vec4 _color)
{
	m_Material->SetColor(_color);
}
