#pragma once

class ConstantBuffer;

class MeshDX11 : public IMesh
{
public:
	MeshDX11(ID3D11Device2* pDevice);
	virtual ~MeshDX11();

	virtual void AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<Buffer> buffer);
	virtual void SetIndexBuffer(std::shared_ptr<Buffer> buffer);

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology);

	virtual void SetMaterial(std::shared_ptr<const Material> material);
	virtual std::shared_ptr<const Material> GetMaterial() const;

	virtual bool Render(RenderEventArgs& renderArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0);

	virtual bool Accept(IVisitor& visitor);

private:
	typedef std::map<BufferBinding, std::shared_ptr<Buffer> > BufferMap;
	BufferMap m_VertexBuffers;
	std::shared_ptr<Buffer> m_pIndexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;
	std::shared_ptr<const Material> m_pMaterial;

	ATL::CComPtr<ID3D11Device2> m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2> m_pDeviceContext;
};