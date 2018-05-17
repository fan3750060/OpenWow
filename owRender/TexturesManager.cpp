#include "stdafx.h"

// General
#include "TexturesManager.h"

#include "../shared/pack_begin.h"
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
#include "../shared/pack_end.h"

//

struct __RGBA
{
    uint8 r, g, b, a;
};

TexturesManager::TexturesManager(RenderDevice* _RenderDevice)
	: m_RenderDevice(_RenderDevice)
{
    ADDCONSOLECOMMAND_CLASS("tm_info", TexturesManager, PrintAllInfo);

    RefManager1DimAssync::Init();

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

    m_DefaultTexture2DObj = m_RenderDevice->createTexture(R_TextureTypes::Tex2D, m_DefaultTextureSize.x, m_DefaultTextureSize.y, 1, R_TextureFormats::RGBA8, true, true, false, false);
	m_DefaultTexture2DObj->uploadTextureData(0, 0, defaultColors);

    //--------------

    /*m_DefaultTextureCubeObj = m_RenderDevice->createTexture(R_TextureTypes::TexCube, 4, 4, 1, R_TextureFormats::RGBA8, true, true, false, false);
    for (uint32 i = 0; i < 6; ++i)
    {
        m_RenderDevice->uploadTextureData(m_DefaultTextureCubeObj, i, 0, texData);
    }*/

    //-------------

    /*unsigned char *texData2 = new unsigned char[256];
    memcpy(texData2, texData, 64);
    memcpy(texData2 + 64, texData, 64);
    memcpy(texData2 + 128, texData, 64);
    memcpy(texData2 + 192, texData, 64);

    m_DefaultTexture3DObj = m_RenderDevice->createTexture(R_TextureTypes::Tex3D, 4, 4, 4, R_TextureFormats::RGBA8, true, true, false, false);
    m_RenderDevice->uploadTextureData(m_DefaultTexture3DObj, 0, 0, texData2);
    delete[] texData2;*/
}

TexturesManager::~TexturesManager()
{
    RefManager1DimAssync::Destroy();

    DeleteAll();

    Log::Info("TexturesManager[]: All textures destroyed.");
}

//

bool TexturesManager::LoadBLPTexture(File& _file, R_Texture*& _texture)
{
    // Read data
    BLPHeader header;
    _file.ReadBytes(&header, 148);

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

                _file.Seek(header.mipOffsets[i]);
                _file.ReadBytes(buf, header.mipSizes[i]);

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
        _file.ReadBytes(pal, 1024);

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
                _file.Seek(header.mipOffsets[i]);
                _file.ReadBytes(buf, header.mipSizes[i]);

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
        Log::Warn("R_Texture[%s]: compression=[%d]", _file.Path_Name().c_str(), header.compression);
        //fail1();
    }

    return true;
}

R_Texture* TexturesManager::Add(cstring _textureFileName)
{
    return RefManager1DimAssync::Add(_textureFileName);
}

R_Texture* TexturesManager::Add(File& _textureFile)
{
    return RefManager1DimAssync::Add(_textureFile.Path_Name());
}

// Protected


R_Texture* TexturesManager::CreateAction(cstring name)
{
   // R_Texture* texture = new R_Texture();
    //    m_DefaultTexture2DObj;

    return nullptr;
}

void TexturesManager::LoadAction(string _name, R_Texture*& _texture)
{
    //wglMakeCurrent(_Render->dc, _Render->glrc2);

    File f = _name;

    if (!f.Open())
    {
        Log::Error("TexturesManager[%s]: Error while open texture.", f.Path_Name().c_str());
        _texture = DefaultTexture();
        return;
    }

    //Log::Info("TexturesManager[%s]: R_Texture loading.", f.Path_Name().c_str());

    // Load texture
    bool result = LoadBLPTexture(f, _texture);

    // Check result
    if (!result)
    {
        Log::Error("TexturesManager[%s]: Error while loading texture data.", f.Path_Name().c_str());
        _texture = DefaultTexture();
        return;
    }

    //Log::Info("TexturesManager[%s]: Texture loaded. Size [%0.0fx%0.0f].", f.Path_Name().c_str(), _texture->GetSize().x, _texture->GetSize().y);
}

bool TexturesManager::DeleteAction(cstring name)
{
    return true;
}