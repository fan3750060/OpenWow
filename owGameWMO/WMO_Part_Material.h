#pragma once

#include "WMO_Headers.h"

// FORWARD BEGIN
class WMO;
// FORWARD END

class WMO_Part_Material : public CRefItem
{
public:
	WMO_Part_Material(const WMO* _parentWMO, const WMO_MaterialDef& _proto);
	~WMO_Part_Material();

	uint32 GetBlendMode() const { return matDef.blendMode; }
	uint32 GetDiffuseColor() const { return matDef.diffColor; }

	CBgra EmissiveColor() const { return matDef.emissive_color; }

	bool IsLightingDisabled() const { return matDef.flags.DisableLighting; }
	bool IsFogDisabled() const { return matDef.flags.DisableFog; }
	bool IsTwoSided() const { return matDef.flags.IsTwoSided; }
	bool IsDarkened() const { return matDef.flags.F_EXTLIGHT; }
	bool IsUnshaded() const { return matDef.flags.F_SIDN; }
	bool IsLightingRelated() const { return matDef.flags.F_WINDOW; }
	bool IsTexClampS() const { return matDef.flags.TextureClampS; }
	bool IsTesClampT() const { return matDef.flags.TextureClampT; }

private:
	const WMO* m_ParentWMO;

public:
	WMO_MaterialDef matDef;
	SmartTexturePtr texture;
};