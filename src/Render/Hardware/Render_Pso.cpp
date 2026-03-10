#include "Render_Pso.h"
#include <iostream>

Render_Pso::Render_Pso()
{
}

Render_Pso::~Render_Pso()
{
	m_pPso->Release();
	m_pPso = nullptr;
}

void Render_Pso::CreatePso(ID3D12Device* _device, std::vector<D3D12_INPUT_ELEMENT_DESC> _inputLayout, ID3D12RootSignature* _rootSignature
	, ID3DBlob* _vsByteCode, ID3DBlob* _psByteCode)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { _inputLayout.data(), (UINT)_inputLayout.size() };
	psoDesc.pRootSignature = _rootSignature;
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(_vsByteCode->GetBufferPointer()),
		_vsByteCode->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(_psByteCode->GetBufferPointer()),
		_psByteCode->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;

	HRESULT hr = _device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pPso));

	if (FAILED(hr))
	{
		std::cout << "Error pso creation" << std::endl;
		return;
	}

	std::cout << "Pso create" << std::endl;
}

ID3D12PipelineState* Render_Pso::GetPso()
{
	if (m_pPso == nullptr)
	{
		std::cout << "Pso -> NULL" << std::endl;
		return nullptr;
	}
	return m_pPso;
}

void Render_Pso::CreatePsoUI(ID3D12Device* _device, std::vector<D3D12_INPUT_ELEMENT_DESC> _inputLayout, ID3D12RootSignature* _rootSignature, ID3DBlob* _vsByteCode, ID3DBlob* _psByteCode)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { _inputLayout.data(), (UINT)_inputLayout.size() };
	psoDesc.pRootSignature = _rootSignature;
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(_vsByteCode->GetBufferPointer()),
		_vsByteCode->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(_psByteCode->GetBufferPointer()),
		_psByteCode->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	HRESULT hr = _device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pPso));

	if (FAILED(hr))
	{
		std::cout << "Error pso UI creation" << std::endl;
	}
}

ID3D12PipelineState* Render_Pso::GetPsoUI()
{
	return nullptr;
}
