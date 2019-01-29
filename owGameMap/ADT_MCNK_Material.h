#pragma once

// FORWARD BEGIN
class ADT;
// FORWARD END

class ADT_MCNK_Material : public MaterialWrapper
{
public:
	ADT_MCNK_Material(const std::weak_ptr<ADT> _parentADT);
	virtual ~ADT_MCNK_Material();

	void SetLayersCnt(uint32 value);

protected:
	void UpdateConstantBuffer() const;

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: m_DiffuseColor(1, 1, 1, 1)
		{}
		vec4   m_DiffuseColor;
		uint32 m_LayersCnt;
		vec3   m_Pad;
		//-------------------------- ( 32 bytes )
	};
	MaterialProperties* m_pProperties;
};