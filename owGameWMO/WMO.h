#pragma once

#include "WMO_Headers.h"

#include "WMO_Group.h"
#include "WMO_MODD_Instance.h"
#include "WMO_Part_Material.h"
#include "WMO_Part_Light.h"
#include "WMO_Part_Fog.h"

class WMO : public CRefItem, public IUpdatable
{
public:
	WMO(cstring _fileName);
	~WMO();

	void CreateInsances(SceneNode* _parent);
	bool Load();

	// IUpdatable
	void Input(double _time, double _dTime) override {};
	void Update(double _time, double _dTime) override;

	bool Render(uint32 _doodadSet);

	bool drawSkybox();

#pragma region Getters
public: // Getters
	bool useAmbColor() const { return !(m_Header.flags.FLAG_skip_base_color); }
	vec4 getAmbColor() const
	{
		return vec4
		(
			static_cast<float>(m_Header.ambColor.r) / 255.0f,
			static_cast<float>(m_Header.ambColor.g) / 255.0f,
			static_cast<float>(m_Header.ambColor.b) / 255.0f,
			static_cast<float>(m_Header.ambColor.a) / 255.0f
		);
	}
#pragma endregion

public:
	const string							m_FileName;
	bool									m_Loaded;
	WMO_HeaderDef							m_Header;				// MOHD chunk
	BoundingBox								m_Bounds;

public:
	//-- Materials --//
	char*									m_TexturesNames;		// MOTX chunk
	vector<SmartPtr<WMO_Part_Material>>		m_Materials;			// MOMT chunk


	//-- Groups --//
	char*									m_GroupsNames;			// MOGN chunk
	vector<WMO_Group*>						m_Groups;				// MOGI chunk


	//-- Skybox --//
	char*									m_Skybox_Filename;		// MOSB chunk
	SmartM2Ptr								m_Skybox;


	//-- Portals --//
	vector<vec3>							m_PortalVertices;		// MOPV chunk
	vector<WMO_PortalDef>					m_PortalInformation;	// MOPT chunk
	vector<WMO_PortalReferencesDef>			m_PortalReferences;		// MOPR chunk


	//-- Visible block
	vector<vec3>							m_VisibleBlockVertices;	// MOVV chunk
	vector<WMO_VisibleBlockListDef>			m_VisibleBlockList;		// MOVB chunk


	// -- Lights --//
	vector<SmartPtr<WMO_Part_Light>>		m_Lights;				// MOLT chunk


	//-- Doodads --//
	vector<WMO_MODD_SetInfo>				m_M2SetInfos;				// MODS chunk
	char*									m_M2Filenames;         // MODN chunk        
	vector<WMO_MODD_PlacementInfo>			m_M2PlacementInfos;


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