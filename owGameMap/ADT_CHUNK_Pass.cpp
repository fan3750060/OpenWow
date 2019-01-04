#include "stdafx.h"

// General
#include "ADT_CHUNK_Pass.h"

ADT_CHUNK_Pass::ADT_CHUNK_Pass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline)
	: BasePass(scene, pipeline),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
}

ADT_CHUNK_Pass::~ADT_CHUNK_Pass()
{}

bool ADT_CHUNK_Pass::Visit(IMesh& mesh)
{
	if (mesh.GetType() == SN_TYPE_ADT_CHUNK && m_QualitySettings.draw_mcnk)
	{
		mesh.Render(GetRenderEventArgs(), GetConstantBuffer());
		return true;
	}

	return false;
}
