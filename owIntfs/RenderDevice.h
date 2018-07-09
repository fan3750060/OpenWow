#pragma once

#include "BaseManager.h"
/*
class R_GeometryInfo;
enum R_DrawBarriers;
class R_Buffer;
enum R_FillMode;
enum R_CullMode;
enum R_BlendFunc;
enum R_DepthFunc;

struct 
	__declspec(uuid("D5E224F6-0034-4760-94A3-6CD9717190B5"))
	IRenderDevice : public IManager
{
	// -----------------------------------------------------------------------------
	// Commands
	// -----------------------------------------------------------------------------
	void setViewport(int x, int y, int width, int height);
	void setScissorRect(int x, int y, int width, int height);
	void setGeometry(R_GeometryInfo* geoIndex);
	void setTexture(uint32 slot, R_Texture* texObj, uint16 samplerState, uint16 usage);
	void setMemoryBarrier(R_DrawBarriers barrier);
	void setStorageBuffer(uint8 slot, R_Buffer* bufObj);

	// Render states
	void setColorWriteMask(bool enabled);
	void getColorWriteMask(bool &enabled) const;

	void setFillMode(R_FillMode fillMode);
	void getFillMode(R_FillMode &fillMode) const;
		
	void setCullMode(R_CullMode cullMode);
	void getCullMode(R_CullMode &cullMode) const;


	void setScissorTest(bool enabled);
	void getScissorTest(bool &enabled) const;


	void setMulisampling(bool enabled);
	void getMulisampling(bool &enabled) const;


	void setAlphaToCoverage(bool enabled);
	void getAlphaToCoverage(bool &enabled) const;


	void setBlendMode(bool enabled, R_BlendFunc srcRGBBlendFunc, R_BlendFunc dstRGBBlendFunc);
	void setBlendModeEx(bool enabled, R_BlendFunc srcRGBBlendFunc, R_BlendFunc dstRGBBlendFunc, R_BlendFunc srcABlendFunc, R_BlendFunc dstABlendFunc);
	void getBlendMode(bool &enabled, R_BlendFunc& srcRGBBlendFunc, R_BlendFunc& dstRGBBlendFunc) const;

	void setDepthMask(bool enabled);
	void getDepthMask(bool &enabled) const;

	void setDepthTest(bool enabled);
	void getDepthTest(bool &enabled) const;

	void setDepthFunc(R_DepthFunc depthFunc);
	void getDepthFunc(R_DepthFunc &depthFunc) const;

	void setTessPatchVertices(uint16 verts);
};*/