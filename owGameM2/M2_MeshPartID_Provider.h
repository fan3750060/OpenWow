#pragma once

#include "M2_Types.h"

class CM2_MeshPartID_Provider
{
public:
	virtual bool		isMeshEnabled		(uint32 _index)							const = 0;
	virtual R_Texture*	getSpecialTexture	(SM2_Texture::SM2_Texture_Type _type)	const = 0;
};