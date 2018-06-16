#include "stdafx.h"

// General
#include "DayNightPhase.h"

DayNightPhase::DayNightPhase()
{}

DayNightPhase::DayNightPhase(IFile* f)
{
    float h, m;

    f->SeekRelative(4); // Always 0x46
    f->ReadBytes(&h, 4);
    f->SeekRelative(4);
    f->ReadBytes(&m, 4);

    f->SeekRelative(4);
    f->ReadBytes(&dayIntensity, 4);
    //
    f->SeekRelative(4);
    f->ReadBytes(&dayColor.x, 4);
    f->SeekRelative(4);
    f->ReadBytes(&dayColor.y, 4);
    f->SeekRelative(4);
    f->ReadBytes(&dayColor.z, 4);
    //
    f->SeekRelative(4);
    f->ReadBytes(&dayDir.x, 4);
    f->SeekRelative(4);
    f->ReadBytes(&dayDir.y, 4);
    f->SeekRelative(4);
    f->ReadBytes(&dayDir.z, 4);
	dayDir = dayDir.toXZmY();
    

    //

    f->SeekRelative(4);
    f->ReadBytes(&nightIntensity, 4);
    //
    f->SeekRelative(4);
    f->ReadBytes(&nightColor.x, 4);
    f->SeekRelative(4);
    f->ReadBytes(&nightColor.y, 4);
    f->SeekRelative(4);
    f->ReadBytes(&nightColor.z, 4);
    //
    f->SeekRelative(4);
    f->ReadBytes(&nightDir.x, 4);
    f->SeekRelative(4);
    f->ReadBytes(&nightDir.y, 4);
    f->SeekRelative(4);
    f->ReadBytes(&nightDir.z, 4);
	nightDir = nightDir.toXZmY();

    //

    f->SeekRelative(4);
    f->ReadBytes(&ambientIntensity, 4);
    //
    f->SeekRelative(4);
    f->ReadBytes(&ambientColor.x, 4);
    f->SeekRelative(4);
    f->ReadBytes(&ambientColor.y, 4);
    f->SeekRelative(4);
    f->ReadBytes(&ambientColor.z, 4);

    //

    f->SeekRelative(4);
    f->ReadBytes(&fogDepth, 4);
    f->SeekRelative(4);
    f->ReadBytes(&fogIntensity, 4);
    //
    f->SeekRelative(4);
    f->ReadBytes(&fogColor.x, 4);
    f->SeekRelative(4);
    f->ReadBytes(&fogColor.y, 4);
    f->SeekRelative(4);
    f->ReadBytes(&fogColor.z, 4);

    uint32 time = (uint32)h * 60 * 2 + (uint32)m * 2;
}

DayNightPhase::DayNightPhase(DayNightPhase* a, DayNightPhase* b, float r)
{
    float ir = 1.0f - r;

    // Day
    dayIntensity = a->dayIntensity * ir + b->dayIntensity * r;
    dayColor = a->dayColor * ir + b->dayColor * r;
    dayDir = a->dayDir * ir + b->dayDir * r;

    // Night
    nightIntensity = a->nightIntensity * ir + b->nightIntensity * r;
    nightColor = a->nightColor * ir + b->nightColor * r;
    nightDir = a->nightDir * ir + b->nightDir * r;

    // Ambient
    ambientIntensity = a->ambientIntensity * ir + b->ambientIntensity * r;
    ambientColor = a->ambientColor * ir + b->ambientColor * r;

    // Fog
    fogDepth = a->fogDepth * ir + b->fogDepth * r;
    fogIntensity = a->fogIntensity * ir + b->fogIntensity * r;
    fogColor = a->fogColor * ir + b->fogColor * r;
}

//

/*void DayNightPhase::setupLighting()
{
    // Setup day lighting
    if (dayIntensity > 0)
    {
        m_dirLightDay.ambient = ambientColor * ambientIntensity * dayIntensity;
        m_dirLightDay.diffuse = dayColor * dayIntensity;
        m_dirLightDay.Direction = dayDir;
    }

    // Setup night lighting
    if (nightIntensity > 0)
    {
        m_dirLightNight.ambient = ambientColor * ambientIntensity * nightIntensity;
        m_dirLightNight.diffuse = nightColor * nightIntensity;
        m_dirLightNight.Direction = nightDir;
    }
}*/

void DayNightPhase::Render_DEBUG(cvec3 _playerPos)
{
    _Render->getTechniquesMgr()->Debug_Pass->Bind();
	_Render->r.setDepthTest(false);

	_Render->r.setGeometry(_Render->getRenderStorage()->_sphereGeo);

    // Sun
	{
		mat4 worldMatrix;
		worldMatrix.translate(_playerPos + (-dayDir) * 300.0f);
		worldMatrix.scale(20);

		_Render->getTechniquesMgr()->Debug_Pass->SetWorldMatrix(worldMatrix);
		_Render->getTechniquesMgr()->Debug_Pass->SetColor4(vec4(dayColor, 1.0f));

		
		_Render->r.drawIndexed(PRIM_TRILIST, 0, 128 * 3, 0, 126, false);
	}

    // Moon
	{
		mat4 worldMatrix;
		worldMatrix.translate(_playerPos + (-nightDir) * 300.0f);
		worldMatrix.scale(20);

		_Render->getTechniquesMgr()->Debug_Pass->SetWorldMatrix(worldMatrix);
		_Render->getTechniquesMgr()->Debug_Pass->SetColor4(vec4(nightColor, 1.0f));

		_Render->r.drawIndexed(PRIM_TRILIST, 0, 128 * 3, 0, 126);
	}
	
	_Render->r.setDepthTest(true);
	_Render->getTechniquesMgr()->Debug_Pass->Unbind();
}
