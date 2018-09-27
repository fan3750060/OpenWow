#pragma once

extern "C"
{
	void DDSDecompressDXT5(unsigned char *src, int width, int height, unsigned char *dest);
	void DDSDecompressDXT3(unsigned char *src, int width, int height, unsigned char *dest);
	void DDSDecompressDXT1(unsigned char *src, int width, int height, unsigned char *dest);
}