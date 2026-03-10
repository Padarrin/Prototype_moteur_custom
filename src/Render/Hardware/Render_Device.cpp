 #include "Render_Device.h"
#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl.h>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <d3d12sdklayers.h>
Render_Device::Render_Device()
{
}

Render_Device::~Render_Device()
{
    m_pD3dDevice->Release();
	m_pD3dDevice = nullptr;
}

bool Render_Device::Create()
{
    IDXGIFactory4* factory = nullptr;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) {
        std::cout << "CreateDXGIFactory1 failed\n";
        return false;
    }

    IDXGIAdapter1* adapter = nullptr;
    m_pD3dDevice = nullptr;

    for (UINT i = 0;; ++i) 
    {
        HRESULT hr = factory->EnumAdapters1(i, &adapter);
        if (hr == DXGI_ERROR_NOT_FOUND) break; 

        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pD3dDevice));

        if (SUCCEEDED(hr)) 
        {
            std::cout << "Device created " << std::endl;
            adapter->Release();
            break; 
        }
        else 
        {
            std::cout << "Device not created " << std::endl;
            adapter->Release();
        }
    }

    if (factory) 
        factory->Release();

    if (m_pD3dDevice == nullptr) 
    {
        std::cout << "Device can't create" << std::endl;
        return false;
    }

    return true;
}

ID3D12Device* Render_Device::GetDevice()
{
	if (m_pD3dDevice)
		return m_pD3dDevice;

	return nullptr;
}
