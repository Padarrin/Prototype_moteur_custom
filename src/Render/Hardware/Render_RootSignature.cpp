#include "Render_RootSignature.h"
#include "d3dx12.h"
#include <iostream>
Render_RootSignature::Render_RootSignature()
{
}

Render_RootSignature::~Render_RootSignature()
{
	m_pRootSignature->Release();
	m_pRootSignature = nullptr;
}

void Render_RootSignature::CreateRootSignature(ID3D12Device* _device)
{
	if (m_pRootSignature)
	{
		std::cout << "Root signature already created." << std::endl;
		return;
	}
	const int count = 3;
	CD3DX12_ROOT_PARAMETER slotRootParameters[count];
	slotRootParameters[0].InitAsConstantBufferView(0); 
	slotRootParameters[1].InitAsConstantBufferView(1);
	CD3DX12_DESCRIPTOR_RANGE srvTable;
	srvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	slotRootParameters[2].InitAsDescriptorTable(1, &srvTable, D3D12_SHADER_VISIBILITY_PIXEL);

	CD3DX12_STATIC_SAMPLER_DESC sampler(
		0, 
		D3D12_FILTER_MIN_MAG_MIP_POINT,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP
	);

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(count, slotRootParameters, 1, &sampler,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* serializedRootSig = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, &errorBlob);

	_device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_pRootSignature));

	std::cout << "Root signature created" << std::endl;
}

ID3D12RootSignature* Render_RootSignature::GetRootSignature()
{
	return m_pRootSignature;
}
