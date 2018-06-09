#pragma once

struct WMO_HeaderDef
{
	uint32 nTextures;
	uint32 nGroups;
	uint32 nPortals;
	uint32 nLights;
	uint32 nDoodadNames;
	uint32 nDoodadDefs;
	uint32 nDoodadSets;
	CArgb ambColor;
	__DBC_FOREIGN_KEY(uint32, DBC_WMOAreaTable, WMOID, wmoID);
	CAaBox bounding_box;

	struct
	{
		uint16 FLAG_attenuate_vertices_based_on_distance_to_portal : 1;
		uint16 FLAG_skip_base_color : 1;                       // do not add base (ambient) color (of MOHD) to MOCVs. apparently does more, e.g. required for multiple MOCVs
		uint16 FLAG_use_liquid_type_dbc_id : 1;                // use real liquid type ID from DBCs instead of local one. See MLIQ for further reference.
		uint16 FLAG_lighten_interiors : 1;                     // makes iterior m_Groups much brighter, effects MOCV rendering. Used e.g.in Stormwind for having shiny bright interiors,
		uint16 : 12;
	} flags;

	uint16 numLod;                                             // ≥ Legion (21108) includes base lod (→ numLod = 3 means '.wmo', 'lod0.wmo' and 'lod1.wmo')
};

struct WMOMaterialDef
{
	struct Flags
	{
		uint32 F_UNLIT : 1;                    // disable lighting logic in shader (but can still use vertex colors)
		uint32 F_UNFOGGED : 1;                 // disable fog shading (rarely used)
		uint32 F_UNCULLED : 1;                 // two-sided
		uint32 F_EXTLIGHT : 1;                 // darkened, the intern face of windows are flagged 0x08
		uint32 F_SIDN : 1;                     // (bright at night, unshaded) (used on windows and lamps in Stormwind, for example) (see emissive color)
		uint32 F_WINDOW : 1;                   // lighting related (flag checked in CMapObj::UpdateSceneMaterials)
		uint32 F_CLAMP_S : 1;                  // tex clamp S (force this material's m_DiffuseTextures to use clamp s addressing)
		uint32 F_CLAMP_T : 1;                  // tex clamp T (force this material's m_DiffuseTextures to use clamp t addressing)
		uint32 : 24;
	} flags;

	uint32 shader;                 // Index into CMapObj::s_wmoShaderMetaData. See below (shader types).
	uint32 blendMode;
	uint32 diffuseNameIndex;       // offset into MOTX
	CBgra emissive_color;        // emissive color; see below (emissive color)
	CBgra sidn_emissive_color;   // set at runtime; gets sidn-manipulated emissive color; see below (emissive color)
	uint32 envNameIndex;
	uint32 diffColor;
	uint32 ground_type;            //  &TerrainTypeRec::m_ID
	uint32 texture_2;
	uint32 color_2;
	uint32 flags_2;
	uint32 unk[4];
};

struct WMO_PortalDef
{
	uint16 startVertex;
	uint16 count;
	C4Plane plane;
};

struct WMO_PortalReferencesDef
{
	uint16 portalIndex;  // into MOPR
	uint16 groupIndex;   // the other one
	int16 side;          // positive or negative.
	uint16 filler;
};

struct WMO_VisibleBlockListDef
{
	uint16 firstVertex;
	uint16 count;
};

struct WMO_LightDef
{
	uint8 type;
	uint8 useAtten;
	uint8 unk0[2];
	uint32 color;
	vec3 pos;
	float intensity;
	float attenStart;
	float attenEnd;
	float unk1[4];
};

// Doodads

struct WMO_DoodadSetDef
{
	char name[20];  // Set name
	uint32 start;   // index of first doodad instance in this set
	uint32 size;    // number of doodad instances in this set
	uint32 unk0;     // always 0

	bool InSet(uint32 _index)
	{
		return ((_index >= start) && (_index < (start + size)));
	}
};

struct WMO_MODD_PlacementInfo
{
	struct
	{
		uint32 nameIndex : 24;          // reference offset into MODN
		uint32 flag_AcceptProjTex : 1;
		uint32 : 7;                     // unused as of 7.0.1.20994
	} flags;

	vec3 position;				// (X,Z,-Y)
	C4ImQuaternion orientation;	// (X, Y, Z, W)
	float scale;				// scale factor
	CBgra color;				// (B,G,R,A) diffuse lighting color, used in place of global diffuse from DBCs
};

struct WMO_FogDef
{
	struct
	{
		uint32 flag_infinite_radius : 1; // F_IEBLEND: Ignore radius in CWorldView::QueryCameraFog
		uint32 : 3;                      // unused as of 7.0.1.20994
				 uint32 flag_0x10 : 1;
				 uint32 : 27;                     // unused as of 7.0.1.20994
	} flags;

	vec3 position;
	float smallerRadius; // Smaller radius
	float largerRadius;  // Larger radius

						 // Fog
	struct
	{
		float end;			// Fog end
		float startScalar;	// Fog start multiplier (0..1)
		CBgra color;	// The back buffer is also cleared to this colour 
	} fog;

	// Underwater fog
	struct
	{
		float end;
		float startScalar;
		CBgra color;
	} underwater_fog;
};