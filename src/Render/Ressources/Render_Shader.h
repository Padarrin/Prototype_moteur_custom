#pragma once
#include "d3d12.h"
#include <vector>
#include <string>
#include <winstring.h>
class Render_Shader
{
private : 
	ID3DBlob* m_pVsByteCode = nullptr;
	ID3DBlob* m_pPsByteCode = nullptr;
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_inputLayout;
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_inputLayoutUI;
public :
	Render_Shader();
	~Render_Shader();

	void CreateShader(const std::wstring& _path);
	void CreateInputLayout();
	void CreateInputLayoutUI();
	ID3DBlob* GetShaderVsByteCode();
	ID3DBlob* GetShaderPsByteCode();

	std::vector<D3D12_INPUT_ELEMENT_DESC> GetInputLayout();
	std::vector<D3D12_INPUT_ELEMENT_DESC> GetInputLayoutUI();
};

