#pragma once

#include "GeometryBase.h"

class CM2_Pass : public GeometryBase
{
    int32 gBones;
    int32  gColor;
    int32  gIsAnimated;

	int32 gShader;

    int32  gDiffuseTexture[4];
    int32  gSpecularTexture;

    int32  gBlendMode;
    int32  gTextureWeight;

    int32  gBillboard;
    int32  gTextureAnimEnable;
    int32  gTextureAnimMatrix;

public:
	CM2_Pass(RenderDevice* _RenderDevice) : 
		GeometryBase(_RenderDevice, "shaders/M2/M2")
    {
        gBones = getLocation("gBones");
        gColor = getLocation("gColor");
        gIsAnimated = getLocation("gIsAnimated");

		gShader = getLocation("gShader");

        gDiffuseTexture[0] = getLocation("gDiffuseTexture0");
		gDiffuseTexture[1] = getLocation("gDiffuseTexture1");
		gDiffuseTexture[2] = getLocation("gDiffuseTexture2");
		gDiffuseTexture[3] = getLocation("gDiffuseTexture3");

        gSpecularTexture = getLocation("gSpecularTexture");

        gBlendMode = getLocation("gBlendMode");
        gTextureWeight = getLocation("gTextureWeight");

        gBillboard = getLocation("gBillboard");

        gTextureAnimEnable = getLocation("gTextureAnimEnable");
        gTextureAnimMatrix = getLocation("gTextureAnimMatrix");
    }

	// Colors

	inline void SetColor(vec4 _color)
	{
		setVec4(gColor, _color);
	}

    inline void SetAnimated(int32 m_IsAnimated)
    {
        setInt(gIsAnimated, m_IsAnimated);
    }

    inline void SetBones(vector<mat4>& _bones)
    {
        if (_bones.empty())
        {
            fail1();
        }

        m_Shader->setShaderConst(gBones, CONST_FLOAT44, _bones.data(), _bones.size());
    }

	// Shader

	inline void SetShader(int _blendMode)
	{
		setInt(gShader, _blendMode);
	}

	// Textures
 
	inline void SetDiffuseTexture(int index, int TextureUnit)
	{
		setTexture(gDiffuseTexture[index], TextureUnit);
	}

	inline void SetSpecularTexture(int TextureUnit)
	{
		setTexture(gSpecularTexture, TextureUnit);
	}

	// Blending & Alpha

	inline void SetBlendMode(int _blendMode)
	{
		setInt(gBlendMode, _blendMode);
	}

	// R_Texture Weight
	inline void SetTextureWeight(float _weight)
	{
		setFloat(gTextureWeight, _weight);
	}


	// Billboard
	inline void SetBillboard(int _flag)
	{
		setInt(gBillboard, _flag);
	}

	// TextureAnim
	inline void SetTextureAnimEnable(int _flag)
	{
		setInt(gTextureAnimEnable, _flag);
	}

	inline void SetTextureAnimMatrix(mat4 _matrix)
	{
		setMat4(gTextureAnimMatrix, _matrix);
	}
};
