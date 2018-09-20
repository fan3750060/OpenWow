#include <stdafx.h>

// General
#include "ConstantBufferOGL.h"

// Additional
#include "OpenGL.h"

ConstantBufferOGL::ConstantBufferOGL(size_t size)
	: m_BufferSize(size)
{
	glGenBuffers(1, &m_GLObj);
	glBindBuffer(GL_UNIFORM_BUFFER, m_GLObj);
	{
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

ConstantBufferOGL::~ConstantBufferOGL()
{
	glDeleteBuffers(1, &m_GLObj);
}

void ConstantBufferOGL::Set(const void* data, size_t size)
{
	assert(size == m_BufferSize);

	glBindBuffer(GL_UNIFORM_BUFFER, m_GLObj);
	{
		GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		memcpy(p, data, size);
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ConstantBufferOGL::Copy(std::shared_ptr<ConstantBuffer> other)
{
	throw std::exception("Not implemented!");
}

void ConstantBufferOGL::Copy(std::shared_ptr<Buffer> other)
{
	Copy(std::dynamic_pointer_cast<ConstantBuffer>(other));
}

bool ConstantBufferOGL::Bind(uint32 id, Shader::ShaderType shaderType, ShaderParameter::Type parameterType)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, id, m_GLObj);
	throw std::exception("Not implemented!");
}

void ConstantBufferOGL::UnBind(uint32 id, Shader::ShaderType shaderType, ShaderParameter::Type parameterType)
{
	throw std::exception("Not implemented!");
}