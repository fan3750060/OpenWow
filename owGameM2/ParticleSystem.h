#pragma once

// Includes
#include "Particle.h"

// Typedefs
typedef list<Particle> ParticleList;

// Defines
#define MAX_PARTICLES 10000

// Classes
class ParticleSystem;

//

struct TexCoordSet
{
	vec2 tc[4];
};

#include "ParticleEmitters.h"

class ParticleSystem
{
	friend class ParticleEmitter;
	friend class PlaneParticleEmitter;
	friend class SphereParticleEmitter;
public:
	ParticleSystem();
	~ParticleSystem();

	void init(IFile* f, const SM2_Particle& mta, cGlobalLoopSeq globals);

	void update(float dt, uint32 _globalTime);
	void setup(uint16 anim, uint32 time, uint32 _globalTime);

	void draw();

public:
	M2* m_ParentM2;
	float tofs;

private:
	void initTile(vec2 *tc, int num);

private:
	M2_Animated<float> speed, variation, spread, lat, gravity, lifespan, rate, areal, areaw, deacceleration;
	M2_Animated<uint8> enabled;
	vec4 colors[3];
	float sizes[3];
	float mid, slowdown, rotation;
	vec3 pos;
	SharedTexturePtr texture;
	ParticleEmitter* emitter;
	ParticleList particles;
	int blend, order, type;
	int manim, mtime;
	int rows, cols;
	vector<TexCoordSet> tiles;

	bool billboard;

	float rem;
	//bool transform;

	// unknown parameters omitted for now ...
	int32 flags;
	int16 pType;

	const CM2_Part_Bone* parent;

};

template<class T>
inline T lifeRamp(float life, float mid, const T &a, const T &b, const T &c)
{
	if (life <= mid)
		return interpolate<T>(life / mid, a, b);
	else
		return interpolate<T>((life - mid) / (1.0f - mid), b, c);
}