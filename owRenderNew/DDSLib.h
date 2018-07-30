#pragma once

extern "C"
{
	int DDSDecompressDXT5(unsigned char *src, int width, int height, unsigned char *dest);
	int DDSDecompressDXT3(unsigned char *src, int width, int height, unsigned char *dest);
	int DDSDecompressDXT1(unsigned char *src, int width, int height, unsigned char *dest);
}