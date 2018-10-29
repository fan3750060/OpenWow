#pragma once

// FORWARD BEGIN
class CM2_ParticleSystem;
// FORWARD END

class ParticleEmitter
{
public:
	ParticleEmitter(std::weak_ptr<CM2_ParticleSystem> sys) : m_ParticleSystem(sys) {}
	virtual Particle newParticle(int anim, int time, float w, float l, float spd, float var, float spr, float spr2, uint32 _globalTime) = 0;

protected:
	std::weak_ptr<CM2_ParticleSystem> m_ParticleSystem;
};

//--

class PlaneParticleEmitter : public ParticleEmitter
{
public:
	PlaneParticleEmitter(std::weak_ptr<CM2_ParticleSystem> sys) :
		ParticleEmitter(sys) 
	{}

	Particle newParticle(int anim, int time, float w, float l, float spd, float var, float spr, float spr2, uint32 _globalTime);
};

//--

class SphereParticleEmitter : public ParticleEmitter
{
public:
	SphereParticleEmitter(std::weak_ptr<CM2_ParticleSystem> sys) :
		ParticleEmitter(sys) 
	{}

	Particle newParticle(int anim, int time, float w, float l, float spd, float var, float spr, float spr2, uint32 _globalTime);
};