#pragma once

class ShaderOGL;

class MeshOGL : public Mesh
{
public:
	MeshOGL();
	virtual ~MeshOGL();

	virtual void AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<Buffer> buffer);
	virtual void SetIndexBuffer(std::shared_ptr<Buffer> buffer);

	virtual void SetMaterial(std::shared_ptr<Material> material);
	virtual std::shared_ptr<Material> GetMaterial() const;

	virtual void Render(RenderEventArgs& renderArgs);

	virtual void Accept(IVisitor& visitor);

private:
	GLuint m_GLObj;

private:
	void ResolveDirty(std::weak_ptr<ShaderOGL> _shader);

private:
	typedef std::map<BufferBinding, std::shared_ptr<Buffer> > BufferMap;
	BufferMap m_VertexBuffers;
	std::shared_ptr<Buffer> m_pIndexBuffer;
	std::shared_ptr<Material> m_pMaterial;

	bool m_bIsDirty;
};