#pragma once

#include "M2_Types.h"

enum ModelLightTypes
{
	MODELLIGHT_DIRECTIONAL = 0,
	MODELLIGHT_POINT = 1
};

class CM2_Part_Light
{
public:
	CM2_Part_Light(IFile* f, const SM2_Light& _proto, cGlobalLoopSeq global);

	void setup(uint16 anim, uint32 time, uint32 globalTime);

	uint16 getType() const { return type; }
	int16 getBoneIndex() const { return bone; }
	cvec3 getPosition() const { return position; }
	cvec3 getDirection() const { return direction; }

private:
	uint16			type;
	int16			bone;    
	vec3			position;
	vec3			direction;

	M2_Animated<vec3> ambColor;
	M2_Animated<float> ambIntensity;

	M2_Animated<vec3> diffColor;
	M2_Animated<float> diffIntensity;

	M2_Animated<float> attenuation_start;
	M2_Animated<float> attenuation_end;

	M2_Animated<uint8> visibility;

	// Values
	vec3			positionValue;
	vec3			directionValue;

	vec3 ambColorValue;
	float ambIntensityValue;

	vec3 diffColorValue;
	float diffIntensityValue;

	float attenuation_startValue;
	float attenuation_endValue;

	uint8 visibilityValue;
};