#pragma once

class CM2_Part_Bone;
class MDX;

struct RibbonSegment
{
	vec3 pos, up, back;
	float len, len0;
};

class RibbonEmitter
{
public:
	void init(IFile* f, M2Ribbon &mta, const vector<SM2_Loop>* globals);
	void setup(int anim, int time);
	void draw();

public:
	MDX* model;

private:
	M2_Animated<vec3> color;
	M2_Animated<float, short, ShortToFloat> opacity;
	M2_Animated<float> above;
	M2_Animated<float> below;

	CM2_Part_Bone* parent;
	float f1, f2;

	vec3 pos;

	int manim, mtime;
	float length, seglen;
	int numsegs;

	vec3 tpos;
	vec4 tcolor;
	float tabove, tbelow;

	SmartTexturePtr texture;

	list<RibbonSegment> segs;
};