#pragma once
#include "d3d12.h"
class Render_RootSignature
{
private:
	ID3D12RootSignature* m_pRootSignature = nullptr;
public:

	Render_RootSignature(); 
	~Render_RootSignature();

	void CreateRootSignature(ID3D12Device* _device);
	ID3D12RootSignature* GetRootSignature();
};

