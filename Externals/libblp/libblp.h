#pragma once

#include <stdint.h>

#define LIBBLP_MIPMAP_COUNT 16
#define LIBBLP_PALETTE_SIZE 256

/* define errors. */
#define LIBBLP_SUCCESS				     0		/* success. */
#define LIBBLP_ERROR_OPEN				-1		/* open error on file. */
#define LIBBLP_ERROR_CLOSE				-2		/* close error on file. */
#define LIBBLP_ERROR_SEEK				-3		/* lseek error on file. */
#define LIBBLP_ERROR_MALLOC				-6		/* memory allocation error. */
#define LIBBLP_ERROR_FORMAT				-7		/* format errror. */
#define LIBBLP_ERROR_SIZE				-9		/* buffer size is to small. */

struct LIBBLP_PixelView
{
	LIBBLP_PixelView()
	{
		for (uint32_t i = 0; i < LIBBLP_MIPMAP_COUNT; i++)
		{
			MipData[i] = nullptr;
			MipWidth[i] = 0;
			MipHeight[i] = 0;
		}
		MipCount = 0;

		HasAlpha = false;
		IsTexture3D = false;
	}
	~LIBBLP_PixelView()
	{
		for (uint32_t i = 0; i < LIBBLP_MIPMAP_COUNT; i++)
		{
			if (MipData[i] != nullptr)
			{
				delete[] MipData[i];
				MipData[i] = nullptr;
			}
		}
	}

	uint8_t* MipData[LIBBLP_MIPMAP_COUNT];
	uint32_t MipWidth[LIBBLP_MIPMAP_COUNT];
	uint32_t MipHeight[LIBBLP_MIPMAP_COUNT];
	uint32_t MipCount;

	bool     HasAlpha;
	bool     IsTexture3D;
};

int LIBBLP_Load(const uint8_t* inputBuffer, uint32_t inputBufferSize, LIBBLP_PixelView * view);