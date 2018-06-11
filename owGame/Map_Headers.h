#pragma once

struct ADT_TextureInfo : public CRefItem
{
	ADT_TextureInfo() :
		diffuseTexture(nullptr),
		specularTexture(nullptr)
	{}

	//

	string textureName;
	SmartTexturePtr diffuseTexture;
	SmartTexturePtr specularTexture;
};

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
		uint32 unk0 : 1;
		uint32 unk1 : 1;
		uint32 unk2 : 7;
		uint32 do_not_fix_alpha_map : 1;      // "fix" alpha maps in MCAL (4 bit alpha maps are 63*63 instead of 64*64). Note that this also means that it *has* to be 4 bit alpha maps, otherwise UnpackAlphaShadowBits will assert.
		uint32 : 15;
	} flags;

	DBC_LIQUIDTYPE_Type getLiquidType()
	{
		if (flags.lq_river)
		{
			return DBC_LIQUIDTYPE_Type::lq_river;
		}
		else if (flags.lq_ocean)
		{
			return  DBC_LIQUIDTYPE_Type::lq_ocean;
		}
		else if (flags.lq_magma)
		{
			return DBC_LIQUIDTYPE_Type::lq_magma;
		}
		else if (flags.lq_slime)
		{
			return DBC_LIQUIDTYPE_Type::lq_slime;
		}
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

	uint8 unk1[12];
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
	__DBC_FOREIGN_KEY_ID(uint16, DBC_GroundEffectTexture, effectId);
	int16 unk0;
};