#pragma once

struct ADT_MHDR
{
	struct Flags
	{
		uint32 HasMFBO : 1;
		uint32 IsNortrend : 1;
	} flags;

	uint32 MCIN;
	uint32 MTEX;
	uint32 MMDX;
	uint32 MMID;
	uint32 MWMO;
	uint32 MWID;
	uint32 MDDF;
	uint32 MODF;
	uint32 MFBO;                     // this is only set if flags & mhdr_MFBO.
	uint32 MH20;
	uint32 MTXF;

	uint8 unk1[16];
};

struct ADT_TextureInfo
{
	std::string           textureName;
	std::shared_ptr<Texture> diffuseTexture;
	std::shared_ptr<Texture> specularTexture;
};

#if (VERSION == VERSION_WotLK)
struct ADT_MCIN
{
	uint32_t offset;               // absolute offset.
	uint32_t size;                 // the size of the MCNK chunk, this is refering to.
	uint32_t flags;                // always 0. only set in the client., FLAG_LOADED = 1
	uint32_t asyncId;
};
#endif

struct ADT_MCNK_Header
{
	struct Flags
	{
		uint32 has_mcsh : 1;
		uint32 impass : 1;
		uint32 lq_river : 1;
		uint32 lq_ocean : 1;
		uint32 lq_magma : 1;
		uint32 lq_slime : 1;
		uint32 has_mccv : 1;
		uint32 unk1 : 1;
		uint32 unk2 : 7;
		uint32 do_not_fix_alpha_map : 1;      // "fix" alpha maps in MCAL (4 bit alpha maps are 63*63 instead of 64*64). Note that this also means that it *has* to be 4 bit alpha maps, otherwise UnpackAlphaShadowBits will assert.
		uint32 : 16;
	} flags;

	std::shared_ptr<const DBC_LiquidTypeRecord> getLiquidType()
	{
		if (flags.lq_river)
		{
			return DBC_LiquidType[1];
		}
		else if (flags.lq_ocean)
		{
			return DBC_LiquidType[2];
		}
		else if (flags.lq_magma)
		{
			return DBC_LiquidType[3];
		}
		else if (flags.lq_slime)
		{
			return DBC_LiquidType[4];
		}

		return nullptr;
	}

	uint32 indexX;
	uint32 indexY;

	uint32 nLayers;
	uint32 nDoodadRefs;

	uint32 ofsHeight;
	uint32 ofsNormal;
	uint32 ofsLayer;
	uint32 ofsRefs;

	uint32 ofsAlpha;
	uint32 sizeAlpha;

	uint32 ofsShadow;
	uint32 sizeShadow;

	uint32 areaid;
	uint32 nMapObjRefs;

	uint16 holes;

	uint8 unk0[22];

	uint32 nEffectDoodad;

	uint32 ofsSndEmitters;
	uint32 nSndEmitters;

	uint32 ofsLiquid;
	uint32 sizeLiquid;

	float zpos;
	float xpos;
	float ypos;

	uint32_t ofsMCCV;                             // only with flags.has_mccv, had uint32_t textureId; in ObscuR's structure.

	uint8 unk1[8];
};

struct ADT_MCNK_MCLY
{
	uint32 textureIndex;
	struct Flags
	{
		uint32 animation_rotation : 3;        // each tick is 45
		uint32 animation_speed : 3;
		uint32 animation_enabled : 1;
		uint32 overbright : 1;                // This will make the texture way brighter. Used for lava to make it "glow".
		uint32 use_alpha_map : 1;             // set for every layer after the first
		uint32 alpha_map_compressed : 1;      // see MCAL chunk description
		uint32 use_cube_map_reflection : 1;   // This makes the layer behave like its a reflection of the skybox. See below
		uint32 : 21;
	} flags;
	uint32 offsetInMCAL;
	FOREIGN_KEY_ID(uint16, DBC_GroundEffectTexture, effectId);
	int16 unk0;
};