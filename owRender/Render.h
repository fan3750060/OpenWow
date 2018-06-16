#pragma once

#include "RenderDevice.h"

class TexturesManager;
class FontsManager;
class TechniquesManager;
class RenderStorage;

struct RenderTarget
{
	std::string           id;
	uint32                numColBufs;
	R_TextureFormats::List  format;
	uint32                width, height;
	uint32                samples;
	float                 scale;  // Scale factor for FB width and height
	bool                  hasDepthBuf;
	uint32                rendBuf;

	RenderTarget()
	{
		hasDepthBuf = false;
		numColBufs = 0;
		rendBuf = 0;
		width = height = 0;
		samples = 0;
		scale = 0;
		format = R_TextureFormats::Unknown;
	}
};

class RenderGL : public IRender
{
	CLASS_INSTANCE(RenderGL);
public:
	RenderGL();
	~RenderGL();

	void Init(IOpenGLAdapter* _adapter, HGLRC _context);
	
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
	void DrawSphere(cvec3 _pos, vec4 _color = vec4(1.0f, 1.0f, 1.0f, 1.0f));
	void DrawGeo(cvec3 _pos, vec4 _color = vec4(1.0f, 1.0f, 1.0f, 1.0f));
	void DrawBoundingBox(cbbox _box, vec4 _color = vec4(1.0f, 1.0f, 1.0f, 1.0f));


	//------------------
	// GUI Part
	//------------------

	void RenderImage(vec2 _pos, Image* _image) override;
	void RenderImage(vec2 _pos, Image* _image, vec2 _size) override;

    void RenderTexture(vec2 _pos, R_Texture* _texture) override;
    void RenderTexture(vec2 _pos, R_Texture* _texture, vec2 _size) override;

	void RenderRectangle(vec2 _pos, vec2 _size, const Color& _color = COLOR_GREEN) override;
	void RenderRectangleOutline(vec2 _pos, vec2 _size, const Color& _color = COLOR_GREEN) override;

	void RenderText(vec2 _pos, cstring _string, const Color& _color = COLOR_WHITE) const;
	void RenderText(vec2 _pos, cstring _string, Font* _font, const Color& _color = COLOR_WHITE) const;
	void RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, const Color& _color = COLOR_WHITE) const;
	void RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, Font* _font, const Color& _color = COLOR_WHITE) const;

	void RenderQuad();
    void RenderQuadVT();

	void DrawPerfomance(vec2 _startPoint);

	//------------------
	// Getters
	//------------------
	cmat4 getOrthoMatrix() const { return m_OrhoMatrix; }
	Camera* getCamera() { return mainCamera; }

public: // Getters
	inline RenderStorage* getRenderStorage() { return m_RenderStorage; }
	inline TexturesManager* TexturesMgr() { return m_TexturesManager; }
	inline FontsManager* FontsMgr() { return m_FontsManager; }
	inline TechniquesManager* getTechniquesMgr() { return m_TechniquesManager; }

private:
	void OnWindowResized(uint32 _width, uint32 _height);

private:
	mat4 m_OrhoMatrix;

public:
	RenderDevice r;
    SmartPtr<R_RenderBuffer> rb;
    SmartPtr<R_RenderBuffer> rbFinal;

    Camera* mainCamera;

private:
	RenderStorage*			m_RenderStorage;
	TexturesManager*        m_TexturesManager;
	FontsManager*           m_FontsManager;
	TechniquesManager*      m_TechniquesManager;
	
	CGroupVideo& m_VideoSettings;
};

#define _Render RenderGL::instance()

#include "TechniquesManager.h"