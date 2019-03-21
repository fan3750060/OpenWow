#pragma once

#include "Camera.h"
#include "RenderDevice.h"
#include "TexturesManager.h"
#include "TechniquesManager.h"
#include "RenderStorage.h"
#include "RenderQueue.h"

class RenderGL : public IRender
{
public:
	RenderGL();
	~RenderGL();

	void Init(IOpenGLAdapter* _adapter);
	
	//------------------
	// Main part
	//------------------

	void Set2D() override;
	void Set3D() override;
	

	//------------------
	// 3D Part
	//------------------

	void BindRBs() override;
	void UnbindRBs() override;
	void PostprocessSimple() override;

	// Primitives
	void DrawCube(cvec3 _pos, vec4 _color = vec4(1.0f, 1.0f, 1.0f, 1.0f));
	void DrawSphere(cmat4 _world, cvec3 _pos, float _radius, vec4 _color = vec4(1.0f, 1.0f, 1.0f, 1.0f));
	void DrawGeo(cvec3 _pos, vec4 _color = vec4(1.0f, 1.0f, 1.0f, 1.0f));
	void DrawBoundingBox(cbbox _box, vec4 _color = vec4(1.0f, 1.0f, 1.0f, 1.0f));


	//------------------
	// GUI Part
	//------------------

	void RenderTexture(vec2 _pos, std::shared_ptr<Texture> _texture, bool rotate = false);
    void RenderTexture(vec2 _pos, std::shared_ptr<Texture> _texture, vec2 _size, bool rotate = false);
	void RenderTexture(vec2 _pos, std::shared_ptr<Texture> _texture, vec2 _size, float rotate);

	void RenderRectangle(vec2 _pos, vec2 _size, const Color& _color = COLOR_GREEN) override;
	void RenderRectangleOutline(vec2 _pos, vec2 _size, const Color& _color = COLOR_GREEN) override;

	void RenderQuad();
    void RenderQuadVT();

	//------------------
	// Getters
	//------------------
	cmat4 getOrthoMatrix() const { return m_OrhoMatrix; }

	Camera* getCamera() { return m_Camera; }

public: // Getters
	IOpenGLAdapter* getAdapter() { return m_OpenGLAdapter; }
	RenderStorage* getRenderStorage() { return m_RenderStorage; }
	TexturesManager* TexturesMgr() { return m_TexturesManager; }
	TechniquesManager* getTechniquesMgr() { return m_TechniquesManager; }

	void PushToQueue(RenderDrawCall _call) { m_RenderQueue->PushCall(_call); }

private:
	void OnWindowResized(uint32 _width, uint32 _height);

public:
	RenderDevice				r;
	std::shared_ptr<R_RenderBuffer>				m_RenderBuffer;

private:
	mat4						m_OrhoMatrix;
	Camera*						m_Camera;

	IOpenGLAdapter*				m_OpenGLAdapter;
	RenderStorage*				m_RenderStorage;
	TexturesManager*			m_TexturesManager;
	TechniquesManager*			m_TechniquesManager;
	RenderQueue*				m_RenderQueue;

	CGroupVideo&				m_VideoSettings;
};

extern RenderGL* _Render;