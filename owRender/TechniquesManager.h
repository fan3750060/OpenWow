#pragma once

// Debug
#include "Debug_GeometryPass.h"
#include "Debug_Normals.h"

// Map
#include "MCNK_Pass.h"
#include "MCNK_Divided_Pass.h"
#include "WDL_LowRes_Pass.h"

// M2
#include "M2_Pass.h"
#include "M2_RibbonEmitters_Pass.h"
#include "M2_Particle_Pass.h"

// Liquids
#include "Magma_Pass.h"
#include "Water_Pass.h"

// Others
#include "Sky_GeometryPass.h"
#include "WMO_GeomertyPass.h"

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

	void PreRender3D(Camera* _camera, R_RenderBuffer* _rb);

	// Debug
	SharedPtr<CDebug_GeometryPass> Debug_Pass;
	SharedPtr<CDebug_Normals> DebugNormal_Pass;

	// Map
	SharedPtr<CMCNK_Pass> MCNK_Pass;
	//CMCNK_Divided_Pass> MCNK_Divided_Pass[4];
	SharedPtr<CWDL_LowRes_Pass> WDL_LowRes_Pass;

	// M2
	SharedPtr<CM2_Pass> M2_Pass;
	SharedPtr<CM2_RibbonEmitters_Pass> M2_RibbonEmitters_Pass;
	SharedPtr<CM2_Particle_Pass> M2_Particles_Pass;

	// Liquids
	SharedPtr<CMagma_Pass> m_Magma;
	SharedPtr<CWater_Pass> m_Water;

	// Others
	SharedPtr<CSky_GeometryPass> Sky_Pass;
	SharedPtr<CWMO_GeomertyPass> WMO_Pass;

	// Postprocess
	SharedPtr<CPOST_Fog> Postprocess_Fog;
	SharedPtr<CPOST_DirectionalLight> Postprocess_Light_Direction;
	SharedPtr<CPOST_Simple> Postprocess_Simple;

	// UI
	SharedPtr<CUI_Color> UI_Color;
	SharedPtr<CUI_Font> UI_Font;
	SharedPtr<CUI_Texture> UI_Texture;

private:
	vector<GeometryBase*>		m_GeomTechniques;
	vector<PostprocessBase*>	m_PostTechniques;

	RenderDevice*				m_RenderDevice;
	const CGroupVideo&			m_VideoSettings;
};
