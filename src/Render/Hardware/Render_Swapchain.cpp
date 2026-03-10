#include "Render_Swapchain.h"
#include <iostream>

Render_Swapchain::Render_Swapchain()
	: m_pDXGIFactory(nullptr),
	  m_pSwapChain(nullptr),
	  m_currentBackBuffer(0)
{
	for (int i = 0; i < m_bufferCountSwapchain; ++i)
	{
		m_pSwapchainBuffer[i] = nullptr;
	}
}

Render_Swapchain::~Render_Swapchain()
{
	
	for (int i = 0; i < m_bufferCountSwapchain; ++i)
	{
		if (m_pSwapchainBuffer[i])
		{
			m_pSwapchainBuffer[i]->Release();
			m_pSwapchainBuffer[i] = nullptr;
		}
	}

	m_pSwapChain->Release();
	m_pSwapChain = nullptr;
	m_pDXGIFactory->Release();
	m_pDXGIFactory = nullptr;
	
}


void Render_Swapchain::CreateFactory()
{
	if (m_pDXGIFactory)
	{
		std::cout << "Factory already created" << std::endl;
		return;
	}
	CreateDXGIFactory1(IID_PPV_ARGS(&m_pDXGIFactory));

	std::cout << "Factory created" << std::endl;
}

void Render_Swapchain::CreateSwapchain(HWND _hWnd, int _width, int _height, ID3D12CommandQueue* _commandQueue)
{
	if (m_pDXGIFactory == nullptr)
	{
		std::cout << "Factory -> NULL" << std::endl;
		return;
	}
	if(m_pSwapChain)
	{ 
		std::cout << "Swapchain already created." << std::endl;
		return;
	}

	DXGI_SWAP_CHAIN_DESC swapchain_desc;
	swapchain_desc.BufferDesc.Width = _width;
	swapchain_desc.BufferDesc.Height = _height;
	swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapchain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain_desc.BufferCount = m_bufferCountSwapchain;
	swapchain_desc.OutputWindow = _hWnd;
	swapchain_desc.Windowed = true;
	swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapchain_desc.SampleDesc.Count = 1;
	swapchain_desc.SampleDesc.Quality = 0;

	m_pDXGIFactory->CreateSwapChain(_commandQueue, &swapchain_desc, &m_pSwapChain);

	std::cout << "Swapchain created." << std::endl;
}

IDXGISwapChain* Render_Swapchain::GetSwapchain()
{
	if(m_pSwapChain == nullptr)
	{
		std::cout << "Swapchain -> NULL" << std::endl;
		return nullptr;
	}
	return m_pSwapChain;
}

const int Render_Swapchain::GetBufferCountSwapchain()
{
	return m_bufferCountSwapchain;
}

void Render_Swapchain::CreateBuffer()
{
	if(m_pSwapChain == nullptr)
	{
		std::cout << "Swapchain -> NULL" << std::endl;
		return;
	}
	for (UINT i = 0; i < m_bufferCountSwapchain; i++)
	{
		m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pSwapchainBuffer[i]));
	}
	std::cout << "Buffers created" << std::endl;
}

ID3D12Resource* Render_Swapchain::GetCurrentBackBuffer()
{
	return m_pSwapchainBuffer[m_currentBackBuffer];
}

int Render_Swapchain::GetCurrentBackBufferIndex() const
{
	return m_currentBackBuffer;
}

void Render_Swapchain::SetCurrentBackBufferIndex(int _index)
{
	m_currentBackBuffer = _index;
}

void Render_Swapchain::Resize(int _width, int _height)
{
	if (!m_pSwapChain) return;
	for (int i = 0; i < m_bufferCountSwapchain; ++i)
	{
		if (m_pSwapchainBuffer[i])
		{
			void* addr = (void*)m_pSwapchainBuffer[i];
			std::cout << "Releasing backbuffer " << i << " at address " << addr << std::endl;

			ULONG refCount = m_pSwapchainBuffer[i]->Release();
			std::cout << "RefCount after Release: " << refCount << std::endl;

			m_pSwapchainBuffer[i] = nullptr;
		}
	}

	HRESULT hr = m_pSwapChain->ResizeBuffers(
		m_bufferCountSwapchain,
		_width,
		_height,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	);

	if (FAILED(hr))
	{
		std::cout << "Erreur fatale Resize (HRESULT: " << std::hex << hr << ")" << std::endl;
		return;
	}

	m_currentBackBuffer = 0;
	CreateBuffer();
	std::cout << "Swapchain resized to " << _width << "x" << _height << std::endl;
}
