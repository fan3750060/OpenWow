#include "stdafx.h"

// General
#include "M2_Part_Material.h"

// Additional
enum modelPixelShaders
{
	PS_Combiners_Opaque,
	PS_Combiners_Mod,
	PS_Combiners_Opaque_Mod,
	PS_Combiners_Opaque_Mod2x,
	PS_Combiners_Opaque_Mod2xNA,
	PS_Combiners_Opaque_Opaque,
	PS_Combiners_Mod_Mod,
	PS_Combiners_Mod_Mod2x,
	PS_Combiners_Mod_Add,
	PS_Combiners_Mod_Mod2xNA,
	PS_Combiners_Mod_AddNA,
	PS_Combiners_Mod_Opaque,
	PS_Combiners_Opaque_Mod2xNA_Alpha,
	PS_Combiners_Opaque_AddAlpha,
	PS_Combiners_Opaque_AddAlpha_Alpha,
	PS_Combiners_Opaque_Mod2xNA_Alpha_Add,
	PS_Combiners_Mod_AddAlpha,
	PS_Combiners_Mod_AddAlpha_Alpha,
	PS_Combiners_Opaque_Alpha_Alpha,
	PS_Combiners_Opaque_Mod2xNA_Alpha_3s,
	PS_Combiners_Opaque_AddAlpha_Wgt,
	PS_Combiners_Mod_Add_Alpha,
	PS_Combiners_Opaque_ModNA_Alpha,
	PS_Combiners_Mod_AddAlpha_Wgt,
	PS_Combiners_Opaque_Mod_Add_Wgt,
	PS_Combiners_Opaque_Mod2xNA_Alpha_UnshAlpha,
	PS_Combiners_Mod_Dual_Crossfade,
	PS_Combiners_Opaque_Mod2xNA_Alpha_Alpha,
	PS_Combiners_Mod_Masked_Dual_Crossfade,
	PS_Combiners_Opaque_Alpha,
	PS_Guild,
	PS_Guild_NoBorder,
	PS_Guild_Opaque,
	PS_Combiners_Mod_Depth,
	PS_Illum,
	PS_Combiners_Mod_Mod_Mod_Const,
};

CM2_Part_Material::CM2_Part_Material(const SM2_Material& _proto)
{
	isUNLIT = _proto.flags.UNLIT == 0;
	isUNFOGGED = _proto.flags.UNFOGGED == 0;
	isTWOSIDED = _proto.flags.TWOSIDED == 0;
	isDEPTHTEST = _proto.flags.DEPTHTEST == 0;
	isDEPTHWRITE = _proto.flags.DEPTHWRITE == 0;

	blending_mode = _proto.blending_mode;

}

void CM2_Part_Material::Set() const
{
	_Render->getTechniquesMgr()->M2_Pass->SetBlendMode(blending_mode);

	_Render->r.setDepthTest(isDEPTHTEST);
	_Render->r.setDepthMask(isDEPTHWRITE);

	//_Render->r.setAlphaToCoverage(true);

	switch (blending_mode)
	{
	case SM2_Material::M2BLEND_OPAQUE:
		_Render->getRenderStorage()->SetEGxBlend(0);
		break;

	case SM2_Material::M2BLEND_ALPHA_KEY:
		_Render->getRenderStorage()->SetEGxBlend(1);
		break;

	case SM2_Material::M2BLEND_ALPHA:
		_Render->getRenderStorage()->SetEGxBlend(2);
		break;

	case SM2_Material::M2BLEND_NO_ALPHA_ADD:
		_Render->getRenderStorage()->SetEGxBlend(10);
		break;

	case SM2_Material::M2BLEND_ADD:
		_Render->getRenderStorage()->SetEGxBlend(3);
		break;

	case SM2_Material::M2BLEND_MOD:
		_Render->getRenderStorage()->SetEGxBlend(4);
		break;

	case SM2_Material::M2BLEND_MOD2X:
		_Render->getRenderStorage()->SetEGxBlend(5);
		break;

	default:
		fail1();
	}
}
