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
        uint32 : 30;
    }								global_flags;
    M2Array<SM2_Loop>				global_loops;					// Timestamps used in global looping animations.


    // Sequences
    M2Array<SM2_Sequence>			sequences;						// Information about the animations in the model.
    M2Array<uint16>					sequencesLookup;				// Mapping of sequence IDs to the entries in the Animation sequences block.
	M2Array<uint32>					playable_animation_lookup;		// ???

    // Bones
    M2Array<SM2_Bone>				bones;                           // MAX_BONES = 0x100
    M2Array<uint16>					NOTbonesLookup;                     // Lookup table for key skeletal bones.

    // Vertices
    M2Array<SM2_Vertex>				vertices;

    // Skin
	M2Array<SM2_SkinProfile>		skin_profiles;					// Views (LOD) are now in .skins.

    // Materials
    M2Array<SM2_Color>				colors;							// Color and alpha animations definitions.
    M2Array<SM2_Texture>			textures;
    M2Array<SM2_TextureWeight>		textureWeights;  
	M2Array<void>					unk0;
	M2Array<SM2_TextureTransform>	textureTransforms;
    M2Array<uint16>					replacable_texture_lookup;		// ???
    M2Array<SM2_Material>			materials;						// Blending modes / render flags.
	M2Array<uint16>					bone_lookup_table;				// ???	
    M2Array<uint16>					texturesLookup;
	M2Array<uint16>					tex_unit_lookup_table;			// ???
    M2Array<uint16>					textureWeightsLookup;
    M2Array<uint16>					textureTransformsLookup;


    // Bounding box
    CAaBox							bounding_box;					// min/max( [1].z, 2.0277779f ) - 0.16f seems to be the maximum camera height
    float							bounding_sphere_radius;			// detail doodad draw dist = clamp (bounding_sphere_radius * detailDoodadDensityFade * detailDoodadDist, …)

    CAaBox							collision_box;
    float							collision_sphere_radius;


    // Collision stuff
    M2Array<uint16>					m_CollisionTriangles;
    M2Array<vec3>					m_CollisionVertices;
    M2Array<vec3>					m_CollisionNormals;


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
};


///////////////////
///////////////////
///////////////////

#define	TEXTURE_MAX	128

#include __PACK_END