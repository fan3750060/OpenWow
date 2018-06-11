#include "stdafx.h"

// General
#include "TexturesManager.h"

#include __PACK_BEGIN
struct BLPHeader
{
    uint8 magic[4];
    uint32 type;

    uint8 compression;  // Compression: 1 for uncompressed, 2 for DXTC, 3 (cataclysm) for plain A8R8G8B8 m_DiffuseTextures
    uint8 alpha_depth;  // Alpha channel bit depth: 0, 1, 4 or 8
    uint8 alpha_type;   // 0, 1, 7, or 8
    uint8 has_mips;     // 0 = no mips, 1 = has mips

    uint32 width;
    uint32 height;
    uint32 mipOffsets[16];
    uint32 mipSizes[16];
};
#include __PACK_END

//

struct __RGBA
{
    uint8 r, g, b, a;
};

TexturesManager::TexturesManager(RenderDevice* _RenderDevice)
	: m_RenderDevice(_RenderDevice)
{
    ADDCONSOLECOMMAND_CLASS("tm_info", TexturesManager, PrintAllInfo);

	vec2 m_DefaultTextureSize = vec2(64, 64);

    struct
    {
        uint8 r, g, b, a;
    }  defaultColors[4096];

    for (uint8 i = 0; i < m_DefaultTextureSize.x; i++)
    {
        for (uint8 j = 0; j < m_DefaultTextureSize.y; j++)
        {
            defaultColors[i * 64 + j].r = i * 4;
            defaultColors[i * 64 + j].g = j * 4;
            defaultColors[i * 64 + j].b = ((i + j) % 64) * 4;
            defaultColors[i * 64 + j].a = 255;
        }
    }

	m_DefaultTexture2DObj = m_RenderDevice->createTexture(
		R_TextureTypes::Tex2D,
		m_DefaultTextureSize.x,
		m_DefaultTextureSize.y,
		1,
		R_TextureFormats::RGBA8,
		true,
		true,
		false,
		false);
	m_DefaultTexture2DObj->uploadTextureData(0, 0, defaultColors);

    //--------------


	AddManager<ITexturesManager>(this);
}

//

R_Texture* TexturesManager::LoadBLPTexture(IFile* f)
{
	R_Texture* _texture = nullptr;

    // Read data
    BLPHeader header;
    f->ReadBytes(&header, 148);

    assert1(header.magic[0] == 'B' && header.magic[1] == 'L' && header.magic[2] == 'P' && header.magic[3] == '2');
    assert1(header.type == 1);

    uint8 mipmax = header.has_mips ? 16 : 1;

    if (header.compression == 2)
    {
        R_TextureFormats::List format;

        if (header.alpha_type == 0)
        {
            format = R_TextureFormats::DXT1;
        }
        else if (header.alpha_type == 1)
        {
            format = R_TextureFormats::DXT3;
        }
        else if (header.alpha_type == 7)
        {
            format = R_TextureFormats::DXT5;
        }
        else
        {
            fail1();
        }

        _texture = m_RenderDevice->createTexture(R_TextureTypes::Tex2D, header.width, header.height, 1, format, header.has_mips, false, true, false);

        uint8* buf = new uint8[header.mipSizes[0]];
        for (uint8 i = 0; i < mipmax; i++)
        {
            if (header.mipOffsets[i])
            {
                assert1(header.mipSizes[i] > 0);

                f->Seek(header.mipOffsets[i]);
                f->ReadBytes(buf, header.mipSizes[i]);

				_texture->uploadTextureData(0, i, buf);
            }
            else
            {
                break;
            }
        }

        delete[] buf;
    }
    else if (header.compression == 1)
    {
        unsigned int pal[256];
        f->ReadBytes(pal, 1024);

        unsigned char* buf = new unsigned char[header.mipSizes[0]];
        unsigned int* buf2 = new unsigned int[header.width * header.height];
        unsigned int* p;
        unsigned char* c, *a;

        bool hasalpha = (header.alpha_depth != 0);

        _texture = m_RenderDevice->createTexture(R_TextureTypes::Tex2D, header.width, header.height, 1, R_TextureFormats::RGBA8, header.has_mips, false, false, false);

        for (int i = 0; i < mipmax; i++)
        {
            if (header.mipOffsets[i] && header.mipSizes[i])
            {
                f->Seek(header.mipOffsets[i]);
                f->ReadBytes(buf, header.mipSizes[i]);

                int cnt = 0;
                p = buf2;
                c = buf;
                a = buf + header.width * header.height;
                for (uint32 y = 0; y < header.height; y++)
                {
                    for (uint32 x = 0; x < header.width; x++)
                    {
                        unsigned int k = pal[*c++];
                        k = ((k & 0x00FF0000) >> 16) | ((k & 0x0000FF00)) | ((k & 0x000000FF) << 16);
                        int alpha;

                        if (header.alpha_depth == 8)
                        {
                            alpha = (*a++);
                        }
                        else if (header.alpha_depth == 1)
                        {
                            alpha = (*a & (1 << cnt++)) ? 0xff : 0;
                            if (cnt == 8)
                            {
                                cnt = 0;
                                a++;
                            }
                        }
                        else if (header.alpha_depth == 0)
                        {
                            alpha = 0xff;
                        }


                        k |= alpha << 24;
                        *p++ = k;
                    }
                }

				_texture->uploadTextureData(0, i, buf2);
            }
            else
            {
                break;
            }
        }

        delete[] buf2;
        delete[] buf;
    }
    else
    {
        Log::Warn("R_Texture[%s]: compression=[%d]", f->Path_Name().c_str(), header.compression);
        //fail1();
    }

    return _texture;
}

// Protected

R_Texture* TexturesManager::CreateAction(cstring _name)
{
	R_Texture* _texture = new R_Texture(m_RenderDevice);
	//wglMakeCurrent(_Render->dc, _Render->glrc2);

	UniquePtr<IFile> f(GetManager<IFilesManager>()->Open(_name));
	if (f == nullptr)
	{
		Log::Error("TexturesManager[%s]: Error while open texture.", _name.c_str());
		return DefaultTexture();
	}

	_texture = LoadBLPTexture(f);

	//Log::Info("TexturesManager[%s]: Texture loaded. Size [%0.0fx%0.0f].", f->Path_Name().c_str(), _texture->GetSize().x, _texture->GetSize().y);

	return DefaultTexture();
}

bool TexturesManager::DeleteAction(cstring name)
{
    return true;
}