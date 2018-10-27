#pragma once

struct RenderState
{
	RenderState();

	void setViewport(int x, int y, int width, int height)
	{
		m_ViewportX = x;
		m_ViewportY = y;
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		m_StatePendingMask |= PM_VIEWPORT;
	}
	void setScissorRect(int x, int y, int width, int height)
	{
		m_ScissorX = x;
		m_ScissorY = y;
		m_ScissorWidth = width;
		m_ScissorHeight = height;
		m_StatePendingMask |= PM_SCISSOR;
	}
	void setGeometry(std::shared_ptr<IMesh> geoIndex)
	{
		m_CurrentGeometry = geoIndex;
		m_StatePendingMask |= PM_GEOMETRY;
	}
	void setTexture(uint32 slot, std::shared_ptr<Texture> texObj, uint16 samplerState, uint16 usage)
	{
		_ASSERT(slot < 16);
		m_TextureSlot[slot] = R_TexSlot(texObj, samplerState, usage);
		m_StatePendingMask |= PM_TEXTURES;
	}
	void setMemoryBarrier(R_DrawBarriers barrier)
	{
		m_MemoryBarriers = barrier;
		m_StatePendingMask |= PM_BARRIER;
	}
	void setStorageBuffer(uint8 slot, R_Buffer* bufObj)
	{
		//_ASSERT(slot < _maxComputeBufferAttachments && m_StorageBufs.size() < _maxComputeBufferAttachments);

		R_Buffer* buf = bufObj;
		m_StorageBufs.push_back(R_ShaderStorage(slot, buf->m_GLObj));

		m_StatePendingMask |= PM_COMPUTE;
	}

	// Render states
	void setColorWriteMask(bool enabled)
	{
		m_NewRasterState.renderTargetWriteMask = enabled;
		m_StatePendingMask |= PM_RENDERSTATES;
	}
	void getColorWriteMask(bool &enabled) const
	{
		enabled = m_NewRasterState.renderTargetWriteMask;
	}

	void setFillMode(R_FillMode fillMode)
	{
		m_NewRasterState.fillMode = fillMode;
		m_StatePendingMask |= PM_RENDERSTATES;
	}
	void getFillMode(R_FillMode &fillMode) const
	{
		fillMode = (R_FillMode)m_NewRasterState.fillMode;
	}

	void setCullMode(R_CullMode cullMode)
	{
		m_NewRasterState.cullMode = cullMode;
		m_StatePendingMask |= PM_RENDERSTATES;
	}
	void getCullMode(R_CullMode &cullMode) const
	{
		cullMode = (R_CullMode)m_NewRasterState.cullMode;
	}

	void setScissorTest(bool enabled)
	{
		m_NewRasterState.scissorEnable = enabled;
		m_StatePendingMask |= PM_RENDERSTATES;
	}
	void getScissorTest(bool &enabled) const
	{
		enabled = m_NewRasterState.scissorEnable;
	}

	void setMulisampling(bool enabled)
	{
		m_NewRasterState.multisampleEnable = enabled;
		m_StatePendingMask |= PM_RENDERSTATES;
	}
	void getMulisampling(bool &enabled) const
	{
		enabled = m_NewRasterState.multisampleEnable;
	}

	void setAlphaToCoverage(bool enabled)
	{
		m_NewBlendState.alphaToCoverageEnable = enabled;
		m_StatePendingMask |= PM_RENDERSTATES;
	}
	void getAlphaToCoverage(bool &enabled) const
	{
		enabled = m_NewBlendState.alphaToCoverageEnable;
	}

	void setBlendMode(bool enabled, R_BlendFunc srcBlendFunc = BS_BLEND_ZERO, R_BlendFunc dstBlendFunc = BS_BLEND_ZERO)
	{
		m_NewBlendState.blendEnable = enabled;

		m_NewBlendState.srcRGBBlendFunc = srcBlendFunc;
		m_NewBlendState.dstRGBBlendFunc = dstBlendFunc;
		m_NewBlendState.srcABlendFunc = srcBlendFunc;
		m_NewBlendState.dstABlendFunc = dstBlendFunc;

		m_StatePendingMask |= PM_RENDERSTATES;
	}
	void getBlendMode(bool &enabled, R_BlendFunc &srcRGBBlendFunc, R_BlendFunc &dstRGBBlendFunc) const
	{
		enabled = m_NewBlendState.blendEnable;
		srcRGBBlendFunc = (R_BlendFunc)m_NewBlendState.srcRGBBlendFunc;
		dstRGBBlendFunc = (R_BlendFunc)m_NewBlendState.dstRGBBlendFunc;
	}
	void setBlendModeEx(bool enabled, R_BlendFunc srcRGBBlendFunc = BS_BLEND_ZERO, R_BlendFunc dstRGBBlendFunc = BS_BLEND_ZERO, R_BlendFunc srcABlendFunc = BS_BLEND_ZERO, R_BlendFunc dstABlendFunc = BS_BLEND_ZERO)
	{
		m_NewBlendState.blendEnable = enabled;

		m_NewBlendState.srcRGBBlendFunc = srcRGBBlendFunc;
		m_NewBlendState.dstRGBBlendFunc = dstRGBBlendFunc;
		m_NewBlendState.srcABlendFunc = srcABlendFunc;
		m_NewBlendState.dstABlendFunc = dstABlendFunc;

		m_StatePendingMask |= PM_RENDERSTATES;
	}
	void getBlendModeEx(bool &enabled, R_BlendFunc &srcRGBBlendFunc, R_BlendFunc &dstRGBBlendFunc, R_BlendFunc &srcABlendFunc, R_BlendFunc &dstABlendFunc) const
	{
		enabled = m_NewBlendState.blendEnable;
		srcRGBBlendFunc = (R_BlendFunc)m_NewBlendState.srcRGBBlendFunc;
		dstRGBBlendFunc = (R_BlendFunc)m_NewBlendState.dstRGBBlendFunc;
		srcABlendFunc = (R_BlendFunc)m_NewBlendState.srcABlendFunc;
		dstABlendFunc = (R_BlendFunc)m_NewBlendState.dstABlendFunc;
	}

	void setDepthMask(bool enabled)
	{
		m_NewDepthStencilState.depthWriteMask = enabled;
		m_StatePendingMask |= PM_RENDERSTATES;
	}
	void getDepthMask(bool &enabled) const
	{
		enabled = m_NewDepthStencilState.depthWriteMask;
	}

	void setDepthTest(bool enabled)
	{
		m_NewDepthStencilState.depthEnable = enabled;
		m_StatePendingMask |= PM_RENDERSTATES;
	}
	void getDepthTest(bool &enabled) const
	{
		enabled = m_NewDepthStencilState.depthEnable;
	}

	void setDepthFunc(R_DepthFunc depthFunc)
	{
		m_NewDepthStencilState.depthFunc = depthFunc;
		m_StatePendingMask |= PM_RENDERSTATES;
	}
	void getDepthFunc(R_DepthFunc &depthFunc) const
	{
		depthFunc = (R_DepthFunc)m_NewDepthStencilState.depthFunc;
	}

	void setTessPatchVertices(uint16 verts)
	{
		m_TesselationPatchVerts = verts;
		m_StatePendingMask |= PM_RENDERSTATES;
	}

	int							m_ViewportX, m_ViewportY, m_ViewportWidth, m_ViewportHeight;
	int							m_ScissorX, m_ScissorY, m_ScissorWidth, m_ScissorHeight;
	std::shared_ptr<IMesh>				m_CurrentGeometry;
	R_TexSlot					m_TextureSlot[16];
	R_DrawBarriers				m_MemoryBarriers;

	vector<R_ShaderStorage>		m_StorageBufs;

	R_RasterState				m_NewRasterState;
	R_BlendState				m_NewBlendState;
	R_DepthStencilState			m_NewDepthStencilState;

	uint32						m_TesselationPatchVerts; // number of vertices in patch. Used for tesselation.

	//--
	uint32						m_StatePendingMask;
};