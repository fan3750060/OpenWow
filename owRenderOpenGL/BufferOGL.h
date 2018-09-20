#pragma once

class BufferOGL : public Buffer
{
public:
	BufferOGL(UINT bindFlags, const void* data, size_t count, UINT stride);
	~BufferOGL();

	// Bind the buffer to a particular attribute ID or slot
	virtual bool Bind(uint32 id, Shader::ShaderType shaderType, ShaderParameter::Type parameterType);
	virtual void UnBind(uint32 id, Shader::ShaderType shaderType, ShaderParameter::Type parameterType);

	// Copy the contents of another buffer into this one.
	// Buffers must be the same size (in bytes).
	virtual void Copy(std::shared_ptr<Buffer> other);

	// Is this an index buffer or an attribute/vertex buffer?
	virtual BufferType GetType() const;
	// How many elements does this buffer contain?
	virtual uint32 GetElementCount() const;

private:
	uint32  m_GLObj;

private:
	// The stride of the vertex buffer in bytes.
	UINT m_uiStride;

	// How this buffer should be bound.
	UINT m_BindFlags;

	// The number of elements in this buffer.
	UINT m_uiCount;

	bool m_bIsBound;
};
