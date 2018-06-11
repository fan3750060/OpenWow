#pragma once

// FORWARD BEGIN
class Image;
// FORWARD END

__interface 
	__declspec(uuid("095BD687-7910-40BA-87A7-A7DD69EAB5F7"))
	IRender : public IManager
{
	// Main

	void Set2D();
	void Set3D();
	
	// 3D Render

	void BindRBs();
	void UnbindRBs();
	void PostprocessSimple();

	// GUI Render

	void RenderImage(vec2 _pos, Image* _image);
	void RenderImage(vec2 _pos, Image* _image, vec2 _size);

	void RenderTexture(vec2 _pos, R_Texture* _texture);
	void RenderTexture(vec2 _pos, R_Texture* _texture, vec2 _size);

	void RenderRectangle(vec2 _pos, vec2 _size, const Color& _color);
	void RenderRectangleOutline(vec2 _pos, vec2 _size, const Color& _color);

	void RenderText(vec2 _pos, cstring _string, const Color& _color) const;
	void RenderText(vec2 _pos, cstring _string, Font* _font, const Color& _color) const;
	void RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, const Color& _color) const;
	void RenderText(vec2 _pos, cstring _string, TextAlignW _alignW, TextAlignH _alignH, Font* _font, const Color& _color) const;
};