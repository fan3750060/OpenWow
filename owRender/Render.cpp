#include "stdafx.h"

// General
#include "Render.h"

RenderGL::RenderGL() :
	m_VideoSettings(GetSettingsGroup<CGroupVideo>())
{
	
}

RenderGL::~RenderGL()
{
	delete m_Camera;
}

void RenderGL::Init(IOpenGLAdapter* _adapter, HGLRC _context)
{
	r.init();
	r.setViewport(0, 0, m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY);

	//m_OrhoMatrix = Matrix4f::OrthoMat(0.0f, m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY, 0.0f, -1.0f, 1.0f);
	m_OrhoMatrix = Matrix4f::OrthoMat(0.0f, m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY, 0.0f, -1.0f, 1.0f);

	m_RenderBuffer = r.createRenderBuffer(m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY, R_TextureFormats::RGBA32F, true, 4, 0);

	m_RenderStorage = new RenderStorage(&r);
	m_TexturesManager = new TexturesManager(_adapter, &r);
	m_FontsManager = new FontsManager(&r);
	m_TechniquesManager = new TechniquesManager(&r);

	// Main game camera
	m_Camera = new Camera;
	m_Camera->setupViewParams(Math::Pi / 4.0f, m_VideoSettings.aspectRatio, 1.0f, 5000.0f);
	m_Camera->CreateRenderable();
}

void RenderGL::Set2D()
{
	r.setCullMode(R_CullMode::RS_CULL_NONE);

	r.setDepthMask(false);
	r.setDepthTest(false);

	r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);
}

void RenderGL::Set3D()
{
	r.setCullMode(R_CullMode::RS_CULL_BACK);

	r.setDepthMask(true);
	r.setDepthTest(true);

	r.setBlendMode(false);
}

void RenderGL::BindRBs()
{
	m_RenderBuffer->setRenderBuffer();
	r.clear();
}

void RenderGL::UnbindRBs()
{
	m_RenderBuffer->resetRenderBuffer(); // HACK!!!! MOVE RESET TO RenderDevice
	for (uint32 i = 0; i < 4; i++)
	{
		r.setTexture(i, m_RenderBuffer->getRenderBufferTex(i), SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_CLAMP, 0);
	}
	r.clear();
}

void RenderGL::PostprocessSimple()
{
	m_TechniquesManager->Postprocess_Simple->Bind();
	m_TechniquesManager->Postprocess_Simple->SetCameraPos(_Render->getCamera()->Position);

	r.setDepthTest(false);
	r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	RenderQuad();

	r.setBlendMode(false);
	r.setDepthTest(true);

	m_TechniquesManager->Postprocess_Simple->Unbind();
}

void RenderGL::DrawCube(cvec3 _pos, vec4 _color)
{
	mat4 world;
	world.translate(_pos);

	r.setCullMode(R_CullMode::RS_CULL_BACK);
	r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	m_TechniquesManager->Debug_Pass->Bind();
	m_TechniquesManager->Debug_Pass->SetWorldMatrix(world);
	m_TechniquesManager->Debug_Pass->SetColor4(vec4(1, 1, 1, 1));

	r.setGeometry(m_RenderStorage->_cubeGeo);
	r.drawIndexed(PRIM_TRILIST, 0, 36, 0, 8);

	m_TechniquesManager->Debug_Pass->Unbind();

	r.setFillMode(R_FillMode::RS_FILL_SOLID);
	r.setCullMode(R_CullMode::RS_CULL_NONE);
}

void RenderGL::DrawSphere(cmat4 _world, cvec3 _pos, float _radius, vec4 _color)
{
	mat4 world;
	world.translate(_pos);
	world.scale(_radius);

	world *= _world;

	r.setCullMode(R_CullMode::RS_CULL_BACK);
	r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	m_TechniquesManager->Debug_Pass->Bind();
	m_TechniquesManager->Debug_Pass->SetWorldMatrix(world);
	m_TechniquesManager->Debug_Pass->SetColor4(_color);

	r.setGeometry(m_RenderStorage->_sphereGeo);
	r.drawIndexed(PRIM_TRILIST, 0, 128 * 3, 0, 126);

	m_TechniquesManager->Debug_Pass->Unbind();

	r.setFillMode(R_FillMode::RS_FILL_SOLID);
	r.setCullMode(R_CullMode::RS_CULL_NONE);
}

void RenderGL::DrawGeo(cvec3 _pos, vec4 _color)
{
	mat4 world;
	world.translate(_pos);

	r.setCullMode(R_CullMode::RS_CULL_BACK);
	r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	m_TechniquesManager->Debug_Pass->Bind();
	m_TechniquesManager->Debug_Pass->SetWorldMatrix(world);
	m_TechniquesManager->Debug_Pass->SetColor4(vec4(1, 1, 1, 1));

	r.setGeometry(m_RenderStorage->_coneGeo);
	r.drawIndexed(PRIM_TRILIST, 0, 66, 0, 13);

	m_TechniquesManager->Debug_Pass->Unbind();

	r.setFillMode(R_FillMode::RS_FILL_SOLID);
	r.setCullMode(R_CullMode::RS_CULL_NONE);
}

void RenderGL::DrawBoundingBox(cbbox _box, vec4 _color)
{
	mat4 world;
	world.translate(_box.getMin());
	world.scale(_box.getMax() - _box.getMin());

	r.setGeometry(m_RenderStorage->_cubeGeo);

	//r.setCullMode(R_CullMode::RS_CULL_BACK);
	r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	m_TechniquesManager->Debug_Pass->Bind();
	m_TechniquesManager->Debug_Pass->SetWorldMatrix(world);
	m_TechniquesManager->Debug_Pass->SetColor4(_color);

	r.drawIndexed(PRIM_TRILIST, 0, 36, 0, 8);

	m_TechniquesManager->Debug_Pass->Unbind();

	r.setFillMode(R_FillMode::RS_FILL_SOLID);
	//r.setCullMode(R_CullMode::RS_CULL_NONE);
}

#pragma region GUI Part

void RenderGL::RenderImage(vec2 _pos, Image* _image)
{
	RenderImage(_pos, _image, _image->GetSize());
}

void RenderGL::RenderImage(vec2 _pos, Image* _image, vec2 _size)
{
	// Transform
	mat4 worldTransform;
	worldTransform.translate(_pos.x + _size.x / 2.0f, _pos.y + _size.y / 2.0f, 0.0f);
	worldTransform.scale(_size.x / 2.0f, _size.y / 2.0f, 1.0f);

	// Update buffer
	vector<vec2> texCoordsQuad;
	texCoordsQuad.push_back(_image->GetP0());
	texCoordsQuad.push_back(_image->GetP1());
	texCoordsQuad.push_back(_image->GetP3());
	texCoordsQuad.push_back(_image->GetP2());
	m_RenderStorage->__vbQuadVTDynamic->updateBufferData(4 * sizeof(vec3), 4 * sizeof(vec2), texCoordsQuad.data());

	// Shader
	m_TechniquesManager->UI_Texture->Bind();
	m_TechniquesManager->UI_Texture->SetProjectionMatrix(m_OrhoMatrix * worldTransform);

	// State
	r.setTexture(10, _image->GetTexture(), SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_CLAMP, 0);
	r.setGeometry(m_RenderStorage->__QuadVTDynamic);

	// Draw call
	r.drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);

	m_TechniquesManager->UI_Texture->Unbind();
}

//

void RenderGL::RenderTexture(vec2 _pos, R_Texture* _texture, bool rotate)
{
	RenderTexture(_pos, _texture, _texture->GetSize(), rotate);
}

void RenderGL::RenderTexture(vec2 _pos, R_Texture* _texture, vec2 _size, bool rotate)
{
	// Transform
	mat4 worldTransform;
	worldTransform.translate(_pos.x + _size.x / 2.0f, _pos.y + _size.y / 2.0f, 0.0f);
	if (rotate)
	{
		worldTransform.rotate(vec3(0.0f, Math::Pi, 0.0f)); // FIXME
	}
	worldTransform.scale(_size.x / 2.0f, _size.y / 2.0f, 0.0f);

	// Shader
	m_TechniquesManager->UI_Texture->Bind();
	m_TechniquesManager->UI_Texture->SetProjectionMatrix(m_OrhoMatrix * worldTransform);

	// State
	r.setTexture(10, _texture, SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_CLAMP, 0);
	r.setGeometry(m_RenderStorage->__QuadVT);

	// Draw call
	r.drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);

	m_TechniquesManager->UI_Texture->Unbind();
}

//

void RenderGL::RenderRectangle(vec2 _pos, vec2 _size, const Color& _color)
{
	// Transform
	mat4 worldTransform;
	worldTransform.translate(_pos.x + _size.x / 2.0f, _pos.y + _size.y / 2.0f, 0.0f);
	worldTransform.scale(_size.x / 2.0f, _size.y / 2.0f, 0.0f);

	// Shader
	m_TechniquesManager->UI_Color->Bind();
	m_TechniquesManager->UI_Color->SetProjectionMatrix(m_OrhoMatrix * worldTransform);
	m_TechniquesManager->UI_Color->SetColor(_color);

	// State
	r.setGeometry(m_RenderStorage->__Quad);

	// Draw call
	r.drawIndexed(PRIM_TRILIST, 0, 6, 0, 4);

	m_TechniquesManager->UI_Color->Unbind();
}

void RenderGL::RenderRectangleOutline(vec2 _pos, vec2 _size, const Color& _color)
{
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

	_font->Render(_string, _pos + offset, _color);
}

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

#pragma endregion

//

void RenderGL::DrawPerfomance(vec2 _startPoint)
{
	vec2 point = _startPoint;
	float diff = 15.0f;

	for (uint8 i = 0; i < OW_COUNT_ELEMENTS(PerfomanceMessages); i++)
	{
		if (PerfomanceMessages[i].what != PERF_DELIM)
		{
			RenderText(
				point,
				PerfomanceMessages[i].descr +
				string(": t[")
				+
				_Perfomance->GetTimer(PerfomanceMessages[i].what)
				+
				(
					PerfomanceMessages[i].showInc ? "], c["
					+
					_Perfomance->GetInc(PerfomanceMessages[i].what) + "]" : "]"
					)
			);
		}

		point.y += diff;
	}

	RenderText(point, "SUMMA: " + _Perfomance->Sum());
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


