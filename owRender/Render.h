#pragma once

#include "Camera.h"
#include "RenderDevice.h"
#include "FontsManager.h"
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

	void RenderImage(vec2 _pos, Image* _image) override;
	void RenderImage(vec2 _pos, Image* _image, vec2 _size) override;

    void RenderTexture(vec2 _pos, R_Texture* _texture, bool rotate = false) override;
    void RenderTexture(vec2 _pos, R_Texture* _texture, vec2 _size, bool rotate = false) override;
	void RenderTexture(vec2 _pos, R_Texture* _texture, vec2 _size, float rotate) override;

	void RenderRectangle(vec2 _pos, vec2 _size, const Color& _color = COLOR_GREEN) override;
	void RenderRectangleOutline(vec2 _pos, vec2 _size, const Color& _color = COLOR_GREEN) override;

	void RenderText(vec2 _pos, cstring _string, const Color& _color = COLOR_WHITE) const;
	void RenderText(vec2 _pos, cstring _string, Font* _font, const Color& _color = COLOR_WHITE) const;
	void RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, const Color& _color = COLOR_WHITE) const;
	void RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, Font* _font, const Color& _color = COLOR_WHITE) const;

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
	FontsManager* FontsMgr() { return m_FontsManager; }
	TechniquesManager* getTechniquesMgr() { return m_TechniquesManager; }

	void PushToQueue(RenderDrawCall _call) { m_RenderQueue->PushCall(_call); }

private:
	void OnWindowResized(uint32 _width, uint32 _height);

public:
	RenderDevice				r;
	R_RenderBuffer*				m_RenderBuffer;

private:
	mat4						m_OrhoMatrix;
	Camera*						m_Camera;

	IOpenGLAdapter*				m_OpenGLAdapter;
	RenderStorage*				m_RenderStorage;
	TexturesManager*			m_TexturesManager;
	FontsManager*				m_FontsManager;
	TechniquesManager*			m_TechniquesManager;
	RenderQueue*				m_RenderQueue;

	CGroupVideo&				m_VideoSettings;
};

extern RenderGL* _Render;