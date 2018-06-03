#pragma once

// FORWARD BEGIN
class ParticleSystem;
// FORWARD END


class ParticleEmitter
{
public:
	ParticleEmitter(ParticleSystem* sys) : sys(sys) {}
	virtual Particle newParticle(int anim, int time, float w, float l, float spd, float var, float spr, float spr2) = 0;

protected:
	ParticleSystem* sys;
};

//--

class PlaneParticleEmitter : public ParticleEmitter
{
public:
	PlaneParticleEmitter(ParticleSystem* sys) : ParticleEmitter(sys) {}
	Particle newParticle(int anim, int time, float w, float l, float spd, float var, float spr, float spr2);
};

//--

class SphereParticleEmitter : public ParticleEmitter
{
public:
	SphereParticleEmitter(ParticleSystem *sys) : ParticleEmitter(sys) {}
	Particle newParticle(int anim, int time, float w, float l, float spd, float var, float spr, float spr2);
};