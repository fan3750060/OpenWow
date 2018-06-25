#pragma once

#include "WMO_Headers.h"

#include "WMO_Group.h"
#include "WMO_Part_Fog.h"
#include "WMO_Part_Light.h"
#include "WMO_Part_Material.h"
#include "WMO_Part_Portal.h"
#include "WMO_Part_PortalsController.h"

// FORWARD BEGIN
class CWMO_Base_Instance;
class CWMO_InstanceController;
// FORWARD END

class WMO : public CRefItem
{
	friend CWMO_Part_Portal;
public:
	WMO(cstring _fileName);
	~WMO();

	void CreateInsances(CWMO_Base_Instance* _parent);
	bool Load();

	void PreRender(CWMO_InstanceController* _localContr);
	void Render(CWMO_InstanceController* _localContr, const WMO_Doodad_SetInfo& _doodadSet);

	bool drawSkybox();

#pragma region Getters
public:
	string getFilename() const { return m_FileName; }
	BoundingBox	getBounds() const { return m_Bounds; }

	bool useAmbColor() const { return !(m_Header.flags.FLAG_skip_base_color); }

	M2* getSkybox() { return m_Skybox; }
#pragma endregion

public:
	const string							m_FileName;
	WMO_HeaderDef							m_Header;				// MOHD chunk
	BoundingBox								m_Bounds;

public:
	//-- Materials --//
	char*									m_TexturesNames;		// MOTX chunk
	vector<SmartPtr<WMO_Part_Material>>		m_Materials;			// MOMT chunk

	//-- Groups --//

	vector<SmartPtr<WMO_Group>>				m_Groups;				// MOGI chunk
	vector<WMO_Group*>						m_OutdoorGroups;

	//-- Skybox --//
	SmartM2Ptr								m_Skybox;

	//-- Portals --//
	vector<vec3>							m_PortalVertices;		// MOPV chunk
	SmartBufferPtr							m_PortalVB;
	vector<CWMO_Part_Portal*>				m_Portals;
	vector<WMO_PortalReferencesDef>			m_PortalReferences;		// MOPR chunk
	CWMO_Part_PortalsController*			m_PortalController;

	//-- Visible block
	vector<vec3>							m_VisibleBlockVertices;	// MOVV chunk
	vector<WMO_VisibleBlockListDef>			m_VisibleBlockList;		// MOVB chunk


	// -- Lights --//
	vector<SmartPtr<WMO_Part_Light>>		m_Lights;				// MOLT chunk


	//-- Doodads --//
	vector<WMO_Doodad_SetInfo>				m_DoodadsSetInfos;			// MODS chunk
	char*									m_DoodadsFilenames;			// MODN chunk        
	vector<WMO_Doodad_PlacementInfo>		m_DoodadsPlacementInfos;


	//-- Fog --//
	vector<SmartPtr<WMO_Part_Fog>>			m_Fogs;					// MFOG chunk


	//-- Volumes plane --//
	// MCVP chunk (optional)	
};

struct WMODeleter
{
	void operator()(WMO* p)
	{
		GetManager<IWMOManager>()->Delete(p);
	}
};
typedef SmartPtr<WMO, WMODeleter> SmartWMOPtr;