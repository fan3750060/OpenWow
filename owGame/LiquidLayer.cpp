#include "stdafx.h"

// General
#include "LiquidLayer.h"

Liquid_Layer::Liquid_Layer(std::shared_ptr<IMesh> _mesh)
	: MeshWrapper(SN_TYPE_LQ, _mesh)
	, m_SkyManager(GetManager<ISkyManager>())
	, m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	m_Material = std::make_shared<LiquidMaterial>();

	SetMaterial(m_Material);
}

Liquid_Layer::~Liquid_Layer()
{
}

bool Liquid_Layer::Render(RenderEventArgs & renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation, UINT indexCnt, UINT vertexStartLocation, UINT vertexCnt)
{
	uint32_t texidx = (uint32_t)(EngineTime::GetTotalTime() * 1000.0f / 60.0f) % m_Textures.size();
	m_Material->SetTexture(0, m_Textures[texidx]);

	if (m_SkyManager != nullptr)
	{
		m_Material->SetColorLight(m_SkyManager->GetColor(LightColors::LIGHT_COLOR_RIVER_LIGHT));
		m_Material->SetColorDark(m_SkyManager->GetColor(LightColors::LIGHT_COLOR_RIVER_DARK));
		m_Material->SetShallowAlpha(m_SkyManager->GetWaterShallowAlpha());
		m_Material->SetDeepAlpha(m_SkyManager->GetWaterDarkAlpha());
	}
	else
	{
		m_Material->SetColorLight(vec3(0.0f, 0.0f, 1.0f));
		m_Material->SetColorDark(vec3(0.0f, 0.0f, 1.0f));
		m_Material->SetShallowAlpha(1.0f);
		m_Material->SetDeepAlpha(1.0f);
	}

	return MeshWrapper::Render(renderEventArgs, perObject, indexStartLocation, indexCnt, vertexStartLocation, vertexCnt);
}

void Liquid_Layer::InitTextures()
{
	std::string baseName = LiquidType->Get_Texture(0);

	char buf[256];
	for (int i = 1; i <= 30; i++)
	{
		sprintf_s(buf, baseName.c_str(), i);
		std::shared_ptr<Texture> texture = _RenderDevice->CreateTexture2D(buf);
		m_Textures.push_back(texture);
	}
}
