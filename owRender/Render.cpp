#include "stdafx.h"

// General
#include "Render.h"

// Additional
#include "Material.h"

RenderGL* _Render;

RenderGL::RenderGL() :
	m_OpenGLAdapter(nullptr),
	m_VideoSettings(GetSettingsGroup<CGroupVideo>())
{}

RenderGL::~RenderGL()
{
	delete m_Camera;

	delete m_RenderStorage;
	//delete m_FontsManager;
	delete m_TechniquesManager;
	delete m_RenderQueue;

	delete m_TexturesManager;
}

void RenderGL::Init(IOpenGLAdapter* _adapter)
{
	assert1(_adapter != nullptr);
	m_OpenGLAdapter = _adapter;

	//--

	r.init(m_OpenGLAdapter);
	r.setViewport(0, 0, m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY);

	//--

	m_RenderStorage = new RenderStorage(&r);
	m_TexturesManager = new TexturesManager(m_OpenGLAdapter, &r);
	m_TechniquesManager = new TechniquesManager(&r);
	m_RenderQueue = new RenderQueue(&r);

	//--

	m_OrhoMatrix = glm::ortho(0.0f, m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY, 0.0f, -1.0f, 1.0f);
	//m_RenderBuffer = r.createRenderBuffer(m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY, R_TextureFormats::RGBA16F, true, 1, 4);

	// Main game camera
	m_Camera = new Camera;
	m_Camera->setupViewParams(glm::pi<float>() / 4.0f, m_VideoSettings.aspectRatio, 2.0f, 3000.0f);
	m_Camera->CreateRenderable();
}

void RenderGL::Set2D()
{
	r.setCullMode(R_CullMode::RS_CULL_NONE);
	r.setFillMode(R_FillMode::RS_FILL_SOLID);

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
	//m_RenderBuffer->setRenderBuffer();
	r.clear();
}

void RenderGL::UnbindRBs()
{
	/*m_RenderBuffer->resetRenderBuffer(); // HACK!!!! MOVE RESET TO RenderDevice
	for (uint32 i = 0; i < 4; i++)
	{
		r.setTexture(i, m_RenderBuffer->getRenderBufferTex(i), SS_FILTER_POINT | SS_ADDR_CLAMP, 0);
	}
	r.clear();*/
}

void RenderGL::PostprocessSimple()
{
	m_TechniquesManager->Postprocess_Simple->Bind();
	m_TechniquesManager->Postprocess_Simple->setCameraPos(_Render->getCamera()->Position);

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
	world = glm::translate(world, _pos);

	r.setCullMode(R_CullMode::RS_CULL_BACK);
	r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	m_TechniquesManager->Debug_Pass->Bind();
	m_TechniquesManager->Debug_Pass->setWorld(world);
	m_TechniquesManager->Debug_Pass->SetColor4(vec4(1, 1, 1, 1));

	r.setGeometry(m_RenderStorage->_cubeGeo);
	r.drawIndexed(0, 36, 0, 8);

	m_TechniquesManager->Debug_Pass->Unbind();

	r.setFillMode(R_FillMode::RS_FILL_SOLID);
	r.setCullMode(R_CullMode::RS_CULL_NONE);
}

void RenderGL::DrawSphere(cmat4 _world, cvec3 _pos, float _radius, vec4 _color)
{
	mat4 world;
	world = glm::translate(world, _pos);
	world = glm::scale(world, vec3(_radius));

	world *= _world;

	r.setCullMode(R_CullMode::RS_CULL_BACK);
	r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	m_TechniquesManager->Debug_Pass->Bind();
	m_TechniquesManager->Debug_Pass->setWorld(world);
	m_TechniquesManager->Debug_Pass->SetColor4(_color);

	r.setGeometry(m_RenderStorage->_sphereGeo);
	r.drawIndexed(0, 128 * 3, 0, 126);

	m_TechniquesManager->Debug_Pass->Unbind();

	r.setFillMode(R_FillMode::RS_FILL_SOLID);
	r.setCullMode(R_CullMode::RS_CULL_NONE);
}

void RenderGL::DrawGeo(cvec3 _pos, vec4 _color)
{
	mat4 world = glm::translate(_pos);

	r.setCullMode(R_CullMode::RS_CULL_BACK);
	r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	m_TechniquesManager->Debug_Pass->Bind();
	m_TechniquesManager->Debug_Pass->setWorld(world);
	m_TechniquesManager->Debug_Pass->SetColor4(vec4(1, 1, 1, 1));

	r.setGeometry(m_RenderStorage->_coneGeo);
	r.drawIndexed(0, 66, 0, 13);

	m_TechniquesManager->Debug_Pass->Unbind();

	r.setFillMode(R_FillMode::RS_FILL_SOLID);
	r.setCullMode(R_CullMode::RS_CULL_NONE);
}

void RenderGL::DrawBoundingBox(cbbox _box, vec4 _color)
{
	mat4 world;
	world = glm::translate(world, _box.getMin());
	world = glm::scale(world, _box.getMax() - _box.getMin());

	r.setGeometry(m_RenderStorage->_cubeGeo);

	//r.setCullMode(R_CullMode::RS_CULL_BACK);
	r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	m_TechniquesManager->Debug_Pass->Bind();
	m_TechniquesManager->Debug_Pass->setWorld(world);
	m_TechniquesManager->Debug_Pass->SetColor4(_color);

	r.drawIndexed(0, 36, 0, 8);

	m_TechniquesManager->Debug_Pass->Unbind();

	r.setFillMode(R_FillMode::RS_FILL_SOLID);
	//r.setCullMode(R_CullMode::RS_CULL_NONE);
}

#pragma region GUI Part

void RenderGL::RenderTexture(vec2 _pos, SharedTexturePtr _texture, bool rotate)
{
	RenderTexture(_pos, _texture, _texture->GetSize(), rotate);
}

void RenderGL::RenderTexture(vec2 _pos, SharedTexturePtr _texture, vec2 _size, bool rotate)
{
	// Transform
	mat4 worldTransform;
	worldTransform = glm::translate(worldTransform, vec3(_pos.x + _size.x / 2.0f, _pos.y + _size.y / 2.0f, 0.1f));
	if (rotate)
	{
		worldTransform = glm::rotate(worldTransform, glm::pi<float>(), vec3(0, 1, 0));// FIXME
	}
	worldTransform = glm::scale(worldTransform, vec3(_size.x / 2.0f, _size.y / 2.0f, 0.0f));

	// Shader
	m_TechniquesManager->UI_Texture->Bind();
	m_TechniquesManager->UI_Texture->setProj(m_OrhoMatrix * worldTransform);

	// State
	r.setTexture(Material::C_DiffuseTextureIndex, _texture, SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_CLAMP, 0);
	r.setGeometry(m_RenderStorage->__QuadVT);

	// Draw call
	r.drawIndexed(0, 6, 0, 4);

	m_TechniquesManager->UI_Texture->Unbind();
}

void RenderGL::RenderTexture(vec2 _pos, SharedTexturePtr _texture, vec2 _size, float rotate)
{
	// Transform
	mat4 worldTransform;
	worldTransform = glm::translate(worldTransform, vec3(_pos.x + _size.x / 2.0f, _pos.y + _size.y / 2.0f, 0.0f));
	worldTransform = glm::rotate(worldTransform, rotate, vec3(0.0f, 0.0f, 1.0f));
	worldTransform = glm::scale(worldTransform, vec3(_size.x / 2.0f, _size.y / 2.0f, 1.0f));

	// Shader
	m_TechniquesManager->UI_Texture->Bind();
	m_TechniquesManager->UI_Texture->setProj(m_OrhoMatrix * worldTransform);

	// State
	r.setTexture(Material::C_DiffuseTextureIndex, _texture, SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_CLAMP, 0);
	r.setGeometry(m_RenderStorage->__QuadVT);

	// Draw call
	r.drawIndexed(0, 6, 0, 4);

	m_TechniquesManager->UI_Texture->Unbind();
}

//

void RenderGL::RenderRectangle(vec2 _pos, vec2 _size, const Color& _color)
{
	// Transform
	mat4 worldTransform;
	worldTransform = glm::translate(worldTransform, vec3(_pos.x + _size.x / 2.0f, _pos.y + _size.y / 2.0f, 0.0f));
	worldTransform = glm::scale(worldTransform, vec3(_size.x / 2.0f, _size.y / 2.0f, 1.0f));

	// Shader
	m_TechniquesManager->UI_Color->Bind();
	m_TechniquesManager->UI_Color->setProj(m_OrhoMatrix * worldTransform);
	m_TechniquesManager->UI_Color->SetColor(_color);

	// State
	r.setGeometry(m_RenderStorage->__Quad);

	// Draw call
	r.drawIndexed(0, 6, 0, 4);

	m_TechniquesManager->UI_Color->Unbind();
}

void RenderGL::RenderRectangleOutline(vec2 _pos, vec2 _size, const Color& _color)
{
	r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);
	RenderRectangle(_pos, _size, _color);
	r.setFillMode(R_FillMode::RS_FILL_SOLID);
}

//


void RenderGL::RenderQuad()
{
	r.setGeometry(m_RenderStorage->__Quad);
	r.drawIndexed(0, 6, 0, 4);
}

void RenderGL::RenderQuadVT()
{
	r.setGeometry(m_RenderStorage->__QuadVT);
	r.drawIndexed(0, 6, 0, 4);
}

#pragma endregion

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
	m_OrhoMatrix = glm::ortho(0.0f, m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY, 0.0f, -1.0f, 1.0f);
}


