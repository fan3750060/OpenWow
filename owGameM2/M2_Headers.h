#pragma once

#include "M2_Types.h"

#include __PACK_BEGIN

struct SM2_Header
{
    char							magic[4];						// MD20 Magic
    uint32							version;
    M2Array<char>					name;							// should be globally unique, used to reload by name in internal clients

    struct Flags
    {
        uint32 flag_tilt_x : 1;
        uint32 flag_tilt_y : 1;
		uint32 unk0 : 1;
		uint32 flag_use_texture_combiner_combos : 1;
        uint32 : 28;
    }								global_flags;
    M2Array<SM2_Loop>				global_loops;					// Timestamps used in global looping animations.


    // Sequences
    M2Array<SM2_Sequence>			sequences;						// Information about the animations in the model.
    M2Array<uint16>					sequencesLookup;				// Mapping of sequence IDs to the entries in the Animation sequences block.
#if VERSION <= VERSION_BC
	M2Array<uint32>					playable_animation_lookup;		// ???
#endif
	

    // Bones
    M2Array<SM2_Bone>				bones;                           // MAX_BONES = 0x100
    M2Array<uint16>					NOTbonesLookup;                     // Lookup table for key skeletal bones.

    // Vertices
    M2Array<SM2_Vertex>				vertices;

	// Skin
#if VERSION <= VERSION_BC
	M2Array<SM2_SkinProfile>		skin_profiles;					// Views (LOD) are now in .m_SkinSections.
#else
	uint32_t num_skin_profiles; 
#endif


    // Materials
    M2Array<SM2_Color>				colors;							// Color and alpha animations definitions.
    M2Array<SM2_Texture>			textures;
    M2Array<SM2_TextureWeight>		textureWeights;
#if VERSION <= VERSION_BC
	M2Array<void>					unk0;
#endif
	M2Array<SM2_TextureTransform>	textureTransforms;

    M2Array<uint16>					replacable_texture_lookup;		// ???
    M2Array<SM2_Material>			materials;						// Blending modes / render flags.
	M2Array<uint16>					bone_lookup_table;				// ???	
    M2Array<uint16>					textureLookup;
	M2Array<uint16>					textureUnitLookup;			// -1 env mapping, 0, 1 - indexes
    M2Array<uint16>					textureWeightsLookup;
    M2Array<uint16>					textureTransformsLookup;


    // Bounding box
    CAaBox							bounding_box;					// min/max( [1].z, 2.0277779f ) - 0.16f seems to be the maximum camera height
    float							bounding_sphere_radius;			// detail doodad draw dist = clamp (bounding_sphere_radius * detailDoodadDensityFade * detailDoodadDist, …)

	// Collision stuff
    CAaBox							collision_box;
    float							collision_sphere_radius;
    M2Array<uint16>					collisionTriangles;
    M2Array<vec3>					collisionVertices;
    M2Array<vec3>					collisionNormals;


    // Attachments
    M2Array<M2Attachment>			attachments;
    M2Array<uint16>					attachmentLookup;


    // Events
    M2Array<M2Event>				events;


    // Light
    M2Array<SM2_Light>				lights;


    // Camara
    M2Array<SM2_Camera>				cameras;
    M2Array<uint16>					camerasLookup;


    // Emitters and particles
    M2Array<SM2_RibbonEmitter>		ribbon_emitters;
    M2Array<M2Particle>				particle_emitters;

#if VERSION >= VERSION_WotLK
	M2Array<uint16>					textureCombinerCombos;
#endif

};


///////////////////
///////////////////
///////////////////

#define	TEXTURE_MAX	128

#include __PACK_END