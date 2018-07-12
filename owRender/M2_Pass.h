#pragma once

#include "GeometryBase.h"

class CM2_Pass : public GeometryBase
{
	// Bones & animated
    int32 gIsAnimated;
	int32 gBones;
	int32 gBonesStartIndex;
	int32 gBonesCount;
	int32 gBonesMaxInfluences;

	// Blend
	int32 gShader;
	int32 gBlendMode;

	// Colors
	int32 gColorEnable;
	int32 gColor;

	int32 gColorDoodadEnable;
	int32 gColorDoodad;

	// Textures
    int32  gDiffuseTexture[2];
    int32  gSpecularTexture;

	int32  gTextureWeightEnable;
    int32  gTextureWeight;

    int32  gTextureAnimEnable;
    int32  gTextureAnimMatrix;

public:
	CM2_Pass(RenderDevice* _RenderDevice) : 
		GeometryBase(_RenderDevice, "shaders/M2/M2")
    {
		gIsAnimated = getLocation("gIsAnimated");
        gBones = getLocation("gBones");
		gBonesStartIndex = getLocation("gBonesStartIndex");
		gBonesCount = getLocation("gBonesCount");
		gBonesMaxInfluences = getLocation("gBonesMaxInfluences");

		// Blend
		gShader = getLocation("gShader");
		gBlendMode = getLocation("gBlendMode");

		// Colors
		gColorEnable = getLocation("gColorEnable");
        gColor = getLocation("gColor");

		gColorDoodadEnable = getLocation("gColorDoodadEnable");
		gColorDoodad = getLocation("gColorDoodad");

		// Textures
        gDiffuseTexture[0] = getLocation("gDiffuseTexture0");
		gDiffuseTexture[1] = getLocation("gDiffuseTexture1");
        gSpecularTexture = getLocation("gSpecularTexture");

		gTextureWeightEnable = getLocation("gTextureWeightEnable");
        gTextureWeight = getLocation("gTextureWeight");

        gTextureAnimEnable = getLocation("gTextureAnimEnable");
        gTextureAnimMatrix = getLocation("gTextureAnimMatrix");
    }

	



    inline void SetAnimated(int32 m_IsAnimated)
    {
        setInt(gIsAnimated, m_IsAnimated);
    }
    inline void SetBones(vector<mat4>& _bones)
    {
		assert1(_bones.size() > 0);
        m_Shader->setShaderConst(gBones, CONST_FLOAT44, _bones.data(), static_cast<uint32>(_bones.size()));
    }
	inline void SetBonesStartIndex(int _index)
	{
		setInt(gBonesStartIndex, _index);
	}
	inline void SetBonesCount(int _index)
	{
		setInt(gBonesCount, _index);
	}
	inline void SetBonesMaxInfluences(int _index)
	{
		setInt(gBonesMaxInfluences, _index);
	}

	// Blend
	inline void SetShader(int _blendMode)
	{
		setInt(gShader, _blendMode);
	}
	inline void SetBlendMode(int _blendMode)
	{
		setInt(gBlendMode, _blendMode);
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

	// Textures
 
	inline void SetDiffuseTexture(int index, int TextureUnit)
	{
		setTexture(gDiffuseTexture[index], TextureUnit);
	}
	inline void SetSpecularTexture(int TextureUnit)
	{
		setTexture(gSpecularTexture, TextureUnit);
	}

	inline void SetTextureWeightEnable(bool _value)
	{
		setInt(gTextureWeightEnable, _value);
	}
	inline void SetTextureWeight(float _weight)
	{
		setFloat(gTextureWeight, _weight);
	}

	inline void SetTextureAnimEnable(bool _flag)
	{
		setInt(gTextureAnimEnable, _flag);
	}
	inline void SetTextureAnimMatrix(mat4 _matrix)
	{
		setMat4(gTextureAnimMatrix, _matrix);
	}
};
