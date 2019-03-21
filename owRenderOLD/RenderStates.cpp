#include "stdafx.h"

// General
#include "RenderStates.h"

// Additional
#include "Render.h"

RenderState::RenderState() :
	m_StatePendingMask(0)
{
	m_ViewportX = 0; m_ViewportY = 0; m_ViewportWidth = 320; m_ViewportHeight = 240;
	m_ScissorX = 0; m_ScissorY = 0; m_ScissorWidth = 320; m_ScissorHeight = 240;
	
	m_CurrentGeometry = nullptr;
	m_StatePendingMask |= PM_GEOMETRY;

	for (uint32 i = 0; i < 16; ++i)
	{
		setTexture(i, nullptr, 0, 0);
	}
	m_MemoryBarriers = NotSet;
	m_StatePendingMask |= PM_BARRIER;

	// Default raster state
	m_NewRasterState.fillMode = R_FillMode::RS_FILL_SOLID;
	m_NewRasterState.cullMode = R_CullMode::RS_CULL_BACK;
	m_NewRasterState.scissorEnable = false;
	m_NewRasterState.multisampleEnable = false;
	m_NewRasterState.renderTargetWriteMask = true;
	m_StatePendingMask |= PM_RENDERSTATES;

	// Default blend
	m_NewBlendState.alphaToCoverageEnable = false;
	m_NewBlendState.blendEnable = false;
	m_NewBlendState.srcRGBBlendFunc = R_BlendFunc::BS_BLEND_ZERO;
	m_NewBlendState.dstRGBBlendFunc = R_BlendFunc::BS_BLEND_ZERO;
	m_NewBlendState.srcABlendFunc = R_BlendFunc::BS_BLEND_ZERO;
	m_NewBlendState.dstABlendFunc = R_BlendFunc::BS_BLEND_ZERO;
	m_StatePendingMask |= PM_RENDERSTATES;

	// Default depth
	m_NewDepthStencilState.depthWriteMask = true;
	m_NewDepthStencilState.depthEnable = true;
	m_NewDepthStencilState.depthFunc = R_DepthFunc::DSS_DEPTHFUNC_LESS_EQUAL;
	m_StatePendingMask |= PM_RENDERSTATES;

	m_TesselationPatchVerts = 1;
}