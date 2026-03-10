#include "Render_Heap.h"
#include <iostream>
Render_Heap::Render_Heap()
{
}

Render_Heap::~Render_Heap()
{
	m_pRtvHeap->Release();
	m_pDsvHeap->Release();
	m_pDepthStencilBuffer->Release();
	m_pRtvHeap = nullptr;
	m_pDsvHeap = nullptr;
	m_pDepthStencilBuffer = nullptr;
	m_pCbvSrvUavHeap->Release();
	m_pCbvSrvUavHeap = nullptr;
}

void Render_Heap::CreateDescriptorHeaps(ID3D12Device* _device, const int _bufferCountSwapchain)
{

	if(m_pDsvHeap || m_pRtvHeap)
	{
		std::cout << "Descriptor heap already created." << std::endl;
		return;
	}

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = 10;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_pRtvHeap));


	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 10;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	_device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_pDsvHeap));

	D3D12_DESCRIPTOR_HEAP_DESC cbvSrvHeapDesc = {};
	cbvSrvHeapDesc.NumDescriptors = 10; 
	cbvSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvSrvHeapDesc.NodeMask = 0;

	_device->CreateDescriptorHeap(&cbvSrvHeapDesc, IID_PPV_ARGS(&m_pCbvSrvUavHeap));
	m_CbvSrvUavDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	std::cout << "Heaps created" << std::endl;
}

void Render_Heap::CreateRtv(ID3D12Device* _device, IDXGISwapChain* _swapchain
	, const int _bufferCountSwapchain)
{
	m_RtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);


	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart());

	for (int i = 0; i < _bufferCountSwapchain; i++)
	{
		ID3D12Resource* backBuffer = nullptr;
		HRESULT hr = _swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffer));

		if (FAILED(hr))
		{
			std::cout << "Error Rtv creation" << std::endl;
		}

		_device->CreateRenderTargetView(backBuffer, nullptr, rtvHeapHandle);

		rtvHeapHandle.Offset(1, m_RtvDescriptorSize);

		if (backBuffer != nullptr)
		{
			backBuffer->Release();
		}
	}
	std::cout << "Rtv created" << std::endl;
}

void Render_Heap::CreateDsv(ID3D12Device* _device, int _width, int _height)
{

	if (m_pDepthStencilBuffer)
	{
		m_pDepthStencilBuffer->Release();
		m_pDepthStencilBuffer = nullptr;
	}

	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = _width;
	depthStencilDesc.Height = _height;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	D3D12_CLEAR_VALUE optClear;
	optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;
	CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	_device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE
		, &optClear, IID_PPV_ARGS(&m_pDepthStencilBuffer)); _device->CreateDepthStencilView(m_pDepthStencilBuffer
			, nullptr, m_pDsvHeap->GetCPUDescriptorHandleForHeapStart());

	std::cout << "Dsv created" << std::endl;
}

void Render_Heap::CreateSrv(ID3D12Device* _device, ID3D12Resource* _texture)
{
	if (!_texture) {
		std::cout << "No Texture" << std::endl;
		return;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = _texture->GetDesc().Format; 
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(m_pCbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart());

	_device->CreateShaderResourceView(_texture, &srvDesc, hDescriptor);

	std::cout << "Srv created" << std::endl;
}

ID3D12DescriptorHeap* Render_Heap::GetCbvSrvUavHeap()
{
	return m_pCbvSrvUavHeap;
}

ID3D12DescriptorHeap* Render_Heap::GetRtvHeap()
{
	return m_pRtvHeap;
}

ID3D12DescriptorHeap* Render_Heap::GetDsvHeap()
{
	return m_pDsvHeap;
}

UINT Render_Heap::GetRtvDescriptorSize() const
{
	return m_RtvDescriptorSize;
}

UINT Render_Heap::GetCbvSrvUavDescriptorSize() const
{
	return m_CbvSrvUavDescriptorSize;
}

