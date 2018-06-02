#include "stdafx.h"

// General
#include "Render.h"

// Additional
#include "RenderStorage.h"

#include "FontsManager.h"
#include "TexturesManager.h"
#include "TechniquesManager.h"
#include "RenderStorage.h"

RenderGL::RenderGL() :
	m_VideoSettings(GetSettingsGroup<CGroupVideo>())
{
	

	hdc = wglGetCurrentDC();
	glrc1 = wglCreateContext(hdc);
	glrc2 = wglCreateContext(hdc);
	assert1(wglShareLists(glrc1, glrc2));
	wglMakeCurrent(hdc, glrc1);

	r.init();
    r.setViewport(0, 0, m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY);

    m_OrhoMatrix = Matrix4f::OrthoMat(0.0f, m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY, 0.0f, -1.0f, 1.0f);

    rb = r.createRenderBuffer(m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY, R_TextureFormats::RGBA32F, true, 4, 0);
    rbFinal = r.createRenderBuffer(m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY, R_TextureFormats::RGBA32F, false, 1, 0);

    // Main game camera
    mainCamera = new Camera;
    mainCamera->setupViewParams(Math::Pi / 4.0f, m_VideoSettings.aspectRatio, 2.0f, 10000.0f);
    _PipelineGlobal->SetCamera(mainCamera);
	_PipelineGlobal->SetCameraFrustum(mainCamera);


}

RenderGL::~RenderGL()
{
}

void RenderGL::Init()
{
	// Managers
	m_RenderStorage = new RenderStorage(&r);
	m_TexturesManager = new TexturesManager(&r);
	m_FontsManager = new FontsManager(&r);
	m_TechniquesManager = new TechniquesManager(&r);
	
}

void RenderGL::Set3D()
{
    // Cull face
    r.setCullMode(R_CullMode::RS_CULL_BACK);

    // Depth settings
    r.setDepthMask(true);
    r.setDepthTest(true);

    // Blending settings
    r.setBlendMode(false);
}

void RenderGL::Set2D()
{
    // Cull face
    r.setCullMode(R_CullMode::RS_CULL_NONE);

    // Depth settings
    r.setDepthMask(false);
    r.setDepthTest(false);

    // Blending settings
    r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);
}

// UI

void RenderGL::RenderImage(vec2 _pos, Image* _image)
{
    RenderImage(_pos, _image, _image->GetSize());
}

void RenderGL::RenderImage(vec2 _pos, Image* _image, vec2 _size)
{
    // Transform
    _Pipeline->Clear();
    _Pipeline->Translate(_pos.x + _size.x / 2.0f, _pos.y + _size.y / 2.0f, 0.0f);
    _Pipeline->Scale(_size.x / 2.0f, _size.y / 2.0f, 0.0f);

    // Update buffer
    vector<vec2> texCoordsQuad;
    texCoordsQuad.push_back(_image->GetP0());
    texCoordsQuad.push_back(_image->GetP1());
    texCoordsQuad.push_back(_image->GetP3());
    texCoordsQuad.push_back(_image->GetP2());
	m_RenderStorage->__vbQuadVTDynamic->updateBufferData(4 * sizeof(vec3), 4 * sizeof(vec2), texCoordsQuad.data());

    // Shader
	m_TechniquesManager->m_UI_Texture->Bind();
	m_TechniquesManager->m_UI_Texture->SetProjectionMatrix(m_OrhoMatrix * _Pipeline->GetWorld());

    // State
    r.setTexture(10, _image->GetTexture(), SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_CLAMP, 0);
    r.setGeometry(m_RenderStorage->__QuadVTDynamic);
    r.drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);

	m_TechniquesManager->m_UI_Texture->Unbind();
}

//

void RenderGL::RenderTexture(vec2 _pos, R_Texture* _texture)
{
    RenderTexture(_pos, _texture, _texture->GetSize());
}

void RenderGL::RenderTexture(vec2 _pos, R_Texture* _texture, vec2 _size)
{
    // Transform
    _Pipeline->Clear();
    _Pipeline->Translate(_pos.x + _size.x / 2.0f, _pos.y + _size.y / 2.0f, 0.0f);
    _Pipeline->Scale(_size.x / 2.0f, _size.y / 2.0f, 0.0f);

    // Shader
    m_TechniquesManager->m_UI_Texture->Bind();
    m_TechniquesManager->m_UI_Texture->SetProjectionMatrix(m_OrhoMatrix * _Pipeline->GetWorld());

    // State
    r.setTexture(10, _texture, SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_CLAMP, 0);
    r.setGeometry(m_RenderStorage->__QuadVT);
    r.drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);

    m_TechniquesManager->m_UI_Texture->Unbind();
}

//

void RenderGL::RenderRectangle(vec2 _pos, vec2 _size, const Color& _color)
{
    // Transform
    _Pipeline->Clear();
    _Pipeline->Translate(_pos.x + _size.x / 2.0f, _pos.y + _size.y / 2.0f, 0.0f);
    _Pipeline->Scale(_size.x / 2.0f, _size.y / 2.0f, 0.0f);

    // Shader
    m_TechniquesManager->m_UI_Color->Bind();
    m_TechniquesManager->m_UI_Color->SetProjectionMatrix(m_OrhoMatrix * _Pipeline->GetWorld());
    m_TechniquesManager->m_UI_Color->SetColor(_color);

    r.setGeometry(m_RenderStorage->__Quad);
    r.drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);

    m_TechniquesManager->m_UI_Color->Unbind();
}

void RenderGL::RenderRectangleOutline(vec2 _pos, vec2 _size, const Color& _color)
{
    // Transform
    /*_Pipeline->Clear();
    _Pipeline->Translate(_pos.x + _size.x / 2.0f, _pos.y + _size.y / 2.0f, 0.0f);
    _Pipeline->Scale(_size.x / 2.0f, _size.y / 2.0f, 0.0f);

    // Shader
    m_TechniquesManager->m_UI_Color->Bind();
    m_TechniquesManager->m_UI_Color->SetProjectionMatrix(m_OrhoMatrix * _Pipeline->GetWorld());
    m_TechniquesManager->m_UI_Color->SetColor(_color);

    r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

    r.setGeometry(_RenderStorage->__Quad);
    r.drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);

    r.setFillMode(R_FillMode::RS_FILL_SOLID);

    m_TechniquesManager->m_UI_Color->Unbind();*/



    r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);
    RenderRectangle(_pos, _size, _color);
    r.setFillMode(R_FillMode::RS_FILL_SOLID);
}

//

void RenderGL::RenderText(vec2 _pos, cstring _string, const Color& _color) const
{
    RenderText(_pos, _string, TextAlignW::TEXT_ALIGNW_LEFT, TextAlignH::TEXT_ALIGNH_BOTTOM, m_FontsManager->GetMainFont(), _color);
}

void RenderGL::RenderText(vec2 _pos, cstring _string, Font* _font, const Color& _color) const
{
    RenderText(_pos, _string, TextAlignW::TEXT_ALIGNW_LEFT, TextAlignH::TEXT_ALIGNH_BOTTOM, _font, _color);
}

void RenderGL::RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, const Color& _color) const
{
    RenderText(_pos, _string, _alignW, _alignH, m_FontsManager->GetMainFont(), _color);
}

void RenderGL::RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, Font* _font, const Color& _color) const
{
    auto stringWidth = _font->GetStringWidth(_string);
    auto fontHeight = _font->GetHeight();

    vec2 offset = vec2();

    switch (_alignW)
    {
        case TEXT_ALIGNW_LEFT:
        offset.x = 0;
        break;

        case TEXT_ALIGNW_CENTER:
        offset.x = -static_cast<float>(stringWidth / 2);
        break;

        case TEXT_ALIGNW_RIGHT:
        offset.x = -static_cast<float>(stringWidth);
        break;
    }

    switch (_alignH)
    {
        case TEXT_ALIGNH_TOP:
        offset.y = -static_cast<float>(fontHeight);
        break;

        case TEXT_ALIGNH_CENTER:
        offset.y = -static_cast<float>(fontHeight / 2);
        break;

        case TEXT_ALIGNH_BOTTOM:
        offset.y = 0;
        break;
    }

    m_TechniquesManager->m_UI_Font->Bind();
    m_TechniquesManager->m_UI_Font->SetProjectionMatrix(m_OrhoMatrix);
    m_TechniquesManager->m_UI_Font->SetFontColor(vec3(_color.red, _color.green, _color.blue));

    _font->Render(_string, _pos + offset);

    m_TechniquesManager->m_UI_Font->Unbind();
}

//

void RenderGL::RenderQuad()
{
    r.setGeometry(m_RenderStorage->__Quad);
    r.drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);
}

void RenderGL::RenderQuadVT()
{
    r.setGeometry(m_RenderStorage->__QuadVT);
    r.drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);
}

//

void RenderGL::DrawBoundingBox(BoundingBox& _box, cmat4 _mat)
{
    _Render->r.setGeometry(m_RenderStorage->_cubeGeo);

    BoundingBox newBox = _box;
    _box.transform(_mat);

    _Pipeline->Push();
    {
        _Pipeline->Clear();
        _Pipeline->Translate(newBox.Min);
        _Pipeline->Scale(newBox.Max - newBox.Min);

        _Render->r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);
        m_TechniquesManager->m_Debug_GeometryPass->Bind();
        m_TechniquesManager->m_Debug_GeometryPass->SetPVW();
    }
    _Pipeline->Pop();

    m_TechniquesManager->m_Debug_GeometryPass->SetColor4(vec4(1, 1, 1, 1));

    _Render->r.drawIndexed(PRIM_TRILIST, 0, 36, 0, 8);

    m_TechniquesManager->m_Debug_GeometryPass->Unbind();
    _Render->r.setFillMode(R_FillMode::RS_FILL_SOLID);
}

//

void RenderGL::OnWindowResized(uint32 _width, uint32 _height)
{
    // Window size
	m_VideoSettings.windowSizeX = _width;
	m_VideoSettings.windowSizeY = _height;

    // Aspect
	m_VideoSettings.CalculateAspectFactor();

    // Set viewport
    r.setViewport(0, 0, m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY);

    // Projection matix
    m_OrhoMatrix = Matrix4f::OrthoMat(0.0f, m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY, 0.0f, -1.0f, 1.0f);
}


