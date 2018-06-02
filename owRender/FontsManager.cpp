#include "stdafx.h"

// General
#include "FontsManager.h"

// Additional
#include <freetype/config/ftheader.h>
#include FT_FREETYPE_H

#include "Render.h"
#include "RenderStorage.h"

FontsManager::FontsManager(RenderDevice* _RenderDevice)
	: m_RenderDevice(_RenderDevice)
{
	mainFont = Add("Fonts\\Consolas.TTF", 12);

	AddManager<IFontsManager>(this);
}


Font* FontsManager::Add(cstring _fontFileName, uint32 _fontSize)
{
	return RefManager1Dim::Add(_fontFileName + "__" + std::to_string(_fontSize));
}

//

Font* FontsManager::CreateAction(cstring _nameAndSize)
{
	uint32_t _delimIndex = _nameAndSize.find_last_of("__");
	if (_delimIndex == -1)
	{
		Log::Error("FontsManager[%s]: Incorrect font nameAndSize.", _nameAndSize.c_str());
		return nullptr;
	}

	string fontFileName = _nameAndSize.substr(0, _delimIndex - 1);
	uint32 fontSize = Utils::ToType<uint32>(_nameAndSize.substr(_delimIndex + 1));

	UniquePtr<IFile> f = GetManager<IFilesManager>()->Open(fontFileName);
	if (f == nullptr)
	{
		Log::Fatal("FontsManager[%s]: Error while loading font.", f->Path_Name().c_str());
		return nullptr;
	}

    R_Texture* texture = 0;
    vector<uint32> charWidth;
    charWidth.reserve(Font::NUM_CHARS);
	uint32 charHeight = 0;

	FT_Library ftLibrary;
	FT_Init_FreeType(&ftLibrary);

	FT_Face face;
	if (FT_New_Memory_Face(ftLibrary, f->GetData(), f->GetSize(), 0, &face) != 0)
	{
		Log::Error("FontsManager[%s]: Error while loading font. Could not load font file.", f->Path_Name().c_str());

		// Unload
		FT_Done_Face(face);
		FT_Done_FreeType(ftLibrary);
		return nullptr;
	}

	if (!(face->face_flags & FT_FACE_FLAG_SCALABLE) || !(face->face_flags & FT_FACE_FLAG_HORIZONTAL))
	{
		Log::Error("FontsManager[%s]: Error while loading font. Error setting font size.", f->Path_Name().c_str());

		// Unload
		FT_Done_Face(face);
		FT_Done_FreeType(ftLibrary);
		return nullptr;
	}

	FT_Set_Pixel_Sizes(face, fontSize, 0);

	// Step 2: Find maxAscent/Descent to calculate imageHeight //
	uint32_t imageHeight = 0;
	uint32_t imageWidth = 512;
	int maxDescent = 0;
	int maxAscent = 0;
	uint32_t lineSpace = imageWidth;
	uint32_t lines = 1;
	uint32_t charIndex;

	for (uint32 ch = 0; ch < Font::NUM_CHARS; ++ch)
	{
		// Look up the character in the font file.
		charIndex = FT_Get_Char_Index(face, ch + Font::SPACE);

		// RenderPhase the current glyph.
		FT_Load_Glyph(face, charIndex, FT_LOAD_RENDER);

		charWidth.push_back(face->glyph->metrics.horiAdvance >> 6);
		// If the line is full go to the next line
		if (charWidth[ch] > lineSpace)
		{
			lineSpace = imageWidth;
			++lines;
		}
		lineSpace -= charWidth[ch];

		maxAscent = max(face->glyph->bitmap_top, maxAscent);
		maxDescent = max((int)face->glyph->bitmap.rows - (int)face->glyph->bitmap_top, maxDescent);
	}

	charHeight = maxAscent + maxDescent;

	uint32_t neededHeight = (maxAscent + maxDescent) * lines;

	// Get the first power of two in which it will fit
	imageHeight = 16;
	while (imageHeight < neededHeight)
	{
		imageHeight <<= 1;
	}

	// Step 3: Generation of the actual texture //

	uint32* image = new uint32[imageHeight * imageWidth];
	memset(image, 0x00, imageHeight * imageWidth * sizeof(uint32));

	// These are the cameraPosition at which to draw the next glyph
	uint32_t x = 0;
	uint32_t y = maxAscent;

	vector<Texture_Vertex> fontVertices;
	float xOffset = 0.0f;

	for (uint32 ch = 0; ch < Font::NUM_CHARS; ++ch)
	{
		//Log::Warn("Char [%c] %d", char(ch), ch);
		uint32_t charIndex = FT_Get_Char_Index(face, ch + Font::SPACE);

		FT_Load_Glyph(face, charIndex, FT_LOAD_DEFAULT);
		FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

		if (charWidth[ch] > imageWidth - x)
		{
			x = 0;
			y += (maxAscent + maxDescent);
		}

		double texX1 = (double)(x) / imageWidth;
		double texX2 = (double)(x + charWidth[ch]) / imageWidth;
		double texY1 = (double)(y - maxAscent) / imageHeight;
		double texY2 = texY1 + (double)(charHeight) / imageHeight;

		fontVertices.push_back({vec3(0.0f,          0.0f,        1.0f), vec2(texX1, texY1)});
		fontVertices.push_back({vec3(charWidth[ch], 0.0f,        1.0f), vec2(texX2, texY1)});
		fontVertices.push_back({vec3(0.0f,          charHeight,  1.0f), vec2(texX1, texY2)});

		fontVertices.push_back({vec3(0.0f,          charHeight,  1.0f), vec2(texX1, texY2)});
		fontVertices.push_back({vec3(charWidth[ch], 0.0f,        1.0f), vec2(texX2, texY1)});
		fontVertices.push_back({vec3(charWidth[ch], charHeight,  1.0f), vec2(texX2, texY2)});

		for (uint32 row = 0; row < face->glyph->bitmap.rows; ++row)
		{
			for (uint32 pixel = 0; pixel < face->glyph->bitmap.width; ++pixel)
			{
				uint8* p = (uint8*)&(image[(x + face->glyph->bitmap_left + pixel) + (y - face->glyph->bitmap_top + row) * imageWidth]);
				p[0] = 0;
				p[1] = 0;
				p[2] = 0;
				p[3] = (face->glyph->bitmap.buffer[pixel + row * face->glyph->bitmap.pitch]);
			}
		}

		x += charWidth[ch];
	}


    R_Buffer* __vb = m_RenderDevice->createVertexBuffer(fontVertices.size() * sizeof(Texture_Vertex), fontVertices.data());

	//

    R_GeometryInfo* __geom = m_RenderDevice->beginCreatingGeometry(_Render->Storage()->__layout_GxVBF_PT);
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 0,            sizeof(Texture_Vertex));
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, sizeof(vec3), sizeof(Texture_Vertex));
	__geom->finishCreatingGeometry();

	// Font texture
	texture = m_RenderDevice->createTexture(R_TextureTypes::Tex2D, imageWidth, imageHeight, 1, R_TextureFormats::RGBA8, false, false, false, false);
	texture->uploadTextureData(0, 0, image);

	delete[] image;

	FT_Done_Face(face);
	FT_Done_FreeType(ftLibrary);

	//

	Font* font = new Font(texture, __geom, charWidth, charHeight);

	Log::Info("FontsManager[%s]: Font loaded. Size [%d].", f->Path_Name().c_str(), fontSize);

	return font;
}

bool FontsManager::DeleteAction(cstring name)
{
	return true;
}

