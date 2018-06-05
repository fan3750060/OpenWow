#include "stdafx.h"

// Include
#include "M2.h"

// General
#include "M2_RibbonEmitters.h"

CM2_RibbonEmitters::CM2_RibbonEmitters(M2* _model, IFile* f, const SM2_RibbonEmitter& _proto, cGlobalLoopSeq globals) :
	m_MDX(_model)
{
	m_Bone = &(m_MDX->m_Bones[_proto.boneIndex]);
	posValue = pos = _proto.position.toXZmY();

	m_Color.init(_proto.colorTrack, f, globals);
	m_Alpha.init(_proto.alphaTrack, f, globals);
	m_HeightAbove.init(_proto.heightAboveTrack, f, globals);
	m_HeightBelow.init(_proto.heightBelowTrack, f, globals);

	{
		uint16_t* TexturesList = (uint16_t*)(f->GetData() + _proto.textureIndices.offset);
		// just use the first texture for now; most models I've checked only had one
		assert1(_proto.textureIndices.size > 0);
		m_Texture = m_MDX->m_Textures[TexturesList[0]].getTexture();

		uint16_t* MaterialsList = (uint16_t*)(f->GetData() + _proto.materialIndices.offset);
		assert1(_proto.materialIndices.size > 0);
		m_Material = &(m_MDX->m_Materials[MaterialsList[0]]);
	}

	// TODO: figure out actual correct way to calculate length
	// in BFD, res is 60 and len is 0.6, the trails are very short (too long here)
	// in CoT, res and len are like 10 but the trails are supposed to be much longer (too short here)
	m_EdgesPerSecond = (int)_proto.edgesPerSecond;
	m_EdgesLifeTime = _proto.edgeLifetime;
	length = m_EdgesPerSecond * m_EdgesLifeTime;

	// create first segment
	RibbonSegment rs;
	rs.pos = posValue;
	rs.len = 0;
	segs.push_back(rs);
}

void CM2_RibbonEmitters::setup(uint32 anim, uint32 time, uint32 _globalTime)
{
	vec3 ntpos = m_Bone->getTransformMatrix() * pos;
	vec3 ntup = m_Bone->getTransformMatrix() * (pos + vec3(0, 0, 1));
	ntup -= ntpos;
	ntup = ntup.normalized();
	float dlen = (ntpos - posValue).length();

	// move first segment
	RibbonSegment& first = *segs.begin();
	if (first.len > m_EdgesLifeTime)
	{
		// add new segment
		first.back = (posValue - ntpos).normalized();
		first.len0 = first.len;

		RibbonSegment newseg;
		newseg.pos = ntpos;
		newseg.up = ntup;
		newseg.len = dlen;
		segs.push_front(newseg);
	}
	else
	{
		first.up = ntup;
		first.pos = ntpos;
		first.len += dlen;
	}

	// kill stuff from the end
	float l = 0;
	bool erasemode = false;
	for (auto it = segs.begin(); it != segs.end(); )
	{
		if (!erasemode)
		{
			l += it->len;
			if (l > length)
			{
				it->len = l - length;
				erasemode = true;
			}
			++it;
		}
		else
		{
			segs.erase(it++);
		}
	}

	posValue = ntpos;
	if (m_Color.uses() && m_Alpha.uses())
	{
		tcolor = vec4(m_Color.getValue(anim, time, _globalTime), m_Alpha.getValue(anim, time, _globalTime));
	}

	if (m_HeightAbove.uses())
	{
		tabove = m_HeightAbove.getValue(anim, time, _globalTime);
	}

	if (m_HeightBelow.uses())
	{
		tbelow = m_HeightBelow.getValue(anim, time, _globalTime);
	}
}

struct RibbonVertex
{
	RibbonVertex(vec3 _pos, vec2 _tex) :
		pos(_pos),
		tex(_tex)
	{}

	vec3 pos;
	vec2 tex;
};

void CM2_RibbonEmitters::draw()
{
	vector<RibbonVertex> vertices;

	list<RibbonSegment>::iterator it = segs.begin();
	float l = 0;
	for (; it != segs.end(); ++it)
	{
		float u = l / length;

		vertices.push_back(RibbonVertex(it->pos + it->up * tabove, vec2(u, 0)));
		vertices.push_back(RibbonVertex(it->pos - it->up * tbelow, vec2(u, 0)));

		l += it->len;
	}

	if (segs.size() > 1)
	{
		// last segment...?
		--it;
		vertices.push_back(RibbonVertex(it->pos + it->up * tabove + it->back*(it->len / it->len0), vec2(1, 0)));
		vertices.push_back(RibbonVertex(it->pos - it->up * tbelow + it->back*(it->len / it->len0), vec2(1, 0)));
	}



	/*texture->Bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	
	glColor4fv(tcolor);

	glBegin(GL_QUAD_STRIP);
	{
		list<RibbonSegment>::iterator it = segs.begin();
		float l = 0;
		for (; it != segs.end(); ++it)
		{
			float u = l / length;

			glTexCoord2f(u, 0);
			glVertex3fv(it->pos + it->up * tabove);

			glTexCoord2f(u, 1);
			glVertex3fv(it->pos - it->up * tbelow);

			l += it->len;
		}

		if (segs.size() > 1)
		{
			// last segment...?
			--it;
			glTexCoord2f(1, 0);
			glVertex3fv(it->pos + it->up * tabove + it->back*(it->len / it->len0));

			glTexCoord2f(1, 1);
			glVertex3fv(it->pos - it->up * tbelow + it->back*(it->len / it->len0));
		}
	}
	glEnd();

	glColor4f(1, 1, 1, 1);
	glEnable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);*/


	// Vertex buffer
	R_Buffer* __vb = _Render->r.createVertexBuffer(vertices.size() * sizeof(RibbonVertex), vertices.data());

	// Geometry
	R_GeometryInfo* __geom = _Render->r.beginCreatingGeometry(_Render->Storage()->__layout_GxVBF_PT);
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 0, sizeof(RibbonVertex));
	__geom->setGeomVertexParams(__vb, R_DataType::T_FLOAT, 12, sizeof(RibbonVertex));
	__geom->finishCreatingGeometry();


	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_ONE);
	_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
	_Render->r.setDepthMask(false);

	_Render->TechniquesMgr()->m_Ribbons->Bind();
	_Render->TechniquesMgr()->m_Ribbons->SetPVW();

	_Render->r.setTexture(10, m_Texture, 0, 0);

	_Render->r.setGeometry(__geom);
	_Render->r.draw(PRIM_LINES, 0, vertices.size());

	_Render->TechniquesMgr()->m_Ribbons->Unbind();

	_Render->r.setDepthMask(true);
	_Render->r.setCullMode(R_CullMode::RS_CULL_BACK);
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	__geom->destroyGeometry(true);
}