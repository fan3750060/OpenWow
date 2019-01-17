#pragma once

class UI_Font_Material : public Material
{
public:
	UI_Font_Material();
	virtual ~UI_Font_Material();

	void SetColor(vec4 color);
	void SetOffset(vec2 offset);

protected:
	void UpdateConstantBuffer() const override;

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: Color(1.0f, 1.0f, 1.0f, 1.0f)
			, Offset(0.0f, 0.0f)
		{}
		vec4 Color;
		vec2 Offset;
		vec2 Padding;
	};
	MaterialProperties* m_pProperties;
};