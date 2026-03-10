#pragma once

#include "d3dx12.h"

class Render_Device
{
private :
	ID3D12Device* m_pD3dDevice = nullptr;

public :
	Render_Device();
	~Render_Device();
	bool Create();

	ID3D12Device* GetDevice();
};

