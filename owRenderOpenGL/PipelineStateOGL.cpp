#include <stdafx.h>

#include "PipelineStateOGL.h"

// Additional
#include "ShaderOGL.h"
#include "OpenGL.h"

GLbitfield TranslateShaderBitType(Shader::ShaderType _type)
{
	GLbitfield result = GL_VERTEX_SHADER;
	switch (_type)
	{
	case Shader::ShaderType::VertexShader:
		result = GL_VERTEX_SHADER_BIT;
		break;
	case Shader::ShaderType::PixelShader:
		result = GL_FRAGMENT_SHADER_BIT;
		break;
	case Shader::ShaderType::GeometryShader:
		result = GL_GEOMETRY_SHADER_BIT;
		break;
	case Shader::ShaderType::TessellationControlShader:
		result = GL_TESS_CONTROL_SHADER_BIT;
		break;
	case Shader::ShaderType::TessellationEvaluationShader:
		result = GL_TESS_EVALUATION_SHADER_BIT;
		break;
	case Shader::ShaderType::ComputeShader:
		result = GL_COMPUTE_SHADER_BIT;
		break;
	default:
		Log::Error("Unknown shader type factor.");
	}

	return result;
}

PipelineStateOGL::PipelineStateOGL()
	: m_BlendState()
	, m_RasterizerState()
	, m_DepthStencilState()
{
	glGenProgramPipelines(1, &m_GLProgramPipeline);
}

PipelineStateOGL::~PipelineStateOGL()
{
	if (m_GLProgramPipeline != 0)
	{
		glDeleteProgramPipelines(1, &m_GLProgramPipeline);
		m_GLProgramPipeline = 0;
	}
}

void PipelineStateOGL::SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader)
{
	std::shared_ptr<ShaderOGL> shaderOGL = std::dynamic_pointer_cast<ShaderOGL>(pShader);
	m_Shaders[type] = shaderOGL;

	glUseProgramStages(m_GLProgramPipeline, TranslateShaderBitType(type), shaderOGL->GetGLObject());
}

std::shared_ptr<Shader> PipelineStateOGL::GetShader(Shader::ShaderType type) const
{
	ShaderMap::const_iterator iter = m_Shaders.find(type);
	if (iter != m_Shaders.end())
	{
		return iter->second;
	}

	return nullptr;
}

const PipelineState::ShaderMap& PipelineStateOGL::GetShaders() const
{
	return m_Shaders;
}

void PipelineStateOGL::SetBlendState(const BlendState& blendState)
{
	m_BlendState = dynamic_cast<const BlendStateOGL&>(blendState);
}

BlendState& PipelineStateOGL::GetBlendState()
{
	return m_BlendState;
}

void PipelineStateOGL::SetRasterizerState(const RasterizerState& rasterizerState)
{
	m_RasterizerState = dynamic_cast<const RasterizerStateOGL&>(rasterizerState);
}

RasterizerState& PipelineStateOGL::GetRasterizerState()
{
	return m_RasterizerState;
}

void PipelineStateOGL::SetDepthStencilState(const DepthStencilState& depthStencilState)
{
	m_DepthStencilState = dynamic_cast<const DepthStencilStateOGL&>(depthStencilState);
}

DepthStencilState& PipelineStateOGL::GetDepthStencilState()
{
	return m_DepthStencilState;
}

void PipelineStateOGL::SetRenderTarget(std::shared_ptr<RenderTarget> renderTarget)
{
	m_RenderTarget = renderTarget;
}

std::shared_ptr<RenderTarget> PipelineStateOGL::GetRenderTarget() const
{
	return m_RenderTarget;
}

void PipelineStateOGL::Bind()
{
	if (m_RenderTarget)
	{
		m_RenderTarget->Bind();
	}

	m_BlendState.Bind();
	m_RasterizerState.Bind();
	m_DepthStencilState.Bind();

	
	for (auto shader : m_Shaders)
	{
		std::shared_ptr<Shader> pShader = shader.second;
		if (pShader)
		{
			//glActiveShaderProgram(m_GLProgramPipeline, )
			pShader->Bind();
		}
	}
	glBindProgramPipeline(m_GLProgramPipeline);
}

void PipelineStateOGL::UnBind()
{
	if (m_RenderTarget)
	{
		m_RenderTarget->UnBind();
	}

	for (auto shader : m_Shaders)
	{
		std::shared_ptr<Shader> pShader = shader.second;
		if (pShader)
		{
			pShader->UnBind();
		}
	}
}