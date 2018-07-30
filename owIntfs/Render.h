#pragma once

// FORWARD BEGIN
class Image;
// FORWARD END

struct
	__declspec(uuid("095BD687-7910-40BA-87A7-A7DD69EAB5F7"))
	IRender : public IManager
{
	virtual ~IRender() = 0 {};

	// Main

	virtual void Set2D() = 0;
	virtual void Set3D() = 0;
	
	// 3D Render

	virtual void BindRBs() = 0;
	virtual void UnbindRBs() = 0;
	virtual void PostprocessSimple() = 0;

	// GUI Render
	//virtual void RenderTexture(vec2 _pos, R_Texture* _texture, bool rotate) = 0;
	//virtual void RenderTexture(vec2 _pos, R_Texture* _texture, vec2 _size, bool rotate) = 0;
	//virtual void RenderTexture(vec2 _pos, R_Texture* _texture, vec2 _size, float rotate) = 0;

	virtual void RenderRectangle(vec2 _pos, vec2 _size, const Color& _color) = 0;
	virtual void RenderRectangleOutline(vec2 _pos, vec2 _size, const Color& _color) = 0;
};