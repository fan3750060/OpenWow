#include "stdafx.h"

// General
#include "ADT_MCNK_Pass.h"

ADT_MCNK_Pass::ADT_MCNK_Pass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline)
	: BasePass(scene, pipeline)
	, m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
}

ADT_MCNK_Pass::~ADT_MCNK_Pass()
{}

bool ADT_MCNK_Pass::Visit(IMesh& mesh)
{
	if (mesh.GetType() == SN_TYPE_ADT_CHUNK && m_QualitySettings.draw_mcnk)
	{
		return mesh.Render(GetRenderEventArgs(), GetPerObjectConstantBuffer());
	}

	return false;
}
