#pragma once
#include <dxgi1_4.h>
#include "d3dx12.h"
class Render_Swapchain
{
private:
	IDXGIFactory4* m_pDXGIFactory = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;

	static const int m_bufferCountSwapchain = 2;

	ID3D12Resource* m_pSwapchainBuffer[m_bufferCountSwapchain] = { nullptr, nullptr };

	int m_currentBackBuffer = 0;

public:
	Render_Swapchain();
	~Render_Swapchain();

	

	void CreateFactory();
	void CreateSwapchain(HWND _hWnd, int _width, int _height, ID3D12CommandQueue* _commandQueue);
	IDXGISwapChain* GetSwapchain();

	static const int GetBufferCountSwapchain();

	void CreateBuffer();
	ID3D12Resource* GetCurrentBackBuffer();

	int GetCurrentBackBufferIndex() const;
	void SetCurrentBackBufferIndex(int _index);

	void Resize(int _width, int _height);

};

