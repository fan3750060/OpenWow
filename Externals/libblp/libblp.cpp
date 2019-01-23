
// General
#include "libblp.h"

// Additional
#include <algorithm>
#include "DDSLib.h"

enum BLPColorEncoding : uint8_t {
	COLOR_JPEG = 0, // not supported
	COLOR_PALETTE = 1,
	COLOR_DXT = 2,
	COLOR_ARGB8888 = 3,
	COLOR_ARGB8888_dup = 4,    // same decompression, likely other PIXEL_FORMAT
};

enum BLPPixelFormat : uint8_t {
	PIXEL_DXT1 = 0,
	PIXEL_DXT3 = 1,
	PIXEL_ARGB8888 = 2,
	PIXEL_ARGB1555 = 3,
	PIXEL_ARGB4444 = 4,
	PIXEL_RGB565 = 5,
	PIXEL_DXT5 = 7,
	PIXEL_UNSPECIFIED = 8,
	PIXEL_ARGB2565 = 9,
	NUM_PIXEL_FORMATS = 10,
};

struct BLPHeader
{
	uint8_t    magic[4];
	uint32_t     type;

	BLPColorEncoding colorEncoding;
	uint8_t    alphaChannelBitDepth;  // Alpha channel bit depth: 0, 1, 4 or 8
	BLPPixelFormat   pixelFormat;   // 0, 1, 7, or 8
	uint8_t    has_mips;     // 0 = no mips, 1 = has mips

	uint32_t     width;
	uint32_t     height;
	uint32_t     mipOffsets[LIBBLP_MIPMAP_COUNT];
	uint32_t     mipSizes[LIBBLP_MIPMAP_COUNT];
	uint32_t     pallete[LIBBLP_PALETTE_SIZE];
};

int LIBBLP_Load(const uint8_t* inputBuffer, uint32_t inputBufferSize, LIBBLP_PixelView * view)
{
	_ASSERT(view != NULL);

	BLPHeader header;
	memcpy_s(&header, sizeof(BLPHeader), &inputBuffer[0], sizeof(BLPHeader));
	_ASSERT(header.magic[0] == 'B' && header.magic[1] == 'L' && header.magic[2] == 'P' && header.magic[3] == '2');
	_ASSERT(header.type == 1);

	if (header.width & (header.width - 1))
	{
		view->IsTexture3D = true;
	}
	if (header.height & (header.height - 1))
	{
		return LIBBLP_ERROR_SIZE;
	}

	view->HasAlpha = (header.alphaChannelBitDepth != 0);
	uint8_t mipmax = (header.has_mips ? LIBBLP_MIPMAP_COUNT : 1);

	for (uint8_t currentMip = 0; currentMip < mipmax; currentMip++)
	{
		if ((header.mipOffsets[currentMip] == 0) || (header.mipSizes[currentMip] == 0))
		{
			break;
		}

		uint16_t mipWidth = std::max(header.width >> currentMip, 1u);
		uint16_t mipHeight = std::max(header.height >> currentMip, 1u);

		view->MipCount++;
		view->MipWidth[currentMip] = mipWidth;
		view->MipHeight[currentMip] = mipHeight;

		switch (header.colorEncoding)
		{
		case BLPColorEncoding::COLOR_PALETTE:
		{
			// Data in mipmaps in indices info pallete
			uint8_t* indexInPalleteBuffer = new uint8_t[header.mipSizes[currentMip]];
			memcpy_s(indexInPalleteBuffer, header.mipSizes[currentMip], &inputBuffer[0] + header.mipOffsets[currentMip], header.mipSizes[currentMip]);

			view->MipData[currentMip] = new uint8_t[header.width * header.height * 4];
			uint32_t resultBufferCntr = 0;

			uint8_t* indexInPalleteColor = indexInPalleteBuffer;
			int alphaBitCntr = 0;
			uint8_t* indexInPalleteAlpha = &indexInPalleteBuffer[0] + mipWidth * mipHeight;

			for (uint32_t y = 0; y < mipWidth; y++)
			{
				for (uint32_t x = 0; x < mipHeight; x++)
				{
					// Read color
					uint32_t color = header.pallete[*indexInPalleteColor++];
					//color = ((color & 0x00FF0000) >> 16) | ((color & 0x0000FF00)) | ((color & 0x000000FF) << 16);

					// Read alpha
					uint8_t alpha;
					switch (header.alphaChannelBitDepth)
					{
					case 0:
						alpha = 0xff;
						break;
					case 1:
						alpha = (*indexInPalleteAlpha & (1 << alphaBitCntr++)) ? 0xff : 0x00;
						if (alphaBitCntr == 8)
						{
							alphaBitCntr = 0;
							indexInPalleteAlpha++;
						}
						break;
					case 8:
						alpha = (*indexInPalleteAlpha++);
						break;
					default:
						return LIBBLP_ERROR_FORMAT;
					}

					view->MipData[currentMip][resultBufferCntr++] = ((color & 0x000000FF));
					view->MipData[currentMip][resultBufferCntr++] = ((color & 0x0000FF00) >> 8);
					view->MipData[currentMip][resultBufferCntr++] = ((color & 0x00FF0000) >> 16);
					view->MipData[currentMip][resultBufferCntr++] = ((alpha & 0x000000FF));
				}
			}

			delete[] indexInPalleteBuffer;
		}
		break;

		case BLPColorEncoding::COLOR_DXT:
		{
			uint8_t* buf = new uint8_t[header.mipSizes[currentMip]];
			memcpy_s(buf, header.mipSizes[currentMip], &inputBuffer[0] + header.mipOffsets[currentMip], header.mipSizes[currentMip]);

			view->MipData[currentMip] = new uint8_t[mipWidth * mipHeight * 4u];
			switch (header.pixelFormat)
			{
			case BLPPixelFormat::PIXEL_DXT1:
				DDSDecompressDXT1(buf, mipWidth, mipHeight, view->MipData[currentMip]);
				break;
			case BLPPixelFormat::PIXEL_DXT3:
				DDSDecompressDXT3(buf, mipWidth, mipHeight, view->MipData[currentMip]);
				break;
			case BLPPixelFormat::PIXEL_DXT5:
				DDSDecompressDXT5(buf, mipWidth, mipHeight, view->MipData[currentMip]);
				break;
			default:
				return LIBBLP_ERROR_FORMAT;
			}

			delete[] buf;
		}
		break;

		case BLPColorEncoding::COLOR_ARGB8888:
		{
			view->MipData[currentMip] = new uint8_t[header.mipSizes[currentMip]];
			memcpy_s(view->MipData[currentMip], header.mipSizes[currentMip], &inputBuffer[0] + header.mipOffsets[currentMip], header.mipSizes[currentMip]);
		}
		break;

		default:
			return LIBBLP_ERROR_FORMAT;
		}
	}

	return LIBBLP_SUCCESS;
}