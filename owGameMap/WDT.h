#pragma once

#include "ADT_WMO_Instance.h"

// FORWARD BEGIN
class MapController;
// FORWARD END

struct WDT_MPHD_Flags
{
	uint32 Flag_GlobalWMO : 1;  // Use global map object definition.
	uint32 Flag_UseMCCV : 1;    // Use vertex shading (ADT.MCNK.MCCV)
	uint32 Flag_8bitMCAL : 1;   // Decides whether to use _env terrain shaders or not: funky and if MCAL has 4096 instead of 2048(?)
	uint32 Flag_Unk0 : 1;       // Disables something. No idea what. Another rendering thing. Someone may check all them in wild life..
	uint32 Flag_HasMCLV : 1;    // vertexBufferFormat = PNC2. (adds second color: ADT.MCNK.MCLV)
	uint32 Flag_FlipGround : 1; // Flips the ground display upside down to create a ceiling (Cataclysm)
	uint32 : 26;
};

struct WDT_MAIN
{
	struct Flags
	{
		uint32 Flag_HasADT : 1;
		uint32 Flag_IsLoaded : 1;
		uint32 : 30;
	} flags;
	uint32_t asyncId;
};

// FORWARD BEGIN
class MapController;
// FORWARD END

class WDT
{
	friend MapController;
public:
	WDT(std::weak_ptr<MapController> _mapController);
	virtual ~WDT();

	void CreateInsances(std::weak_ptr<SceneNode> _parent);
	void Load();

	const WDT_MPHD_Flags* getFlags() const { return &m_Flag; }
	bool MapHasTiles() { return m_IsTileBased; }
	bool MapHasGlobalWMO() { return !m_GlobalWMOName.empty(); }
#ifdef GAME_MAP_INCLUDE_WMO_AND_M2
	const std::shared_ptr<ADT_WMO_Instance> GetGlobalWMOInstance() const { return m_GlobalWMO; }
#endif


private:
	WDT_MPHD_Flags						m_Flag;
	bool								m_IsTileBased;
	WDT_MAIN							m_TileFlag[C_TilesInMap][C_TilesInMap];

private:
	std::string								m_GlobalWMOName;
	ADT_MODF							m_GlobalWMOPlacementInfo;
#ifdef GAME_MAP_INCLUDE_WMO_AND_M2
	std::shared_ptr<ADT_WMO_Instance>	m_GlobalWMO;
#endif

private: // PARENT
	const std::weak_ptr<MapController>  m_MapController;
};