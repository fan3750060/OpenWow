#pragma once

#include "GeometryBase.h"

class CM2_Pass : public GeometryBase
{
    int32 gBones;

	int32 gColorEnable;
    int32 gColor;

	int32 gColorDoodadEnable;
	int32 gColorDoodad;

    int32  gIsAnimated;

	int32 gShader;

    int32  gDiffuseTexture[2];
    int32  gSpecularTexture;

    int32  gBlendMode;

	int32  gTextureWeightEnable;
    int32  gTextureWeight;

    int32  gBillboard;

    int32  gTextureAnimEnable;
    int32  gTextureAnimMatrix;

public:
	CM2_Pass(RenderDevice* _RenderDevice) : 
		GeometryBase(_RenderDevice, "shaders/M2/M2")
    {
		gIsAnimated = getLocation("gIsAnimated");
        gBones = getLocation("gBones");


		gShader = getLocation("gShader");
		gBlendMode = getLocation("gBlendMode");



		gColorEnable = getLocation("gColorEnable");
        gColor = getLocation("gColor");

		gColorDoodadEnable = getLocation("gColorDoodadEnable");
		gColorDoodad = getLocation("gColorDoodad");

		

        gDiffuseTexture[0] = getLocation("gDiffuseTexture0");
		gDiffuseTexture[1] = getLocation("gDiffuseTexture1");

        gSpecularTexture = getLocation("gSpecularTexture");

        

		gTextureWeightEnable = getLocation("gTextureWeightEnable");
        gTextureWeight = getLocation("gTextureWeight");

        gBillboard = getLocation("gBillboard");

        gTextureAnimEnable = getLocation("gTextureAnimEnable");
        gTextureAnimMatrix = getLocation("gTextureAnimMatrix");
    }

	// Colors

	inline void SetColorEnable(bool _value)
	{
		setInt(gColorEnable, _value);
	}
	inline void SetColor(vec4 _color)
	{
		setVec4(gColor, _color);
	}

	inline void SetColorDoodadEnable(bool _value)
	{
		setInt(gColorDoodadEnable, _value);
	}
	inline void SetColorDoodad(vec4 _color)
	{
		setVec4(gColorDoodad, _color);
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

        m_Shader->setShaderConst(gBones, CONST_FLOAT44, _bones.data(), static_cast<uint32>(_bones.size()));
    }

	// Shader

	inline void SetShader(int _blendMode)
	{
		setInt(gShader, _blendMode);
	}
	// Blending & Alpha

	inline void SetBlendMode(int _blendMode)
	{
		setInt(gBlendMode, _blendMode);
	}
	// Billboard
	inline void SetBillboard(int _flag)
	{
		setInt(gBillboard, _flag);
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




	// R_Texture Weight
	inline void SetTextureWeightEnable(bool _value)
	{
		setInt(gTextureWeightEnable, _value);
	}
	inline void SetTextureWeight(float _weight)
	{
		setFloat(gTextureWeight, _weight);
	}




	// TextureAnim
	inline void SetTextureAnimEnable(bool _flag)
	{
		setInt(gTextureAnimEnable, _flag);
	}

	inline void SetTextureAnimMatrix(mat4 _matrix)
	{
		setMat4(gTextureAnimMatrix, _matrix);
	}
};
