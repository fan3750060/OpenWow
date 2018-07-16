#pragma once

// Misc
#include "M2_Part_Attachment.h"
#include "M2_Part_Event.h"
#include "M2_Part_Light.h"
#include "M2_Part_Camera.h"
#include "M2_RibbonEmitters.h"
#include "Particle.h"
#include "ParticleSystem.h"

class CM2_Comp_Miscellaneous
{
	friend class CM2_Builder;
public:
	CM2_Comp_Miscellaneous();
	virtual ~CM2_Comp_Miscellaneous();

	void calc(uint16 anim, uint32 time, uint32 globalTime, cmat4 _worldMat);
	void render(cmat4 _worldMat);

public:
	CM2_Part_Attachment* getAttachmentDirect(uint32 _index) const
	{
		assert1(_index < static_cast<int16>(m_Attachments.size()));
		return (m_Attachments[_index]);
	}

	CM2_Part_Attachment* getAttachment(M2_AttachmentType::List _index) const
	{
		if (_index >= m_AttachmentsLookup.size())
		{
			//Log::Warn("M2[%s]: getAttachment [%d] not found in Lookup[%d]", m_FileName.c_str(), _index, m_AttachmentsLookup.size());
			return nullptr;
		}
		int16 newIndex = m_AttachmentsLookup[_index];
		assert1(newIndex != -1);
		assert1(newIndex < static_cast<int16>(m_Attachments.size()));
		return (m_Attachments[newIndex]);
	}

	CM2_Part_Event* getEvents(uint32 _index) const
	{
		assert1(_index < m_Events.size());
		return (m_Events[_index]);
	}

	CM2_Part_Light* getLight(uint32 _index) const
	{
		assert1(_index < m_Lights.size());
		return (m_Lights[_index]);
	}

	CM2_Part_Camera* getCameraDirect(uint32 _index) const
	{
		assert1(_index < static_cast<int16>(m_Cameras.size()));
		return (m_Cameras[_index]);
	}

	CM2_Part_Camera* getCamera(uint32 _index) const
	{
		if (_index >= m_CamerasLookup.size())
		{
			//Log::Warn("M2[%s]: getCamera [%d] not found in Lookup[%d]", m_FileName.c_str(), _index, m_CamerasLookup.size());
			return nullptr;
		}
		int16 newIndex = m_CamerasLookup[_index];
		assert1(newIndex != -1);
		assert1(newIndex < static_cast<int16>(m_Cameras.size()));
		return (m_Cameras[newIndex]);
	}

private:
	// Attachments, events, lights and cameras
	vector<CM2_Part_Attachment*>		m_Attachments;
	vector<int16>						m_AttachmentsLookup;
	vector<CM2_Part_Event*>				m_Events;
	vector<CM2_Part_Light*>				m_Lights;
	vector<CM2_Part_Camera*>			m_Cameras;
	vector<int16>						m_CamerasLookup;

	// Particles
	vector<CM2_RibbonEmitters*>			m_RibbonEmitters;
	vector<ParticleSystem*>				particleSystems;

	bool								m_HasMisc;
};