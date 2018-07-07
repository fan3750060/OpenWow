#pragma once

#include "M2_Types.h"
#include "M2_Part_Bone.h"

// FORWARD BEGIN
class M2;
// FORWARD END

struct M2_AttachmentType
{
	enum List : uint32_t
	{
		ShieldORMountMainorORItemVisual0 = 0,
		HandRightORItemVisual1,
		HandLeftORItemVisual2,
		ElbowRightORItemVisual3,
		ElbowLeftORItemVisual4,

		ShoulderRight,
		ShoulderLeft,
		KneeRight,
		KneeLeft,
		HipRight,
		HipLeft,

		Helm,
		Back,

		ShoulderFlapRight,
		ShoulderFlapLeft,
		ChestBloodFront,
		ChestBloodBack,

		Breath,
		PlayerName,
		Base,
		Head,
		SpellLeftHand,
		SpellRightHand,
		Special1,
		Special2,
		Special3,
		SheathMainHand,
		SheathOffHand,
		SheathShield,
		PlayerNameMounted,
		LargeWeaponLeft,
		LargeWeaponRight,
		HipWeaponLeft,
		HipWeaponRight,
		Chest,
		HandArrow,

		Bullet,
		SpellHandOmni,
		SpellHandDirected,

		VehicleSeat1,
		VehicleSeat2,
		VehicleSeat3,
		VehicleSeat4,
		VehicleSeat5,
		VehicleSeat6,
		VehicleSeat7,
		VehicleSeat8,

		LeftFoot,
		RightFoot,

		ShieldNoGlove,
		SpineLow,

		AlteredShoulderR,
		AlteredShoulderL,

		Count = 53
	};
};


class CM2_Part_Attachment
{
public:
	CM2_Part_Attachment(const M2* _parentM2, IFile* f, const SM2_Attachment& _proto, cGlobalLoopSeq global);

private:
	M2_AttachmentType::List	m_Type;
	const CM2_Part_Bone*	m_Bone;
	vec3					m_Position;
	M2_Animated<uint8>		m_IsAnimateAttached;
};