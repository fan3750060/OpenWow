#pragma once

// FORWARD BEGIN
class CM2_Part_Bone;
// FORWARD END

struct RibbonSegment
{
	vec3 pos, up, back;
	float len, len0;
};

class CM2_RibbonEmitters
{
public:
	CM2_RibbonEmitters(M2* _model, IFile* f, const SM2_RibbonEmitter& mta, cGlobalLoopSeq globals);

	void setup(uint16 anim, uint32 time, uint32 _globalTime, cmat4 _worldMatrix);

	void Render(cmat4 _world);

private:
	const M2*									m_ParentM2;

	uint32										m_ID;
	const CM2_Part_Bone*						m_Bone;
	vec3										pos;
	vec3										posValue;

	std::shared_ptr<Texture>							m_Texture;
	const CM2_Part_Material*					m_Material;

	//SharedBufferPtr								__vb;
	//std::shared_ptr<Mesh>								__geom;

	M2_Animated<vec3>							m_Color;
	M2_Animated<float, short, ShortToFloat>		m_Alpha;
	M2_Animated<float>							m_HeightAbove;
	M2_Animated<float>							m_HeightBelow;
	
	vec4										tcolor;
	float										tabove;
	float										tbelow;
	
	float length;
	float m_EdgesLifeTime;
	int m_EdgesPerSecond;

	list<RibbonSegment> segs;
};