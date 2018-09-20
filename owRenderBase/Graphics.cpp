#include "stdafx.h"

#include "Graphics.h"

Graphics* g_pGrahics = NULL;

Graphics& Graphics::Instance()
{
	assert( g_pGrahics != NULL );
	return *g_pGrahics;
}

void Graphics::CreateInstance()
{
	assert(g_pGrahics == NULL);
	g_pGrahics = new Graphics();
}

void Graphics::DestroyInstance()
{
	assert( g_pGrahics != NULL );
	delete g_pGrahics;
}

Graphics::Graphics(void)
{

}


Graphics::~Graphics(void)
{
}
