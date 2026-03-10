#pragma once
#include "d3dx12.h"
#include "dxgi1_4.h"
class Render_Heap
{
private :
	ID3D12DescriptorHeap* m_pRtvHeap = nullptr;
	ID3D12DescriptorHeap* m_pDsvHeap = nullptr;

	UINT m_RtvDescriptorSize = 0;
	ID3D12Resource* m_pDepthStencilBuffer = nullptr;

	ID3D12DescriptorHeap* m_pCbvSrvUavHeap = nullptr;
	UINT m_CbvSrvUavDescriptorSize = 0;

public :
	Render_Heap();
	~Render_Heap();

	void CreateDescriptorHeaps(ID3D12Device* _device
		, const int _bufferCountSwapchain);

	void CreateRtv(ID3D12Device* _device, IDXGISwapChain* _swapchain, const int _bufferCountSwapchain);

	void CreateDsv(ID3D12Device* _device, int _width, int _height);

	void CreateSrv(ID3D12Device* _device, ID3D12Resource* _texture);

	ID3D12DescriptorHeap* GetCbvSrvUavHeap();

	ID3D12DescriptorHeap* GetRtvHeap();
	ID3D12DescriptorHeap* GetDsvHeap();
	UINT GetRtvDescriptorSize() const;
	UINT GetCbvSrvUavDescriptorSize() const;

};

