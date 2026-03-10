#pragma once
#include "d3dUtil.h"
#include "Vertex.h"
#include <vector>
#include <memory>
class Render_Mesh
{
private :
	std::unique_ptr<MeshGeometry> m_meshGeo = nullptr;
	UINT m_indexCount = 0;

public :

	Render_Mesh();
	~Render_Mesh();
	void Build(ID3D12Device* _device, ID3D12GraphicsCommandList* _cmdList
		, std::vector<Vertex> _v, std::vector<std::uint16_t> _indices, std::string _submeshName);

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const;

	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const;

	UINT GetIndexCount();
};

