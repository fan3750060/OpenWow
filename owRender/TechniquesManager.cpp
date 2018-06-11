#include "stdafx.h"

// General
#include "TechniquesManager.h"

// Additional
#include "Material.h"

TechniquesManager::TechniquesManager(RenderDevice* _RenderDevice)
	: m_RenderDevice(_RenderDevice)
{
	CGroupVideo& groupVideo = GetSettingsGroup<CGroupVideo>();

	m_Debug_GeometryPass = new Debug_GeometryPass(m_RenderDevice);

	m_Debug_Normals = new Debug_Normals(m_RenderDevice);

	m_Sky_GeometryPass = new Sky_GeometryPass(m_RenderDevice);

	//----------------------------------------------------------------//

	m_MapChunk_GeometryPass = new MCNK_Pass(m_RenderDevice);
	m_MapChunk_GeometryPass->Bind();
	for (uint8 i = 0; i < 4; i++)
	{
		m_MapChunk_GeometryPass->SetColorTextureUnit(i, i);
		m_MapChunk_GeometryPass->SetSpecularTextureUnit(i, 5 + i);
	}
	m_MapChunk_GeometryPass->SetBlendBuffer(4);
	m_MapChunk_GeometryPass->Unbind();

	//----------------------------------------------------------------//

	m_MapTileLowRes_GeometryPass = new WDL_LowRes_Pass(m_RenderDevice);

	//

	m_Magma = new Magma_Pass(m_RenderDevice);
	m_Magma->Bind();
	m_Magma->SetColorTextureUnit(Material::C_DiffuseTextureIndex);
	m_Magma->Unbind();

	//

	m_Water = new Water_Pass(m_RenderDevice);
	m_Water->Bind();
	m_Water->SetColorTextureUnit(Material::C_DiffuseTextureIndex);
	m_Water->SetSpecularTextureUnit(Material::C_SpecularTextureIndex);
	m_Water->Unbind();

	//----------------------------------------------------------------//

	m_Ribbons = new M2_RibbonEmitters_Pass(m_RenderDevice);
	m_Ribbons->Bind();
	m_Ribbons->SetColorTextureUnit(Material::C_DiffuseTextureIndex);
	m_Ribbons->Unbind();

	//----------------------------------------------------------------//

	m_WMO_GeometryPass = new WMO_GeomertyPass(m_RenderDevice);
	m_WMO_GeometryPass->Bind();
	m_WMO_GeometryPass->SetColorTextureUnit(Material::C_DiffuseTextureIndex);
	m_WMO_GeometryPass->SetSpecularTextureUnit(Material::C_SpecularTextureIndex);
	m_WMO_GeometryPass->Unbind();

	//----------------------------------------------------------------//

	m_Model = new M2_Pass(m_RenderDevice);
	m_Model->Bind();
	m_Model->SetDiffuseTexture(Material::C_DiffuseTextureIndex);
	m_Model->SetSpecularTexture(Material::C_SpecularTextureIndex);
	m_Model->Unbind();

	//----------------------------------------------------------------//

	m_POST_DirectionalLight = new POST_DirectionalLight(m_RenderDevice);
	m_POST_DirectionalLight->Bind();
	m_POST_DirectionalLight->BindToPostprocess();
	m_POST_DirectionalLight->SetScreenSize(groupVideo.windowSizeX, groupVideo.windowSizeY);
	m_POST_DirectionalLight->SetMatSpecularPower(32);
	m_POST_DirectionalLight->Unbind();

	//----------------------------------------------------------------//

	m_POST_Fog = new POST_Fog(m_RenderDevice);
	m_POST_Fog->Bind();
	m_POST_Fog->BindToPostprocess();
	m_POST_Fog->SetScreenSize(groupVideo.windowSizeX, groupVideo.windowSizeY);
	m_POST_Fog->Unbind();
	//----------------------------------------------------------------//

	m_POST_Simple = new POST_Simple(m_RenderDevice);
	m_POST_Simple->Bind();
	m_POST_Simple->BindToPostprocess();
	m_POST_Simple->SetScreenSize(groupVideo.windowSizeX, groupVideo.windowSizeY);
	m_POST_Simple->Unbind();

	//

	m_UI_Color = new UI_Color(m_RenderDevice);

	//

	m_UI_Font = new UI_Font(m_RenderDevice);
	m_UI_Font->Bind();
	m_UI_Font->SetFontTexture(Material::C_DiffuseTextureIndex);
	m_UI_Font->Unbind();

	//

	m_UI_Texture = new UI_Texture(m_RenderDevice);
	m_UI_Texture->Bind();
	m_UI_Texture->SetTexture(Material::C_DiffuseTextureIndex);
	m_UI_Texture->Unbind();
}

TechniquesManager::~TechniquesManager()
{

}
