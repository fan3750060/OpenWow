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
	std::shared_ptr<CDebug_GeometryPass> Debug_Pass;
	std::shared_ptr<CDebug_Normals> DebugNormal_Pass;

	// Map
	std::shared_ptr<CMCNK_Pass> MCNK_Pass;
	//CMCNK_Divided_Pass> MCNK_Divided_Pass[4];
	std::shared_ptr<CWDL_LowRes_Pass> WDL_LowRes_Pass;

	// M2
	std::shared_ptr<CM2_Pass> M2_Pass;
	std::shared_ptr<CM2_RibbonEmitters_Pass> M2_RibbonEmitters_Pass;
	std::shared_ptr<CM2_Particle_Pass> M2_Particles_Pass;

	// Liquids
	std::shared_ptr<CMagma_Pass> m_Magma;
	std::shared_ptr<CWater_Pass> m_Water;

	// Others
	std::shared_ptr<CSky_GeometryPass> Sky_Pass;
	std::shared_ptr<CWMO_GeomertyPass> WMO_Pass;

	// Postprocess
	std::shared_ptr<CPOST_Fog> Postprocess_Fog;
	std::shared_ptr<CPOST_DirectionalLight> Postprocess_Light_Direction;
	std::shared_ptr<CPOST_Simple> Postprocess_Simple;

	// UI
	std::shared_ptr<CUI_Color> UI_Color;
	std::shared_ptr<CUI_Font> UI_Font;
	std::shared_ptr<CUI_Texture> UI_Texture;

private:
	vector<std::shared_ptr<GeometryBase>>		m_GeomTechniques;
	vector<std::shared_ptr<PostprocessBase>>	m_PostTechniques;

	RenderDevice*				m_RenderDevice;
	const CGroupVideo&			m_VideoSettings;
};
