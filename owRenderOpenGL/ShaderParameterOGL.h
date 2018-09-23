#pragma once

class ShaderParameterOGL : public ShaderParameter
{
public:
	ShaderParameterOGL();
	ShaderParameterOGL(cstring name, UINT slotID, std::shared_ptr<Shader> shader, Type parameterType);

	bool IsValid() const;
	Type GetType() const;

	virtual void Bind();
	virtual void UnBind();

protected:
	void SetConstantBuffer(std::shared_ptr<ConstantBuffer> buffer);
	void SetTexture(std::shared_ptr<Texture> texture);
	void SetSampler(std::shared_ptr<SamplerState> sampler);
	void SetStructuredBuffer(std::shared_ptr<StructuredBuffer> rwBuffer);

private:
	std::string                     m_Name;
	UINT                            m_uiSlotID;
	std::weak_ptr<Shader>           m_Shader;
	Type                            m_ParameterType;

	std::weak_ptr<ConstantBuffer>   m_pConstantBuffer;
	std::weak_ptr<Texture>          m_pTexture;
	std::weak_ptr<SamplerState>     m_pSamplerState;
	std::weak_ptr<StructuredBuffer> m_pStructuredBuffer;
};