#pragma once

class ShaderOGL;

class MeshOGL : public Mesh
{
public:
	MeshOGL();
	virtual ~MeshOGL();

	virtual void AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<Buffer> buffer);
	virtual void SetIndexBuffer(std::shared_ptr<Buffer> buffer);

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology);

	virtual void SetMaterial(std::shared_ptr<Material> material);
	virtual std::shared_ptr<Material> GetMaterial() const;

	virtual void Render(RenderEventArgs& renderArgs, std::shared_ptr<ConstantBuffer> constantBuffer);

	virtual void Accept(IVisitor& visitor);

private:
	GLuint m_GLObj;

private:
	void Commit(std::weak_ptr<ShaderOGL> _shader);

private:
	typedef std::map<BufferBinding, std::shared_ptr<Buffer> > BufferMap;
	BufferMap m_VertexBuffers;
	size_t m_VertexCount;
	std::shared_ptr<Buffer> m_pIndexBuffer;
	GLuint m_PrimitiveTopology;
	std::shared_ptr<Material> m_pMaterial;

	bool m_bIsDirty;
};