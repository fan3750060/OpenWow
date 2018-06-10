#pragma once

#include __PACK_BEGIN

#pragma region WMO

// HEADER
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

	struct Flags
	{
		uint16 FLAG_attenuate_vertices_based_on_distance_to_portal : 1;
		uint16 FLAG_skip_base_color : 1;                       // do not add base (ambient) color (of MOHD) to MOCVs. apparently does more, e.g. required for multiple MOCVs
		uint16 FLAG_use_liquid_type_dbc_id : 1;                // use real liquid type ID from DBCs instead of local one. See MLIQ for further reference.
		uint16 FLAG_lighten_interiors : 1;                     // makes iterior m_Groups much brighter, effects MOCV rendering. Used e.g.in Stormwind for having shiny bright interiors,
		uint16 : 12;
	} flags;

	uint16 numLod;                                             // ≥ Legion (21108) includes base lod (→ numLod = 3 means '.wmo', 'lod0.wmo' and 'lod1.wmo')
};

struct WMO_MaterialDef
{
	struct Flags
	{
		uint32 DisableLighting : 1;                    // disable lighting logic in shader (but can still use vertex colors)
		uint32 DisableFog : 1;                 // disable fog shading (rarely used)
		uint32 IsTwoSided : 1;                 // two-sided
		uint32 F_EXTLIGHT : 1;                 // darkened, the intern face of windows are flagged 0x08
		uint32 F_SIDN : 1;                     // (bright at night, unshaded) (used on windows and lamps in Stormwind, for example) (see emissive color)
		uint32 F_WINDOW : 1;                   // lighting related (flag checked in CMapObj::UpdateSceneMaterials)
		uint32 TextureClampS : 1;                  // tex clamp S (force this material's m_DiffuseTextures to use clamp s addressing)
		uint32 TextureClampT : 1;                  // tex clamp T (force this material's m_DiffuseTextures to use clamp t addressing)
		uint32 : 24;
	} flags;

	uint32 shader;                 // Index into CMapObj::s_wmoShaderMetaData. See below (shader types).
	uint32 blendMode;
	uint32 diffuseNameIndex;       // offset into MOTX
	CBgra emissive_color;        // emissive color; see below (emissive color)
	CBgra sidn_emissive_color;   // set at runtime; gets sidn-manipulated emissive color; see below (emissive color)
	uint32 envNameIndex;
	uint32 diffColor;
	__DBC_FOREIGN_KEY_ID(uint32, DBC_TerrainType, ground_type);

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

struct WMO_MODD_SetInfo
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
	struct Flags
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
	struct Flags
	{
		uint32 flag_infinite_radius : 1; // F_IEBLEND: Ignore radius in CWorldView::QueryCameraFog
		uint32 : 31;
	} flags;

	vec3 position;
	float smallerRadius; // Smaller radius
	float largerRadius;  // Larger radius

	struct Fog
	{
		float end;			// Fog end
		float startScalar;	// Fog start multiplier (0..1)
		CBgra color;	// The back buffer is also cleared to this colour 
	} fog;

	struct UnderwaterFog
	{
		float end;
		float startScalar;
		CBgra color;
	} underwater_fog;
};

#pragma endregion


#pragma region WMO Group

struct WMOGroupFlags
{
	uint32 FLAG_HAS_COLLISION : 1;                  // = 0x1			//Has MOBN and MOBR chunk.
	uint32 FLAG_UNK_0x2 : 1;                        // = 0x2			// UNUSED
	uint32 FLAG_HAS_VERTEX_COLORS : 1;              // = 0x4			//Has vertex colors(MOCV chunk).
	uint32 FLAG_IS_OUTDOOR : 1;                     // = 0x8			//EXTERIOR -- Outdoor

	uint32 FLAG_UNK_0x10 : 1;                       // = 0x10			(UNUSED: 20740)
	uint32 FLAG_UNK_0x20 : 1;                       // = 0x20			(UNUSED: 20740)
	uint32 FLAG_DO_NOT_USE_LIGHTING_DIFFUSE : 1;    // = 0x40			//Do not use local diffuse lightning .Applicable for both doodads from this wmo group(color from MODD) and water(CWorldView::GatherMapObjDefGroupLiquids).
	uint32 FLAG_IS_UNREACHABLE : 1;                 // = 0x80			//UNREACHABLE

	uint32 FLAG_UNK_0x100 : 1;                      // = 0x100
	uint32 FLAG_HAS_LIGHTS : 1;                     // = 0x200		    // Has m_Lights (MOLR chunk)
	uint32 FLAG_UNK_0x400 : 1;                      // = 0x400
	uint32 FLAG_HAS_DOODADS : 1;                    // = 0x800		    // Has doodads(MODR chunk)

	uint32 FLAG_HAS_WATER : 1;                      // = 0x1000		    //LIQUIDSURFACE -- Has water(MLIQ chunk)
	uint32 FLAG_IS_INDOOR : 1;                      // = 0x2000		    //INTERIOR -- Indoor
	uint32 FLAG_UNK_0x4000 : 1;                     // = 0x4000
	uint32 FLAG_UNK_0x8000 : 1;                     // = 0x8000

	uint32 FLAG_ALWAYS_DRAW : 1;                    // = 0x10000		//ALWAYSDRAW -- clear 0x8 after CMapObjGroup::Create() in MOGP and MOGI
	uint32 FLAG_UNK_0x20000 : 1;                    // = 0x20000		Has MORI and MORB chunks.
	uint32 FLAG_HAS_SKYBOX : 1;                     // = 0x40000		//Show m_Skybox -- automatically unset if MOSB not present.
	uint32 FLAG_UNK_0x80000 : 1;                    // = 0x80000		is_not_water_but_ocean, LiquidType related, see below in the MLIQ chunk.

	uint32 FLAG_UNK_0x100000 : 1;
	uint32 FLAG_UNK_0x200000 : 1;
	uint32 FLAG_UNK_0x400000 : 1;
	uint32 FLAG_UNK_0x800000 : 1;

	uint32 : 8;
};

//-----

struct WMO_Group_HeaderDef
{
	uint32 groupName;
	uint32 descriptiveGroupName;
	WMOGroupFlags flags;
	CAaBox boundingBox;

	uint16 portalStart; // Index into the MOPR chunk
	uint16 portalCount; // Number of items used from the MOPR chunk

						// Batches
	uint16 transBatchCount;
	uint16 intBatchCount;
	uint16 extBatchCount;
	uint16 unk0;

	uint8 m_Fogs[4]; // Up to four indices into the WMO fog list
	uint32 liquidType; // LiquidType related, see below in the MLIQ chunk.

	uint32 wmoGroupId; // WMO group ID (column 4 in WMOAreaTable.dbc)

	uint32 unk1;
	uint32 unk2;
};

struct WMO_Group_MaterialDef
{
	struct Flags
	{
		uint8 F_UNK_0x01 : 1;
		uint8 F_NOCAMCOLLIDE : 1;
		uint8 F_DETAIL : 1;
		uint8 F_COLLISION : 1; // Turns off rendering of water ripple effects. May also do more. Should be used for ghost material triangles.
		uint8 F_HINT : 1;
		uint8 F_RENDER : 1;
		uint8 F_UNK_0x40 : 1;
		uint8 F_COLLIDE_HIT : 1;

		bool isTransFace() { return F_UNK_0x01 && (F_DETAIL || F_RENDER); }
		bool isColor() { return !F_COLLISION; }
		bool isRenderFace() { return F_RENDER && !F_DETAIL; }
		bool isCollidable() { return F_COLLISION || isRenderFace(); }
	} flags;

	uint8 materialId; // 0xff for collision
};

struct WMO_Group_BatchDef
{
	int16 bx, by, bz;                      // a bounding box for culling, see "unknown_box" below
	int16 tx, ty, tz;

	uint32 indexStart;
	uint16 indexCount;

	uint16 vertexStart;
	uint16 vertexEnd;

	uint8 flags;
	uint8 material_id; // index in MOMT

	uint16 getVerticesCount() const { return vertexEnd - vertexStart; }
};

struct WMO_Group_MLIQDef
{
	uint32 X, Y;
	uint32 A, B;
	vec3 pos;
	uint16 type;
};

#pragma endregion

#include __PACK_END