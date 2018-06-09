#pragma once

// FORWARD BEGIN
class WMO;
// FORWARD END

#include "WMO_Headers.h"

class WMOMaterial : public CRefItem
{
public:
	WMOMaterial(const WMO* _parentWMO, const WMOMaterialDef& _proto);
	~WMOMaterial();

	uint32 GetBlendMode() const { return matDef.blendMode; }
	uint32 GetDiffuseColor() const { return matDef.diffColor; }

	CBgra EmissiveColor() const { return matDef.emissive_color; }

	bool IsLightingDisabled() const { return matDef.flags.F_UNLIT; }
	bool IsFogDisabled() const { return matDef.flags.F_UNFOGGED; }
	bool IsTwoSided() const { return matDef.flags.F_UNCULLED; }
	bool IsDarkened() const { return matDef.flags.F_EXTLIGHT; }
	bool IsUnshaded() const { return matDef.flags.F_SIDN; }
	bool IsLightingRelated() const { return matDef.flags.F_WINDOW; }
	bool IsTexClampS() const { return matDef.flags.F_CLAMP_S; }
	bool IsTesClampT() const { return matDef.flags.F_CLAMP_T; }

private:
	const WMO* m_ParentWMO;

public:
	WMOMaterialDef matDef;
	SmartTexturePtr texture;
};