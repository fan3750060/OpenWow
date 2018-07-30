#include "stdafx.h"

// Include
#include "ParticleSystem.h"

// General
#include "ParticleEmitters.h"

//Generates the rotation matrix based on spread
mat4 SpreadMat;

void CalcSpreadMatrix(float Spread1, float Spread2, float w, float l)
{
	int i, j;
	float a[2], c[2], s[2];
	mat4	Temp;

	SpreadMat = mat4();

	a[0] = Random::GenerateRange(-Spread1, Spread1) / 2.0f;
	a[1] = Random::GenerateRange(-Spread2, Spread2) / 2.0f;

	/*SpreadMat.m[0][0]*=l;
	SpreadMat.m[1][1]*=l;
	SpreadMat.m[2][2]*=w;*/

	for (i = 0; i < 2; i++)
	{
		c[i] = cos(a[i]);
		s[i] = sin(a[i]);
	}

	Temp = mat4();
	Temp[1][1] = c[0];
	Temp[2][1] = s[0];
	Temp[2][2] = c[0];
	Temp[1][2] = -s[0];

	SpreadMat = SpreadMat*Temp;

	Temp = mat4();
	Temp[0][0] = c[1];
	Temp[1][0] = s[1];
	Temp[1][1] = c[1];
	Temp[0][1] = -s[1];

	SpreadMat = SpreadMat*Temp;

	float Size = abs(c[0])*l + abs(s[0])*w;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			SpreadMat[i][j] *= Size;
}

Particle PlaneParticleEmitter::newParticle(int anim, int time, float w, float l, float spd, float var, float spr, float spr2, uint32 _globalTime)
{
	// Model Flags - *shrug* gotta write this down somewhere.
	// 0x1 =
	// 0x2 =
	// 0x4 =
	// 0x8 = 
	// 0x10 = 
	// 19 = 0x13 = blue ball in thunderfury = should be billboarded?

	// Particle Flags
	// 0x0	/ 0		= Basilisk has no flags?
	// 0x1	/ 1		= Pretty much everything I know of except Basilisks have this flag..  Billboard?
	// 0x2	/ 2		=
	// 0x4	/ 4		=
	// 0x8  / 8		= 
	// 0x10	/ 16	= Position Relative to bone pivot?
	// 0x20	/ 32	=
	// 0x40	/ 64	=
	// 0x80 / 128	=
	// 0x100 / 256	=
	// 0x200 / 512	=
	// 0x400 / 1024 =
	// 0x800 / 2048 =
	// 0x1000/ 4096 =
	// 0x0000/ 1593 = [1,8,16,32,512,1024]"Warp Storm" - aura type particle effect
	// 0x419 / 1049 = [1,8,16,1024] Forest Wind shoulders
	// 0x411 / 1041 = [1,16,1024] Halo
	// 0x000 / 541	= [1,4,8,16,512] Staff glow
	// 0x000 / 537 = "Warp Storm"
	// 0x31 / 49 = [1,16,32] particle moving up?
	// 0x00 / 41 = [1,8,32] Blood elf broom, dust spread out on the ground (X, Z axis)
	// 0x1D / 29 = [1,4,8,16] particle being static
	// 0x19 / 25 = [1,8,16] flame on weapon - move up/along the weapon
	// 17 = 0x11 = [1,16] glow on weapon - static, random direction.  - Aurastone Hammer
	// 1 = 0x1 = perdition blade
	// 4121 = water ele
	// 4097 = water elemental
	// 1041 = Transcendance Halo
	// 1039 = water ele

	Particle p;

	//Spread Calculation
	mat4 mrot;

	CalcSpreadMatrix(spr, spr, 1.0f, 1.0f);
	mrot = m_ParticleSystem->m_ParentBone->getRotateMatrix() * SpreadMat;

	if (m_ParticleSystem->flags == 1041)
	{ // Trans Halo
		p.pos = m_ParticleSystem->m_ParentBone->getTransformMatrix() * vec4((m_ParticleSystem->m_Position + vec3(Random::GenerateRange(-l, l), 0, Random::GenerateRange(-w, w))), 0);

		const float t = Random::GenerateRange(0.0f, glm::two_pi<float>());

		p.pos = vec3(0.0f, m_ParticleSystem->m_Position.y + 0.15f, m_ParticleSystem->m_Position.z) + vec3(cos(t) / 8, 0.0f, sin(t) / 8); // Need to manually correct for the halo - why?

																								 // var isn't being used, which is set to 1.0f,  whats the importance of this?
																								 // why does this set of values differ from other particles

		vec3 dir(0.0f, 1.0f, 0.0f);
		p.dir = dir;

		p.speed = glm::normalize(dir) * spd * Random::GenerateRange(0.0f, var);
	}
	else if (m_ParticleSystem->flags == 25 && m_ParticleSystem->m_ParentBone->getParentBoneID() < 1)
	{ // Weapon Flame
		p.pos = m_ParticleSystem->m_ParentBone->getPivot() * (m_ParticleSystem->m_Position + vec3(Random::GenerateRange(-l, l), Random::GenerateRange(-l, l), Random::GenerateRange(-w, w)));
		vec3 dir = mrot * vec4(0.0f, 1.0f, 0.0f, 0.0f);
		p.dir = glm::normalize(dir);
		//vec3 dir = sys->model->bones[sys->parent->parent].mrot * sys->parent->mrot * vec3(0.0f, 1.0f, 0.0f);
		//p.speed = dir.Normalize() * spd;

	}
	else if (m_ParticleSystem->flags == 25 && m_ParticleSystem->m_ParentBone->getParentBoneID() > 0)
	{ // Weapon with built-in Flame (Avenger lightsaber!)
		p.pos = m_ParticleSystem->m_ParentBone->getTransformMatrix() * vec4((m_ParticleSystem->m_Position + vec3(Random::GenerateRange(-l, l), Random::GenerateRange(-l, l), Random::GenerateRange(-w, w))), 0);
		vec3 dir = vec3
		(
			m_ParticleSystem->m_ParentBone->getTransformMatrix()[1][0], 
			m_ParticleSystem->m_ParentBone->getTransformMatrix()[1][1], 
			m_ParticleSystem->m_ParentBone->getTransformMatrix()[1][2]
		) * vec3(0.0f, 1.0f, 0.0f);
		p.speed = glm::normalize(dir) * spd * Random::GenerateRange(0.0f, var * 2);

	}
	else if (m_ParticleSystem->flags == 17 && m_ParticleSystem->m_ParentBone->getParentBoneID() < 1)
	{ // Weapon Glow
		p.pos = m_ParticleSystem->m_ParentBone->getPivot() * (m_ParticleSystem->m_Position + vec3(Random::GenerateRange(-l, l), Random::GenerateRange(-l, l), Random::GenerateRange(-w, w)));
		vec3 dir = mrot * vec4(0, 1, 0, 0);
		p.dir = glm::normalize(dir);
	}
	else
	{
		p.pos = m_ParticleSystem->m_Position + vec3(Random::GenerateRange(-l, l), 0, Random::GenerateRange(-w, w));
		p.pos = m_ParticleSystem->m_ParentBone->getTransformMatrix() * vec4(p.pos, 0);

		//vec3 dir = mrot * vec3(0,1,0);
		vec3 dir = m_ParticleSystem->m_ParentBone->getRotateMatrix() * vec4(0, 1, 0, 0);

		p.dir = dir;//.Normalize();
		p.down = vec3(0, -1.0f, 0); // dir * -1.0f;
		p.speed = glm::normalize(dir) * spd * (1.0f + Random::GenerateRange(-var, var));
	}

	if (!m_ParticleSystem->billboard)
	{
		p.corners[0] = mrot * vec4(-1, 0, +1, 0);
		p.corners[1] = mrot * vec4(+1, 0, +1, 0);
		p.corners[2] = mrot * vec4(+1, 0, -1, 0);
		p.corners[3] = mrot * vec4(-1, 0, -1, 0);
	}

	p.life = 0;
	p.maxlife = m_ParticleSystem->lifespan.getValue(anim, time, _globalTime);

	p.origin = p.pos;

	p.m_TileExists = Random::GenerateRange(0, m_ParticleSystem->rows*m_ParticleSystem->cols - 1);
	return p;
}

Particle SphereParticleEmitter::newParticle(int anim, int time, float w, float l, float spd, float var, float spr, float spr2, uint32 _globalTime)
{
	Particle p;
	vec3 dir;
	float radius;

	radius = Random::GenerateRange(0.0f, 1.0f);

	// Old method
	//float t = Random::GenerateRange(0,2*Math::Pi);

	// New
	// Spread should never be zero for sphere particles ?
	float t = 0;
	if (spr == 0)
		t = Random::GenerateRange(-glm::pi<float>(), glm::pi<float>());
	else
		t = Random::GenerateRange(-spr, spr);

	//Spread Calculation
	mat4 mrot;

	CalcSpreadMatrix(spr * 2, spr2 * 2, w, l);
	mrot = m_ParticleSystem->m_ParentBone->getRotateMatrix()*SpreadMat;

	// New
	// Length should never technically be zero ?
	//if (l==0)
	//	l = w;

	// New method
	// vec3 bdir(w*cosf(t), 0.0f, l*sinf(t));
	// --

	// TODO: fix shpere emitters to work properly
	/* // Old Method
	//vec3 bdir(l*cosf(t), 0, w*sinf(t));
	//vec3 bdir(0, w*cosf(t), l*sinf(t));


	float theta_range = sys->spread.getValue(anim, time);
	float theta = -0.5f* theta_range + Random::GenerateRange(0, theta_range);
	vec3 bdir(0, l*cosf(theta), w*sinf(theta));

	float phi_range = sys->lat.getValue(anim, time);
	float phi = Random::GenerateRange(0, phi_range);
	rotate(0,0, &bdir.z, &bdir.x, phi);
	*/

	if (m_ParticleSystem->flags == 57 || m_ParticleSystem->flags == 313)
	{ // Faith Halo
		vec3 bdir(w*cosf(t)*1.6, 0.0f, l*sinf(t)*1.6);

		p.pos = m_ParticleSystem->m_Position + bdir;
		p.pos = m_ParticleSystem->m_ParentBone->getTransformMatrix() * vec4(p.pos, 0);

		if (glm::length2(bdir) == 0)
			p.speed = vec3(0, 0, 0);
		else
		{
			dir = m_ParticleSystem->m_ParentBone->getRotateMatrix() * vec4((glm::normalize(bdir)), 0);//mrot * vec3(0, 1.0f,0);
			p.speed = glm::normalize(dir) * spd * (1.0f + Random::GenerateRange(-var, var));   // ?
		}

	}
	else
	{
		vec3 bdir;
		float temp;

		bdir = mrot * vec4(vec3(0, 1, 0) * radius, 0);
		temp = bdir.z;
		bdir.z = bdir.y;
		bdir.y = temp;

		p.pos = m_ParticleSystem->m_ParentBone->getTransformMatrix() * vec4(m_ParticleSystem->m_Position + bdir, 0);


		//p.pos = sys->pos + bdir;
		//p.pos = sys->parent->mat * p.pos;


		if ((glm::length2(bdir) == 0) && ((m_ParticleSystem->flags & 0x100) != 0x100))
		{
			p.speed = vec3(0, 0, 0);
			dir = m_ParticleSystem->m_ParentBone->getRotateMatrix() * vec4(0, 1, 0, 0);
		}
		else
		{
			if (m_ParticleSystem->flags & 0x100)
				dir = m_ParticleSystem->m_ParentBone->getRotateMatrix() * vec4(0, 1, 0, 0);
			else
				dir = glm::normalize(bdir);

			p.speed = glm::normalize(dir) * spd * (1.0f + Random::GenerateRange(-var, var));   // ?
		}
	}

	p.dir = glm::normalize(dir);//mrot * vec3(0, 1.0f,0);
	p.down = vec3(0, -1.0f, 0);

	p.life = 0;
	p.maxlife = m_ParticleSystem->lifespan.getValue(anim, time, _globalTime);

	p.origin = p.pos;

	p.m_TileExists = Random::GenerateRange(0, m_ParticleSystem->rows*m_ParticleSystem->cols - 1);
	return p;
}
