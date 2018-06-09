#pragma once

#include "Camera.h"
#include "RenderDevice.h"

class GLFWBackend;

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

class RenderGL
{
	friend GLFWBackend;
	CLASS_INSTANCE(RenderGL);
public:
	RenderGL();
	~RenderGL();

	void Init();

	inline RenderStorage* Storage() { return m_RenderStorage; }
	inline TexturesManager* TexturesMgr() { return m_TexturesManager; }
	inline FontsManager* FontsMgr() { return m_FontsManager; }
	inline TechniquesManager* TechniquesMgr() { return m_TechniquesManager; }
	
	void Set3D();
	void Set2D();

	void RenderImage(vec2 _pos, Image* _image);
	void RenderImage(vec2 _pos, Image* _image, vec2 _size);

    void RenderTexture(vec2 _pos, R_Texture* _texture);
    void RenderTexture(vec2 _pos, R_Texture* _texture, vec2 _size);

	void RenderRectangle(vec2 _pos, vec2 _size, const Color& _color = COLOR_GREEN);
	void RenderRectangleOutline(vec2 _pos, vec2 _size, const Color& _color = COLOR_GREEN);

	void RenderText(vec2 _pos, cstring _string, const Color& _color = COLOR_WHITE) const;
	void RenderText(vec2 _pos, cstring _string, Font* _font, const Color& _color = COLOR_WHITE) const;
	void RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, const Color& _color = COLOR_WHITE) const;
	void RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, Font* _font, const Color& _color = COLOR_WHITE) const;

	void RenderQuad();
    void RenderQuadVT();

    void DrawBoundingBox(BoundingBox& _box, cmat4 _mat);

	void DrawPerfomance(vec2 _startPoint);

private:
	void OnWindowResized(uint32 _width, uint32 _height);

private:
	mat4 m_OrhoMatrix;

public:
	RenderDevice r;
    R_RenderBuffer* rb;
    R_RenderBuffer* rbFinal;
    Camera* mainCamera;

	HGLRC glrc1;
	HGLRC glrc2;
	HDC hdc;

private:
	RenderStorage*			m_RenderStorage;
	TexturesManager*        m_TexturesManager;
	FontsManager*           m_FontsManager;
	TechniquesManager*      m_TechniquesManager;
	
	CGroupVideo& m_VideoSettings;
};

#define _Render RenderGL::instance()

#include "TechniquesManager.h"