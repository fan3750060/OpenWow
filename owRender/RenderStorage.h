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

	void CreateLayouts();
	void CreateGeometry();
	void CreateWoWLayouts();

	void SetEGxBlend(uint8 _index);
	
    //----------------------------------------------
    // Layouts
    //----------------------------------------------

    // Layouts geometry
    uint32 __layoutSky;
    uint32 __layoutWMO;
    uint32 __layoutWMO_VC;
    uint32 __layoutWater;

    uint32 __layout_GxVBF_P;
    uint32 __layout_GxVBF_PN;
	uint32 __layout_GxVBF_PNT2;
    uint32 __layout_GxVBF_PÑ;
    uint32 __layout_GxVBF_PT;
    uint32 __layout_GxVBF_PBNT2;  // MDX
    uint32 __layout_GxVBF_PNC2T2; // Tiles

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