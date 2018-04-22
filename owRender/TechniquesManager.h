#pragma once

// Log
#include "Debug_GeometryPass.h"
#include "Debug_Normals.h"

#include "Sky_GeometryPass.h"
#include "MapTileLowRes_GeometryPass.h"
#include "MapChunk_GeometryPass.h"
#include "WMO_GeomertyPass.h"
#include "Water_Pass.h"
#include "Model_Shader.h"

// After
#include "PostProcess_Common.h"
#include "POST_DirectionalLight.h"
#include "POST_Fog.h"
#include "POST_Simple.h"

// UI
#include "UI_Color.h"
#include "UI_Font.h"
#include "UI_Texture.h"

class TechniquesMgr
{
	CLASS_INSTANCE(TechniquesMgr);

	TechniquesMgr();
	~TechniquesMgr();

	void Init();

	Debug_GeometryPass* m_Debug_GeometryPass;
    Debug_Normals* m_Debug_Normals;

	Sky_GeometryPass* m_Sky_GeometryPass;
	MapChunk_GeometryPass* m_MapChunk_GeometryPass;
	MapTileLowRes_GeometryPass* m_MapTileLowRes_GeometryPass;
	WMO_GeomertyPass* m_WMO_GeometryPass;
	Water_Pass* m_Water;
	Model_Shader* m_Model;

	// Lights

	POST_DirectionalLight* m_POST_DirectionalLight;
    POST_Fog* m_POST_Fog;
	POST_Simple* m_POST_Simple;

	// UI
	UI_Color* m_UI_Color;
	UI_Font* m_UI_Font;
	UI_Texture* m_UI_Texture;
};

#define _TechniquesMgr TechniquesMgr::instance()