#pragma once

class M2_Material : public Material
{
public:
	M2_Material();
	virtual ~M2_Material();

	void SetAnimated(uint32 value);
	void SetBones(const std::vector<mat4>& bones);
	void SetMaxInfluences(uint32 value);

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
		vec2 pad;
		mat4 Bones[200];
		//-------------------------- ( 32 bytes )
	};
	MaterialProperties* m_pProperties;
};