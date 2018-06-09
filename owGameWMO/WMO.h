#pragma once

#include "WMO_MODD.h"

class WMOGroup;
class ADT_WMO_Instance;
class WMOManager;

class WMOFog;
class WMOLight;
class WMOMaterial;

#include "WMO_Headers.h"

class WMO : public CRefItem, public IUpdatable
{
public:
	WMO(cstring _fileName);
	~WMO();

	void CreateInsances(SceneNode* _parent);

public:
	bool Load();
	inline bool IsLoaded() { return m_Loaded; }

	// IUpdatable
	void Input(double _time, double _dTime) override {};
	void Update(double _time, double _dTime) override;

	bool Render(uint32 _doodadSet);

	bool drawSkybox();

//#ifdef _DEBUG
	void DEBUG_DrawLightPlaceHolders();
	void DEBUG_DrawFogPositions();
	void DEBUG_DrawMainBoundingBox();
    void DEBUG_DrawMainBoundingSphere();
	void DEBUG_DrawBoundingBoxes();
	void DEBUG_DrawPortalsRelations();
	void DEBUG_DrawPortals();
//#endif

public:
	string m_FileName;
	bool m_Loaded;
	WMO_HeaderDef m_Header;                                   // MOHD chunk

public:
	//-- Materials --//
	char* m_TexturesNames;                                          // MOTX chunk
	vector<SmartPtr<WMOMaterial>> m_Materials;                              // MOMT chunk


	//-- Groups --//
	char* m_GroupsNames;									   // MOGN chunk
	vector<WMOGroup*> m_Groups;                              // MOGI chunk


	//-- Skybox --//
	char* m_Skybox_Filename;                                 // MOSB chunk
	SmartM2Ptr m_Skybox;


	//-- Portals --//
	vec3* m_PortalVertices;                                 // MOPV chunk
	vector<WMO_PortalDef> m_PortalInformation;     // MOPT chunk
	vector<WMO_PortalReferencesDef> m_PortalReferences;       // MOPR chunk


	//-- Visible block
	vec3* m_VisibleBlockVertices;                           // MOVV chunk
	vector<WMO_VisibleBlockListDef> m_VisibleBlockList;		// MOVB chunk


	// -- Lights --//
	vector<WMOLight> m_Lights;                               // MOLT chunk


	//-- Doodads --//
	vector<WMO_DoodadSetDef*> doodadsets;                      // MODS chunk
	char* m_MDXFilenames;                                   // MODN chunk
	vector<string> m_MDXNames;                             
	vector<WMO_MODD_PlacementInfo> m_MDX_Placement;
	vector<SmartPtr<WMO_MODD>> m_MDXInstances;						// MODD chunk


	//-- Fog --//
	vector<WMOFog*> m_Fogs;                                   // MFOG chunk


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