#pragma once

#include "M2_Character_Instance.h"

class Creature : public CM2_Base_Instance
{
public:
	Creature(vec3 _position);

	void InitFromDisplayInfo(uint32 _id);

private:
	void RecalculateMatrix();

private:
	vec3 m_CreaturePosition;
	float m_CreatureScale;
};