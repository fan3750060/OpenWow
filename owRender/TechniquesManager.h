#pragma once

#include "Debug_GeometryPass.h"
#include "Debug_Normals.h"

#include "Sky_GeometryPass.h"
#include "WMO_GeomertyPass.h"



// Map
#include "MCNK_Pass.h"
#include "WDL_LowRes_Pass.h"

// M2
#include "M2_Pass.h"
#include "M2_RibbonEmitters_Pass.h"

// Liquids
#include "Magma_Pass.h"
#include "Water_Pass.h"

// Postprocess
#include "Postprocess_Fog.h"
#include "Postprocess_Light_Direction.h"
#include "Postprocess_Simple.h"

// UI
#include "UI_Color.h"
#include "UI_Font.h"
#include "UI_Texture.h"

class TechniquesManager
{
public:
	TechniquesManager(RenderDevice* _RenderDevice);
	~TechniquesManager();

public:
	Debug_GeometryPass* m_Debug_GeometryPass;
    Debug_Normals* m_Debug_Normals;

	Sky_GeometryPass* m_Sky_GeometryPass;
	MCNK_Pass* m_MapChunk_GeometryPass;
	WDL_LowRes_Pass* m_MapTileLowRes_GeometryPass;
	WMO_GeomertyPass* m_WMO_GeometryPass;
	Magma_Pass* m_Magma;
	Water_Pass* m_Water;
	M2_Pass* m_Model;
	M2_RibbonEmitters_Pass* m_Ribbons;

	// Lights

	POST_DirectionalLight* m_POST_DirectionalLight;
    POST_Fog* m_POST_Fog;
	POST_Simple* m_POST_Simple;

	// UI
	UI_Color* m_UI_Color;
	UI_Font* m_UI_Font;
	UI_Texture* m_UI_Texture;

private:
	RenderDevice* m_RenderDevice;
};