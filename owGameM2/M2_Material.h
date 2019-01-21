#pragma once

class M2_Material : public Material
{
public:
	M2_Material();
	virtual ~M2_Material();

	void SetAnimated(uint32 value);
	void SetMaxInfluences(uint32 value);
	void SetBlendMode(uint32 value);
	void SetNewShader(uint32 value);

	void SetTextureWeightEnable(uint32 value);
	void SetTextureWeight(float value);

	void SetTextureAnimEnable(uint32 value);
	void SetTextureAnimMatrix(cmat4 value);

	void SetColorEnable(uint32 value);
	void SetColor(vec4 value);

	void SetBones(const std::vector<mat4>& bones);
	

protected:
	void UpdateConstantBuffer() const override;

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: gIsAnimated(0)
			, gBonesMaxInfluences(0)
		{}
		uint32 gIsAnimated;
		uint32 gBonesMaxInfluences;
		uint32 gBlendMode;
		uint32 gShader;
		
		uint32 gColorEnable;
		uint32 gTextureWeightEnable;
		float  gTextureWeight;
		uint32 gTextureAnimEnable;

		mat4 gTextureAnimMatrix;

		vec4 gColor;

		mat4 Bones[200];
		//-------------------------- ( 32 bytes )
	};
	MaterialProperties* m_pProperties;
};