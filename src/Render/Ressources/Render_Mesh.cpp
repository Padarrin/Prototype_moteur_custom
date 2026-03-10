#include "Render_Mesh.h"


Render_Mesh::Render_Mesh()
 {
}

Render_Mesh::~Render_Mesh()
{
    
    m_meshGeo->VertexBufferGPU->Release();
    m_meshGeo->IndexBufferGPU->Release();

    m_meshGeo->VertexBufferUploader->Release();
	m_meshGeo->IndexBufferUploader->Release();

    m_meshGeo->VertexBufferCPU->Release();
    m_meshGeo->IndexBufferCPU->Release();
    m_meshGeo.reset();
    m_meshGeo = nullptr;
}

void Render_Mesh::Build(ID3D12Device* _device, ID3D12GraphicsCommandList* _cmdList
    , std::vector<Vertex> _v, std::vector<std::uint16_t> _indices, std::string _submeshName)
{
    m_meshGeo = std::make_unique<MeshGeometry>();
    m_meshGeo->Name = _submeshName;

    std::vector<std::uint16_t> indices;

    m_indexCount = (UINT)_indices.size();

    const UINT vbByteSize = (UINT)_v.size() * sizeof(Vertex);
    const UINT ibByteSize = (UINT)_indices.size() * sizeof(std::uint16_t);

    m_meshGeo->VertexByteStride = sizeof(Vertex);
    m_meshGeo->VertexBufferByteSize = vbByteSize;
    m_meshGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
    m_meshGeo->IndexBufferByteSize = ibByteSize;

    D3DCreateBlob(vbByteSize, &m_meshGeo->VertexBufferCPU);
    CopyMemory(m_meshGeo->VertexBufferCPU->GetBufferPointer(), _v.data(), vbByteSize);

    D3DCreateBlob(ibByteSize, &m_meshGeo->IndexBufferCPU);
    CopyMemory(m_meshGeo->IndexBufferCPU->GetBufferPointer(), _indices.data(), ibByteSize);

    m_meshGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(_device,
        _cmdList, _v.data(), vbByteSize, m_meshGeo->VertexBufferUploader);

    m_meshGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(_device,
        _cmdList, _indices.data(), ibByteSize, m_meshGeo->IndexBufferUploader);

    SubmeshGeometry submesh;
    submesh.IndexCount = (UINT)_indices.size();
    submesh.StartIndexLocation = 0;
    submesh.BaseVertexLocation = 0;

    m_meshGeo->DrawArgs[_submeshName] = submesh;
}

D3D12_VERTEX_BUFFER_VIEW Render_Mesh::GetVertexBufferView() const
{
    return m_meshGeo->VertexBufferView();
}

D3D12_INDEX_BUFFER_VIEW Render_Mesh::GetIndexBufferView() const
{
    return m_meshGeo->IndexBufferView();
}

UINT Render_Mesh::GetIndexCount()
{
    return m_indexCount;
}
