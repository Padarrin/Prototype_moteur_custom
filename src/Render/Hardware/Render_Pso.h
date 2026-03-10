#pragma once
#include "d3dx12.h"
#include <vector>
class Render_Pso
{
private :
	ID3D12PipelineState* m_pPso = nullptr;
public:
	Render_Pso();
	~Render_Pso();

	void CreatePso(ID3D12Device* _device, std::vector<D3D12_INPUT_ELEMENT_DESC> _inputLayout, ID3D12RootSignature* _rootSignature
		, ID3DBlob* _vsByteCode, ID3DBlob* _psByteCode);
	ID3D12PipelineState* GetPso();

	void CreatePsoUI(ID3D12Device* _device, std::vector<D3D12_INPUT_ELEMENT_DESC> _inputLayout, ID3D12RootSignature* _rootSignature
		, ID3DBlob* _vsByteCode, ID3DBlob* _psByteCode);
	ID3D12PipelineState* GetPsoUI();
};

