#include "stdafx.h"

// General
#include "TechniquesManager.h"

// Additional
#include "Material.h"
#include "Render.h"

TechniquesManager::TechniquesManager(RenderDevice* _RenderDevice) : 
	m_RenderDevice(_RenderDevice), 
	m_VideoSettings(GetSettingsGroup<CGroupVideo>())
{
	// Debug

	Debug_Pass = new CDebug_GeometryPass(m_RenderDevice);
	m_GeomTechniques.push_back(Debug_Pass);

	DebugNormal_Pass = new CDebug_Normals(m_RenderDevice);
	m_GeomTechniques.push_back(DebugNormal_Pass);

	// Map

	MCNK_Pass = new CMCNK_Pass(m_RenderDevice);
	MCNK_Pass->Bind();
	for (uint8 i = 0; i < 4; i++)
	{
		MCNK_Pass->SetColorTextureUnit(i, i);
		MCNK_Pass->SetSpecularTextureUnit(i, i);
	}
	MCNK_Pass->SetBlendBuffer();
	MCNK_Pass->Unbind();
	m_GeomTechniques.push_back(MCNK_Pass);

	/*for (uint8 i = 0; i < 4; i ++)
	{
		MCNK_Divided_Pass[i] = new CMCNK_Divided_Pass(m_RenderDevice, i);
		MCNK_Divided_Pass[i]->Bind();
		for (uint8 j = 0; j < i + 1; j++)
		{
			MCNK_Divided_Pass[i]->SetColorTextureUnit(j);
			MCNK_Divided_Pass[i]->SetSpecularTextureUnit(j);
		}
		MCNK_Divided_Pass[i]->SetBlendBuffer(CMCNK_Divided_Pass::C_Blend);
		MCNK_Divided_Pass[i]->Unbind();
		m_GeomTechniques.push_back(MCNK_Divided_Pass[i]);
	}*/

	WDL_LowRes_Pass = new CWDL_LowRes_Pass(m_RenderDevice);
	m_GeomTechniques.push_back(WDL_LowRes_Pass);

	// M2

	M2_Pass = new CM2_Pass(m_RenderDevice);
	M2_Pass->Bind();
	M2_Pass->SetDiffuseTexture(0, Material::C_DiffuseTextureIndex + 0);
	M2_Pass->SetDiffuseTexture(1, Material::C_DiffuseTextureIndex + 1);
	M2_Pass->SetSpecularTexture(Material::C_SpecularTextureIndex);
	M2_Pass->Unbind();
	m_GeomTechniques.push_back(M2_Pass);

	// M2 ribbons

	M2_RibbonEmitters_Pass = new CM2_RibbonEmitters_Pass(m_RenderDevice);
	M2_RibbonEmitters_Pass->Bind();
	M2_RibbonEmitters_Pass->SetColorTextureUnit(Material::C_DiffuseTextureIndex);
	M2_RibbonEmitters_Pass->Unbind();
	m_GeomTechniques.push_back(M2_RibbonEmitters_Pass);
	
	// M2 particles

	M2_Particles_Pass = new CM2_Particle_Pass(m_RenderDevice);
	M2_Particles_Pass->Bind();
	M2_Particles_Pass->SetColorTextureUnit(Material::C_DiffuseTextureIndex);
	M2_Particles_Pass->Unbind();
	m_GeomTechniques.push_back(M2_Particles_Pass);
	
	// Liquids

	m_Magma = new CMagma_Pass(m_RenderDevice);
	m_Magma->Bind();
	m_Magma->SetColorTextureUnit(Material::C_DiffuseTextureIndex);
	m_Magma->Unbind();
	m_GeomTechniques.push_back(m_Magma);

	m_Water = new CWater_Pass(m_RenderDevice);
	m_Water->Bind();
	m_Water->SetColorTextureUnit(Material::C_DiffuseTextureIndex);
	m_Water->SetSpecularTextureUnit(Material::C_SpecularTextureIndex);
	m_Water->Unbind();
	m_GeomTechniques.push_back(m_Water);

	// Sky

	Sky_Pass = new CSky_GeometryPass(m_RenderDevice);
	m_GeomTechniques.push_back(Sky_Pass);

	// WMO

	WMO_Pass = new CWMO_GeomertyPass(m_RenderDevice);
	WMO_Pass->Bind();
	WMO_Pass->SetColorTextureUnit(0, Material::C_DiffuseTextureIndex + 0);
	WMO_Pass->SetColorTextureUnit(1, Material::C_DiffuseTextureIndex + 1);
	WMO_Pass->SetColorTextureUnit(2, Material::C_DiffuseTextureIndex + 2);
	WMO_Pass->SetSpecularTextureUnit(Material::C_SpecularTextureIndex);
	WMO_Pass->Unbind();
	m_GeomTechniques.push_back(WMO_Pass);

	// Postprocess

	Postprocess_Light_Direction = new CPOST_DirectionalLight(m_RenderDevice);
	Postprocess_Light_Direction->Bind();
	Postprocess_Light_Direction->setScreenSize(m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY);
	Postprocess_Light_Direction->SetMatSpecularPower(16);
	Postprocess_Light_Direction->Unbind();
	m_PostTechniques.push_back(Postprocess_Light_Direction);

	Postprocess_Fog = new CPOST_Fog(m_RenderDevice);
	Postprocess_Fog->Bind();
	Postprocess_Fog->setScreenSize(m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY);
	Postprocess_Fog->Unbind();
	m_PostTechniques.push_back(Postprocess_Fog);

	Postprocess_Simple = new CPOST_Simple(m_RenderDevice);
	Postprocess_Simple->Bind();
	Postprocess_Simple->setScreenSize(m_VideoSettings.windowSizeX, m_VideoSettings.windowSizeY);
	Postprocess_Simple->Unbind();
	m_PostTechniques.push_back(Postprocess_Simple);

	// UI

	UI_Color = new CUI_Color(m_RenderDevice);

	UI_Font = new CUI_Font(m_RenderDevice);
	UI_Font->Bind();
	UI_Font->SetFontTexture(Material::C_DiffuseTextureIndex);
	UI_Font->Unbind();

	UI_Texture = new CUI_Texture(m_RenderDevice);
	UI_Texture->Bind();
	UI_Texture->SetTexture(Material::C_DiffuseTextureIndex);
	UI_Texture->Unbind();
}

TechniquesManager::~TechniquesManager()
{
	delete Debug_Pass;
	delete DebugNormal_Pass;

	// Map
	delete MCNK_Pass;
	delete WDL_LowRes_Pass;

	// M2
	delete M2_Pass;
	delete M2_RibbonEmitters_Pass;

	// Liquids
	delete m_Magma;
	delete m_Water;

	// Others
	delete Sky_Pass;
	delete WMO_Pass;

	// Postprocess
	delete Postprocess_Fog;
	delete Postprocess_Light_Direction;
	delete Postprocess_Simple;

	// UI
	delete UI_Color;
	delete UI_Font;
	delete UI_Texture;
}

void TechniquesManager::PreRender3D(Camera* _camera, R_RenderBuffer* _rb)
{
	for (auto& it : m_GeomTechniques)
	{
		it->Bind();
		it->setProjView(_camera->getProjMat() * _camera->getViewMat());
		it->Unbind();
	}

	int32 width, height;
	_rb->getRenderBufferDimensions(&width, &height);

	for (auto& it : m_PostTechniques)
	{
		it->Bind();
		it->setCameraPos(_camera->Position);
		it->setScreenSize(width, height);
		it->Unbind();
	}
}
