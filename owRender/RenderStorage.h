#pragma once

#include "RenderDevice.h"

struct Texture_Vertex
{
	vec3 vertex;
	vec2 textureCoord;
};

class RenderStorage
{
public:
	RenderStorage(RenderDevice* _RenderDevice);
	~RenderStorage();

	void CreateGeometry();
	void CreateWoWLayouts();

	void SetEGxBlend(RenderState* _state, uint8 _index);
	
    //----------------------------------------------
    // Layouts
    //----------------------------------------------

    // Layouts geometry

    uint32 __layout_GxVBF_P;

    uint32 __layout_GxVBF_PN;
	uint32 __layout_GxVBF_PNC;
	uint32 __layout_GxVBF_PNT;
	uint32 __layout_GxVBF_PNCT;
	uint32 __layout_GxVBF_PNT2;
	uint32 __layout_GxVBF_PNCT2;
	// NO NORMAL
    uint32 __layout_GxVBF_PC;
	uint32 __layout_GxVBF_PCT;
	uint32 __layout_GxVBF_PCT2;
	// NO NORMAL AND COLOR
    uint32 __layout_GxVBF_PT;
	uint32 __layout_GxVBF_PT2;

    uint32 __layout_GxVBF_PBNT2;
    uint32 __layout_GxVBF_PNC2T2;

	//----------------------------------------------
	// Geometry & VertexBuffers
    //----------------------------------------------

    R_GeometryInfo* __QuadVTDynamic;

    // 2D

    R_GeometryInfo* __Quad;
    R_GeometryInfo* __QuadVT;
    R_Buffer* __vbQuadVTDynamic;

    // 3D

    R_GeometryInfo* _cubeGeo;
    R_GeometryInfo* _sphereGeo;
    R_GeometryInfo* _coneGeo;

    //----------------------------------------------
    // Index buffers
    //----------------------------------------------

    R_Buffer* __ibQuadDefault;

private:
	RenderDevice* m_RenderDevice;
};