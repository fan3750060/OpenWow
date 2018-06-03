#pragma once

#include "M2_CommonTypes.h"

#include __PACK_BEGIN

struct SM2_Loop
{
	uint32 timestamp;
};

struct SM2_Sequence
{
	uint16		id;						// Animation id in AnimationData.dbc
	uint16		variationIndex;			// Sub-animation id: Which number in a row of animations this one is.
	uint32_t	start_timestamp;
	uint32_t	end_timestamp;

	float		movespeed;				// This is the speed the character moves with in this animation.

	uint32		flags;					// See below.
	int16		frequency;				// This is used to determine how often the animation is played. For all animations of the same type, this adds up to 0x7FFF (32767).
	uint16		unk0;
	M2Range		replay;					// May both be 0 to not repeat. Client will pick a random number of repetitions within bounds if given.
	uint32		blendTime;

	M2Bounds	bounds;
	int16		variationNext;			// id of the following animation of this AnimationID, points to an Index or is -1 if none.
	uint16		aliasNext;				// id in the list of animations. Used to find actual animation if this sequence is an alias (flags & 0x40)
};

struct SM2_Bone
{
	int32			key_bone_id;			// Back-reference to the key bone lookup table. -1 if this is no key bone.

	struct Flags
	{
		uint32 unk0 : 1;
		uint32 unk1 : 1;
		uint32 unk2 : 1;
		uint32 spherical_billboard : 1;
		uint32 cylindrical_billboard_lock_x : 1;
		uint32 cylindrical_billboard_lock_y : 1;
		uint32 cylindrical_billboard_lock_z : 1;
		uint32 unk3 : 1;
		uint32 unk4 : 1;
		uint32 transformed : 1;
		uint32 kinematic_bone : 1;			// MoP+: allow physics to influence this bone
		uint32 unk5 : 1;
		uint32 helmet_anim_scaled : 1;		// set blend_modificator to helmetAnimScalingRec.m_amount for this bone
		uint32 : 19;
	}				flags;

	int16			parent_bone;            // Parent bone ID or -1 if there is none.
	uint16			submesh_id;				// Mesh part ID OR uDistToParent?

	M2Track<vec3>	translation;
	M2Track<quat>	rotation;				// compressed values, default is (32767,32767,32767,65535) == (0,0,0,1) == identity
	M2Track<vec3>	scale;

	vec3			pivot;					// The pivot point of that bone.
};

struct SM2_Vertex
{
	vec3	pos;
	uint8	bone_weights[4];
	uint8	bone_indices[4];
	vec3	normal;
	vec2	tex_coords[2];  // two DiffuseTextures, depending on shader used
};

#include __PACK_END

#include "M2_SkinTypes.h"

#include __PACK_BEGIN

struct SM2_Color
{
	M2Track<vec3> color;     // vertex colors in rgb order
	M2Track<short> alpha;    // 0 - transparent, 0x7FFF - opaque
};

struct SM2_Texture
{
	uint32 type;
	struct Flags
	{
		uint32 WRAPX : 1;
		uint32 WRAPY : 1;
		uint32 : 30;
	} flags;
	M2Array<char> filename; // for non-hardcoded textures (type != 0), this still points to a zero-sized string
};

struct SM2_TextureWeight
{
	M2Track<short> weight; // fixed 16
};

struct SM2_TextureTransform
{
	M2Track<vec3> translation;
	M2Track<Quaternion> rotation;    // rotation center is texture center (0.5, 0.5)
	M2Track<vec3> scaling;
};

struct SM2_Material
{
	struct Flags
	{
		uint16 UNLIT : 1;
		uint16 UNFOGGED : 1;
		uint16 TWOSIDED : 1;
		uint16 DEPTHTEST : 1;
		uint16 DEPTHWRITE : 1;
		uint16 : 11;
	} flags;
	uint16 blending_mode;
};

struct M2Attachment
{
	uint32 id;                        // Referenced in the lookup-block below.
	uint16 bone;                      // attachment base
	uint16 unk0;
	vec3 position;                      // relative to bone; Often this value is the same as bone's pivot point 
	M2Track<uint8> animate_attached;  // whether or not the attached model is animated when this model is. only a bool is used. default is true.
};

struct M2Event
{
	uint32 identifier;  // mostly a 3 character name prefixed with '$'.
	uint32 data;        // This data is passed when the event is fired. 
	uint32 bone;        // Somewhere it has to be attached.
	vec3 position;    // Relative to that bone of course, animated. Pivot without animating.
	M2TrackBase enabled;  // This is a timestamp-only animation block. It is built up the same as a normal AnimationBlocks, but is missing values, as every timestamp is an implicit "fire now".
};

struct SM2_Light
{
	uint16 type;                      // Types are listed below.
	int16 bone;                       // -1 if not attached to a bone
	vec3 position;                 // relative to bone, if given

	M2Track<vec3> ambient_color;
	M2Track<float> ambient_intensity;   // defaults to 1.0

	M2Track<vec3> diffuse_color;
	M2Track<float> diffuse_intensity;   // defaults to 1.0

	M2Track<float> attenuation_start;
	M2Track<float> attenuation_end;

	M2Track<uint8> visibility;          // enabled?
};

struct SM2_Camera // TODO Spline keys
{
	uint32 type; // 0: portrait, 1: characterinfo; -1: else (flyby etc.); referenced backwards in the lookup table.

	float fov;

	float far_clip;
	float near_clip;

	M2Track<vec3> positions; // How the camera's position moves. Should be 3*3 floats.
	vec3 position_base;

	M2Track<vec3> target_position; // How the target moves. Should be 3*3 floats.
	vec3 target_position_base;

	M2Track<float> roll; // The camera can have some roll-effect. Its 0 to 2*Pi. 
};

#include __PACK_END

#include "M2_ParticlesTypes.h"